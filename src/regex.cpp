#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>

#include "regex.h"
#include "regex/automata/nfa.h"
#include "regex/language/parser.h"
#include "regex/utilities/cmdline.h"
#include "regex/utilities/compile.h"

int main( int argc, const char **argv )
{
    regex::cmd::cmdline args( "Pattern matching tool" );
    args.add_flag( "--version", "version", "Version number" );
    args.add_flag( "-v", "verbose", "Verbose logging" );
    args.add_positional( "expression", regex::cmd::cmdline::type::string, "Regular expression" );
    args.add_positional( "target", regex::cmd::cmdline::type::string, "Target to match" );
    args.add_optional( "-t", "type", regex::cmd::cmdline::type::string, "nfa", "Type of finite automata",
                       { "nfa", "dfa" } );

    try
    {
        args.parse( argc, argv );
    }
    catch ( const regex::cmd::exception &e )
    {
        std::cout << "Regex version " << REGEX_VERSION_MAJOR << "." << REGEX_VERSION_MINOR << std::endl;
        std::cerr << e.what() << '\n';
        return 1;
    }

    if ( args.get_flag( "version" ) )
    {
        std::cout << REGEX_VERSION_MAJOR << "." << REGEX_VERSION_MINOR << std::endl;
        return 0;
    }

    std::stringstream pattern( args.get_argument<std::string>( "expression" ) );
    std::string target( args.get_argument<std::string>( "target" ) );
    regex::compile_flag flag =
        args.get_argument<std::string>( "type" ) == "nfa" ? regex::compile_flag::nfa : regex::compile_flag::dfa;

    std::shared_ptr<regex::fa> automata = regex::compile( std::move( pattern ), flag );

    std::ifstream file( target, std::ios_base::in );
    std::string line;

    while ( std::getline( file, line ) )
    {
        if ( automata->execute( line ) )
        {
            std::cout << line << std::endl;
        }
    }

    return 0;
}