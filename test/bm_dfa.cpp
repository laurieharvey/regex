#include "benchmark/benchmark.h"
#include "regex/utilities/compile.h"
#include <iostream>

static void bm_compile( benchmark::State &state )
{
    std::string input( state.range( 0 ), 'a' );

    for ( auto _ : state ) // iterate iterations
    {
        // only code in here is benchmarked
        regex::compile( input, regex::compile_flag::nfa );
    }

    // loop exits when noise is sufficiently low
}

BENCHMARK( bm_compile )->Arg( 1 << 0 );
BENCHMARK( bm_compile )->Arg( 1 << 1 );
BENCHMARK( bm_compile )->Arg( 1 << 2 );
BENCHMARK( bm_compile )->Arg( 1 << 3 );
BENCHMARK( bm_compile )->Arg( 1 << 4 );
BENCHMARK( bm_compile )->Arg( 1 << 5 );
BENCHMARK( bm_compile )->Arg( 1 << 6 );
BENCHMARK( bm_compile )->Arg( 1 << 7 );
BENCHMARK( bm_compile )->Arg( 1 << 8 );

