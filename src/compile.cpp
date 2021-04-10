#include "compile.h"

#include <sstream>

#include "compile.h"
#include "language/parser.h"
#include "automata/dfa.h"
#include "automata/fa.h"
#include "automata/nfa.h"
#include "language/ast.h"

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

    std::shared_ptr<regex::nfa> compile_nfa( std::basic_stringstream<language::character_type> expression )
    {
        std::unique_ptr<language::token> ast = regex::parse( expression );

        regex::nfa_generator g;

        ast->walk( std::bind( &regex::nfa_generator::callback, &g, std::placeholders::_1 ) );

        return g.result( );
    }

    std::shared_ptr<regex::dfa> compile_dfa( std::basic_stringstream<language::character_type> expression )
    {
        std::unique_ptr<language::token> ast = regex::parse( expression );

        regex::dfa_generator g;

        ast->walk( std::bind( &regex::dfa_generator::callback, &g, std::placeholders::_1 ) );

        return g.result( );
    }
}