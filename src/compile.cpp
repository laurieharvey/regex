#include <sstream>

#include "automata/dfa.h"
#include "automata/fa.h"
#include "automata/nfa.h"
#include "utilities/compile.h"
#include "language/ast.h"
#include "language/parser.h"

namespace regex
{
    std::shared_ptr<regex::fa> compile( std::basic_stringstream<language::character_type> expression, compile_flag flag )
    {
        if( flag == compile_flag::nfa )
        {
            return compile_nfa( std::move( expression ) );
        }
        else
        {
            return compile_dfa( std::move( expression ) );
        }
    }

    std::shared_ptr<nfa> compile_nfa( std::unique_ptr<language::token> expression )
    {
        std::stack<std::shared_ptr<nfa>> result;

        auto generator = [ &result ]( const language::token &token ){

            std::shared_ptr<regex::nfa> lhs, rhs;

            switch( token.get_type( ) )
            {
                case language::type::character:
                    result.push( nfa::from_character( token.get_token( ) ) );
                    break;
                case language::type::any:
                    result.push( nfa::from_any( ) );
                    break;
                case language::type::alternation:
                    rhs = result.top( );
                    result.pop( );
                    lhs = result.top( );
                    result.pop( );
                    result.push( nfa::from_alternation( lhs, rhs ) );
                    break;
                case language::type::concatenation:
                    rhs = result.top( );
                    result.pop( );
                    lhs = result.top( );
                    result.pop( );
                    result.push( nfa::from_concatenation( lhs, rhs ) );
                    break;
                case language::type::kleene:
                    lhs = result.top( );
                    result.pop( );
                    result.push( nfa::from_kleene( lhs ) );
                    break;
                case language::type::zero_or_one:
                    lhs = result.top( );
                    result.pop( );
                    result.push( nfa::from_alternation( nfa::from_epsilon( ), lhs ) );
                    break;
                case language::type::one_or_more:
                    lhs = result.top( );
                    result.pop( );
                    rhs = lhs;
                    result.push( nfa::from_concatenation( lhs, nfa::from_kleene( rhs ) ) );
                    break;
                case language::type::parenthesis:
                    break;
            }
        };

        expression->walk( generator );

        return result.top( );
    }

    std::shared_ptr<regex::nfa> compile_nfa( std::basic_stringstream<language::character_type> expression )
    {
        return compile_nfa( regex::language::parse( std::move( expression ) ) );
    }

    std::shared_ptr<regex::dfa> compile_dfa( std::unique_ptr<language::token> expression )
    {
        std::stack<std::shared_ptr<dfa>> result;

        auto generator = [ &result ]( const language::token &token ){

            std::shared_ptr<dfa> lhs, rhs;

            switch( token.get_type( ) )
            {
                case language::type::character:
                    result.push( dfa::from_character( token.get_token( ) ) );
                    break;
                case language::type::any:
                    result.push( dfa::from_any( ) );
                    break;
                case language::type::alternation:
                    rhs = result.top( );
                    result.pop( );
                    lhs = result.top( );
                    result.pop( );
                    result.push( dfa::from_alternation( lhs, rhs ) );
                    break;
                case language::type::concatenation:
                    rhs = result.top( );
                    result.pop( );
                    lhs = result.top( );
                    result.pop( );
                    result.push( dfa::from_concatenation( lhs, rhs ) );
                    break;
                case language::type::kleene:
                    lhs = result.top( );
                    result.pop( );
                    result.push( dfa::from_kleene( lhs ) );
                    break;
                case language::type::zero_or_one:
                    lhs = result.top( );
                    result.pop( );
                    result.push( dfa::from_zero_or_one( lhs ) );
                    break;
                case language::type::one_or_more:
                    lhs = result.top( );
                    result.pop( );
                    rhs = std::make_shared<dfa>( *lhs );
                    result.push( dfa::from_concatenation( lhs, dfa::from_kleene( rhs ) ) );
                    break;
                case language::type::parenthesis:
                    break;
            }

        };

        expression->walk( generator );

        return result.top( );
    }

    std::shared_ptr<regex::dfa> compile_dfa( std::basic_stringstream<language::character_type> expression )
    {
        return compile_dfa( regex::language::parse( std::move( expression ) ) );
    }
}