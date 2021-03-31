#include "compile.h"

#include <sstream>

#include "ast.h"
#include "automata/dfa.h"
#include "automata/fa.h"
#include "automata/nfa.h"
#include "parser.h"

std::shared_ptr<regex::fa> compile( std::basic_stringstream<regex::character_type> pattern, compile_flag flag )
{
    if( flag == compile_flag::nfa )
    {
        return compile_nfa( std::move( pattern ) );
    }
    else
    {
        return compile_dfa( std::move( pattern ) );
    }
}

std::shared_ptr<regex::nfa> compile_nfa( std::basic_stringstream<regex::character_type> pattern )
{
    std::basic_stringstream<regex::character_type> explicit_pattern = regex::make_explicit( pattern );
    std::unique_ptr<regex::token> ast = regex::parse( explicit_pattern );

    regex::nfa_generator g;

    ast->walk( std::bind( &regex::nfa_generator::callback, &g, std::placeholders::_1 ) );

    return g.result( );
}

std::shared_ptr<regex::dfa> compile_dfa( std::basic_stringstream<regex::character_type> pattern )
{
    std::basic_stringstream<regex::character_type> explicit_pattern = regex::make_explicit( pattern );
    std::unique_ptr<regex::token> ast = regex::parse( explicit_pattern );

    regex::dfa_generator g;

    ast->walk( std::bind( &regex::dfa_generator::callback, &g, std::placeholders::_1 ) );

    return g.result( );
}