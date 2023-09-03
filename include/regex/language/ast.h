#pragma once

#include <exception>
#include <functional>
#include <limits>
#include <memory>
#include <ostream>
#include <stack>
#include <vector>

#include "regex/language/alphabet.h"
#include "regex/language/parser.h"
#include "regex/memory/pool_allocator.h"

namespace regex::language
{
    struct token
    {
        character_type character;
        token *_lhs;
        token *_rhs;
    };

    template <typename Allocator = std::allocator<token>>
    class ast : public Allocator
    {
      public:
        /**
         * Construct ast from an expression
         * @param expression
         */
        ast( std::basic_string_view<character_type> expression )
            : Allocator()
            , _root( _parse( expression ) )
        {
        }
        /**
         * Construct ast from an expression using specified allocator
         * @param expression
         * @param allocator
         */
        ast( std::basic_string_view<character_type> expression, Allocator &&allocator )
            : Allocator( std::move( allocator ) )
            , _root( _parse( expression ) )
        {
        }
        /**
         * Move construct from another ast. Leaves other ast empty.
         * @param rhs
         */
        ast( ast &&rhs ) noexcept
            : _root( rhs._root )
        {
            rhs._root = nullptr;
        }
        /**
         * Destroy ast.
         */
        ~ast()
        {
            _erase( _root );
        }
        /**
         * Iterate the ast in postfix order
         * @tparam F
         * @param callback function of signature void(character_type)
         */
        template <typename F>
        void postfix( F callback ) const;

      private:
        token *_root = nullptr;
        token *_parse( std::basic_string_view<character_type> expression );
        void _erase( token *node )
        {
            while( node )
            {
                if( node->_rhs )
                    _erase( node->_rhs );
                token *tmp = node->_lhs;
                node->~token();
                std::allocator_traits<Allocator>::deallocate( *this, node, 1 );
                node = tmp;
            }
        };
    };

    template <typename Allocator = std::allocator<token>>
    inline ast<Allocator> parse( std::basic_string_view<character_type> expression )
    {
        return regex::language::ast<Allocator>( make_explicit( expression ) );
    }

    template <>
    inline ast<pool_allocator<token>> parse<pool_allocator<token>>( std::basic_string_view<character_type> expression )
    {
        std::string explicit_expression = make_explicit( expression );
        pool_allocator<token> allocator( explicit_expression.size() );
        return { explicit_expression, std::move( allocator ) };
    }

    template <typename Allocator>
    token *ast<Allocator>::_parse( std::basic_string_view<character_type> expression )
    {
        std::stack<token *> output;
        std::stack<character_type> ops;

        auto check_args = [&output]( unsigned short args_needed ) {
            if( output.size() < args_needed )
                throw std::runtime_error( "Expected " + std::to_string( args_needed ) + " arguments but only have " +
                                          std::to_string( output.size() ) );
        };

        token *op, *lhs, *rhs;

        for( character_type character : expression )
        {
            if( character == '*' || character == '?' || character == '|' || character == '-' || character == '+' )
            {
                while( !ops.empty() && ops.top() != '(' && precedence[character] < precedence[ops.top()] )
                {
                    switch( ops.top() )
                    {
                    case '*':
                    case '?':
                    case '+':
                        check_args( 1 );
                        lhs = output.top();
                        output.pop();
                        op = this->allocate( 1 );
                        new( op ) token( ops.top(), lhs, nullptr );
                        output.push( op );
                        break;
                    case '-':
                    case '|':
                        check_args( 2 );
                        rhs = output.top();
                        output.pop();
                        lhs = output.top();
                        output.pop();
                        op = this->allocate( 1 );
                        new( op ) token( ops.top(), lhs, rhs );
                        break;
                    }
                    ops.pop();
                }
                ops.push( character );
            }
            else if( character == '(' )
            {
                ops.push( character );
            }
            else if( character == ')' )
            {
                while( ops.top() != '(' )
                {
                    switch( ops.top() )
                    {
                    case '*':
                    case '?':
                    case '+':
                        check_args( 1 );
                        lhs = output.top();
                        output.pop();
                        op = this->allocate( 1 );
                        new( op ) token( ops.top(), lhs, nullptr );
                        output.push( op );
                        break;
                    case '-':
                    case '|':
                        check_args( 2 );
                        rhs = output.top();
                        output.pop();
                        lhs = output.top();
                        output.pop();
                        op = this->allocate( 1 );
                        new( op ) token( ops.top(), lhs, rhs );
                        output.push( op );
                        break;
                    }
                    ops.pop();
                }
                lhs = output.top();
                output.pop();
                op = this->allocate( 1 );
                new( op ) token( character, lhs, nullptr );
                output.push( op );
                ops.pop();
            }
            else
            {
                lhs = this->allocate( 1 );
                new( lhs ) token( character, nullptr, nullptr );
                output.push( lhs );
            }
        }
        while( !ops.empty() )
        {
            switch( ops.top() )
            {
            case '*':
            case '?':
            case '+':
                check_args( 1 );
                lhs = output.top();
                output.pop();
                op = this->allocate( 1 );
                new( op ) token( ops.top(), lhs, nullptr );
                output.push( op );
                break;
            case '-':
            case '|':
                check_args( 2 );
                rhs = output.top();
                output.pop();
                lhs = output.top();
                output.pop();
                op = this->allocate( 1 );
                new( op ) token( ops.top(), lhs, rhs );
                output.push( op );
                break;
            case ')':
            case '(':
                check_args( 1 );
                lhs = output.top();
                output.pop();
                op = this->allocate( 1 );
                new( op ) token( ops.top(), lhs, nullptr );
                output.push( op );
                break;
            }
            ops.pop();
        }

        if( output.size() != 1 )
            throw std::runtime_error( std::to_string( output.size() - 1 ) + " unmatched arguments remaining" );

        return output.top();
    }

    template <typename F>
    void postfix( token *t, F callback )
    {
        if( t->_lhs )
            postfix( t->_lhs, callback );

        if( t->_rhs )
            postfix( t->_rhs, callback );

        callback( t->character );
    }

    template <typename Allocator>
    template <typename T>
    void ast<Allocator>::postfix( T callback ) const
    {
        if( _root )
            ::regex::language::postfix<T>( _root, callback );
    }

    template <typename Allocator>
    std::string to_string( ast<Allocator> a )
    {
        std::stack<std::string> args;
        std::string arg1, arg2;

        auto cbk = [&]( character_type character ) {
            switch( character )
            {
            case '*':
            case '?':
            case '+':
                args.top() = args.top() + character;
                break;
            case '-':
                arg2 = args.top();
                args.pop();
                arg1 = args.top();
                args.pop();
                args.push( arg1 + arg2 );
                break;
            case '|':
                arg2 = args.top();
                args.pop();
                arg1 = args.top();
                args.pop();
                args.push( arg1 + character + arg2 );
                break;
            case ')':
            case '(':
                args.top() = '(' + args.top() + ')';
                break;
            default:
                args.emplace( 1, character );
            }
        };

        a.postfix( cbk );

        return args.top();
    }
} // namespace regex::language