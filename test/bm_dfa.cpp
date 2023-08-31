#include "benchmark/benchmark.h"
#include "regex/utilities/compile.h"
#include <sstream>

static void bm_compile( benchmark::State &state )
{
    std::string input( 100, 'a' );

    for ( auto _ : state ) // iterate iterations
    {
        // only code in here is benchmarked
        std::basic_stringstream<regex::language::character_type> ss( input );
        regex::compile( std::move( ss ), regex::compile_flag::nfa );
    }

    // loop exits when noise is sufficiently low
}

BENCHMARK( bm_compile );