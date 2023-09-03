#include "benchmark/benchmark.h"
#include "regex/memory/pool_allocator.h"
#include "regex/utilities/compile.h"
#include <iostream>

static void benchmark_ast( benchmark::State &state )
{
    std::string input( state.range( 0 ), 'a' );

    for( auto _ : state ) // iterate iterations
    {
        // only code in here is benchmarked
        benchmark::DoNotOptimize( regex::language::parse<std::allocator<regex::language::token>>( input ) );
    }

    // loop exits when noise is sufficiently low
}

static void benchmark_ast_block_allocator( benchmark::State &state )
{
    std::string input( state.range( 0 ), 'a' );

    for( auto _ : state ) // iterate iterations
    {
        // only code in here is benchmarked
        benchmark::DoNotOptimize( regex::language::parse<pool_allocator<regex::language::token>>( input ) );
    }

    // loop exits when noise is sufficiently low
}

BENCHMARK( benchmark_ast )->Arg( 1 << 0 );
BENCHMARK( benchmark_ast )->Arg( 1 << 1 );
BENCHMARK( benchmark_ast )->Arg( 1 << 2 );
BENCHMARK( benchmark_ast )->Arg( 1 << 3 );
BENCHMARK( benchmark_ast )->Arg( 1 << 4 );
BENCHMARK( benchmark_ast )->Arg( 1 << 5 );
BENCHMARK( benchmark_ast )->Arg( 1 << 6 );
BENCHMARK( benchmark_ast )->Arg( 1 << 7 );
BENCHMARK( benchmark_ast )->Arg( 1 << 8 );

BENCHMARK( benchmark_ast_block_allocator )->Arg( 1 << 0 );
BENCHMARK( benchmark_ast_block_allocator )->Arg( 1 << 1 );
BENCHMARK( benchmark_ast_block_allocator )->Arg( 1 << 2 );
BENCHMARK( benchmark_ast_block_allocator )->Arg( 1 << 3 );
BENCHMARK( benchmark_ast_block_allocator )->Arg( 1 << 4 );
BENCHMARK( benchmark_ast_block_allocator )->Arg( 1 << 5 );
BENCHMARK( benchmark_ast_block_allocator )->Arg( 1 << 6 );
BENCHMARK( benchmark_ast_block_allocator )->Arg( 1 << 7 );
BENCHMARK( benchmark_ast_block_allocator )->Arg( 1 << 8 );
