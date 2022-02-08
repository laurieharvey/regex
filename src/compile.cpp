#include <sstream>

#include "automata/dfa.h"
#include "automata/nfa.h"
#include "language/ast.h"
#include "language/parser.h"
#include "utilities/compile.h"

namespace regex
{
    static std::unique_ptr<nfa> compile_nfa( std::unique_ptr<language::token> expression )
    {
        std::stack<std::unique_ptr<nfa>> result;

        auto generator = [&result]( const language::token &token ) {
            std::unique_ptr<regex::nfa> lhs, rhs;

            switch ( token.get_type() )
            {
            case language::type::character:
                result.push( nfa::from_character( token.get_token() ) );
                break;
            case language::type::any:
                result.push( nfa::from_any() );
                break;
            case language::type::alternation:
                rhs = std::move( result.top() );
                result.pop();
                lhs = std::move( result.top() );
                result.pop();
                result.push( nfa::from_alternation( std::move( lhs ), std::move( rhs ) ) );
                break;
            case language::type::concatenation:
                rhs = std::move( result.top() );
                result.pop();
                lhs = std::move( result.top() );
                result.pop();
                result.push( nfa::from_concatenation( std::move( lhs ), std::move( rhs ) ) );
                break;
            case language::type::kleene:
                lhs = std::move( result.top() );
                result.pop();
                result.push( nfa::from_kleene( std::move( lhs ) ) );
                break;
            case language::type::zero_or_one:
                lhs = std::move( result.top() );
                result.pop();
                result.push( nfa::from_alternation( nfa::from_epsilon(), std::move( lhs ) ) );
                break;
            case language::type::one_or_more:
                lhs = std::move( result.top() );
                result.pop();
                rhs = std::make_unique<regex::nfa>( *lhs );
                result.push( nfa::from_concatenation( std::move( lhs ), nfa::from_kleene( std::move( rhs ) ) ) );
                break;
            case language::type::parenthesis:
                break;
            }
        };

        expression->walk( generator );

        return std::move( result.top() );
    }

    static std::unique_ptr<regex::nfa> compile_nfa( std::basic_stringstream<language::character_type> expression )
    {
        return compile_nfa( regex::language::parse( std::move( expression ) ) );
    }

    static std::unique_ptr<regex::dfa> compile_dfa( std::unique_ptr<language::token> expression )
    {
        return nfa::to_dfa( compile_nfa( std::move( expression ) ) );
    }

    static std::unique_ptr<regex::dfa> compile_dfa( std::basic_stringstream<language::character_type> expression )
    {
        return compile_dfa( regex::language::parse( std::move( expression ) ) );
    }

    std::unique_ptr<regex::fa> compile( std::unique_ptr<language::token> expression, compile_flag flag )
    {
        if ( flag == compile_flag::nfa )
        {
            return compile_nfa( std::move( expression ) );
        }
        else
        {
            return compile_dfa( std::move( expression ) );
        }
    }

    std::unique_ptr<regex::fa> compile( std::basic_stringstream<language::character_type> expression,
                                        compile_flag flag )
    {
        if ( flag == compile_flag::nfa )
        {
            return compile_nfa( std::move( expression ) );
        }
        else
        {
            return compile_dfa( std::move( expression ) );
        }
    }
} // namespace regex
