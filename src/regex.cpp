#include <functional>
#include <iostream>
#include <sstream>

#include "automata/nfa.h"
#include "cmdline.h"
#include "language/parser.h"
#include "state/state.h"
#include "utilities/compile.h"

int main( int argc, const char **argv )
{
    regex::cmd::cmdline args( "Pattern matching tool" );

    args.add_optional( "-t", "type", regex::cmd::cmdline::type::string, "nfa", "Type of finite automata",
                       { "nfa", "dfa" } );
    args.add_flag( "-v", "verbose", "Verbose logging" );
    args.add_positional( "expression", regex::cmd::cmdline::type::string, "Regular expression" );
    args.add_positional( "target", regex::cmd::cmdline::type::string, "Target to match" );

    try
    {
        args.parse( argc, argv );
    }
    catch ( const regex::cmd::exception &e )
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::stringstream pattern( args.get_argument<std::string>( "expression" ) );
    std::string target( args.get_argument<std::string>( "target" ) );
    regex::compile_flag flag =
        args.get_argument<std::string>( "type" ) == "nfa" ? regex::compile_flag::nfa : regex::compile_flag::dfa;

    std::shared_ptr<regex::fa> automata = regex::compile( std::move( pattern ), flag );

    if ( automata->execute( target ) == regex::match::accepted )
    {
        std::cout << "Match" << std::endl;
    }
    else
    {
        std::cout << "No match" << std::endl;
    }

    return 0;
}