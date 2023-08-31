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

namespace regex::language
{
    struct token
    {
        character_type character;
        token *_lhs;
        token *_rhs;
    };

    template <typename Allocator = std::allocator<token>> class ast : public Allocator
    {
        token *_root = nullptr;

      public:
        ast() = default;

        explicit ast( std::basic_string_view<character_type> expression );
        ast( ast &&rhs )  noexcept : _root( rhs._root )
        {
            rhs._root = nullptr;
        }

        void erase( token *node )
        {
            while( node )
            {
                if( node->_rhs )
                    erase( node->_rhs );
                token *tmp = node->_lhs;
                node->~token();
                std::allocator_traits<Allocator>::deallocate( *this, node, 1 );
                node = tmp;
            }
        };

        ~ast()
        {
            erase( _root );
        }

        template <typename F> void walk( F callback ) const;
    };

    template <typename Allocator> ast<Allocator>::ast( std::basic_string_view<character_type> expression ) : Allocator()
    {
        std::stack<token *> output;
        std::stack<character_type> ops;
        ast<Allocator> tree;
        std::basic_string<character_type> explicit_expression = make_explicit( expression );

        auto check_args = [&output]( unsigned short args_needed ) {
            if( output.size() < args_needed )
                throw std::runtime_error( "Expected " + std::to_string( args_needed ) + " arguments but only have " +
                                          std::to_string( output.size() ) );
        };

        token *op, *lhs, *rhs;

        for( character_type character : explicit_expression )
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
                        op = tree.allocate( 1 );
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
                        op = tree.allocate( 1 );
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
                        op = tree.allocate( 1 );
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
                        op = tree.allocate( 1 );
                        new( op ) token( ops.top(), lhs, rhs );
                        output.push( op );
                        break;
                    }
                    ops.pop();
                }
                lhs = output.top();
                output.pop();
                op = tree.allocate( 1 );
                new( op ) token( character, lhs, nullptr );
                output.push( op );
                ops.pop();
            }
            else
            {
                lhs = tree.allocate( 1 );
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
                op = tree.allocate( 1 );
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
                op = tree.allocate( 1 );
                new( op ) token( ops.top(), lhs, rhs );
                output.push( op );
                break;
            case ')':
            case '(':
                check_args( 1 );
                lhs = output.top();
                output.pop();
                op = tree.allocate( 1 );
                new( op ) token( ops.top(), lhs, nullptr );
                output.push( op );
                break;
            }
            ops.pop();
        }

        if( output.size() != 1 )
            throw std::runtime_error( std::to_string( output.size() - 1 ) + " unmatched arguments remaining" );

        _root = output.top();
    }

    template <typename F> void walk( token *t, F callback )
    {
        if( t->_lhs )
            walk( t->_lhs, callback );

        if( t->_rhs )
            walk( t->_rhs, callback );

        callback( t->character );
    }

    template <typename Allocator> template <typename T> void ast<Allocator>::walk( T callback ) const
    {
        if( _root )
            ::regex::language::walk<T>( _root, callback );
    }

    template <typename Allocator> std::string to_string( ast<Allocator> a )
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

        a.walk( cbk );

        return args.top();
    }
} // namespace regex::language