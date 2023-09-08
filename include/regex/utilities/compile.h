#pragma once

#include <memory>
#include <sstream>

#include "regex/automata/dfa.h"
#include "regex/automata/nfa.h"
#include "regex/language/alphabet.h"
#include "regex/language/ast.h"

namespace regex
{
    enum class compile_flag
    {
        nfa,
        dfa
    };

    template <typename Allocator>
    std::unique_ptr<nfa> compile_nfa( const language::ast<Allocator> &a )
    {
        std::stack<std::unique_ptr<nfa>> result;

        auto generator = [&result]( language::character_type character ) {
            std::unique_ptr<regex::nfa> lhs, rhs;

            switch( character )
            {
            case '.':
                result.push( nfa::from_any() );
                break;
            case '|':
                rhs = std::move( result.top() );
                result.pop();
                lhs = std::move( result.top() );
                result.pop();
                result.push( nfa::from_alternation( std::move( lhs ), std::move( rhs ) ) );
                break;
            case '-':
                rhs = std::move( result.top() );
                result.pop();
                lhs = std::move( result.top() );
                result.pop();
                result.push( nfa::from_concatenation( std::move( lhs ), std::move( rhs ) ) );
                break;
            case '*':
                lhs = std::move( result.top() );
                result.pop();
                result.push( nfa::from_kleene( std::move( lhs ) ) );
                break;
            case '?':
                lhs = std::move( result.top() );
                result.pop();
                result.push( nfa::from_alternation( nfa::from_epsilon(), std::move( lhs ) ) );
                break;
            case '+':
                lhs = std::move( result.top() );
                result.pop();
                rhs = std::make_unique<regex::nfa>( *lhs );
                result.push( nfa::from_concatenation( std::move( lhs ), nfa::from_kleene( std::move( rhs ) ) ) );
                break;
            case '(':
            case ')':
                break;
            default:
                result.push( nfa::from_character( character ) );
                break;
            }
        };

        a.postfix( generator );

        return std::move( result.top() );
    }

    template <typename Allocator>
    std::unique_ptr<regex::dfa> compile_dfa( const language::ast<Allocator> &a )
    {
        return compile_nfa( std::move( a ) )->to_dfa();
    }
    /*
     * Compile the regular expression to its finite automaton
     */
    std::unique_ptr<regex::nfa> compile_nfa( std::basic_string_view<language::character_type> expression );
    /*
     * Compile the regular expression to its finite automaton
     */
    std::unique_ptr<regex::dfa> compile_dfa( std::basic_string_view<language::character_type> expression );
    /*
     * Compile the regular expression to its finite automaton
     */
    std::unique_ptr<regex::fa> compile( std::basic_string_view<language::character_type> expression, compile_flag flag );
} // namespace regex
