#include <functional>
#include <iostream>
#include <sstream>

#include "automata/nfa.h"
#include "utilities/compile.h"
#include "language/parser.h"
#include "cmdline.h"
#include "state/state.h"

int main( int argc, char **argv )
{
    std::stringstream pattern( argv[1] );
    std::string input_line;

    std::shared_ptr<regex::fa> nfa = regex::compile( std::move( pattern ), regex::compile_flag::nfa );

    while( std::getline( std::cin, input_line ) )
    {
        regex::match result = nfa->execute( input_line );

        if( result == regex::match::accepted )
        {
            std::cout << input_line << std::endl;
        }
    }

    return 0;
}