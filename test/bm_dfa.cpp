#include "benchmark/benchmark.h"
#include "regex/memory/pool_allocator.h"
#include "regex/utilities/compile.h"
#include <iostream>

static void benchmark_ast( benchmark::State &state )
{
    std::int64_t n = state.range( 0 );
    std::stringstream ss;
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a?";
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a";
    std::string expression = ss.str();

    for( auto _ : state ) // iterate iterations
    {
        // only code in here is benchmarked
        benchmark::DoNotOptimize( regex::language::parse<std::allocator<regex::language::token>>( expression ) );
        benchmark::ClobberMemory();
    }

    // loop exits when noise is sufficiently low
}

static void benchmark_ast_block_allocator( benchmark::State &state )
{
    std::int64_t n = state.range( 0 );
    std::stringstream ss;
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a?";
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a";
    std::string expression = ss.str();

    for( auto _ : state ) // iterate iterations
    {
        // only code in here is benchmarked
        benchmark::DoNotOptimize( regex::language::parse<pool_allocator<regex::language::token>>( expression ) );
        benchmark::ClobberMemory();
    }

    // loop exits when noise is sufficiently low
}

static void benchmark_compile_nfa_from_ast( benchmark::State &state )
{
    std::int64_t n = state.range( 0 );
    std::stringstream ss;
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a?";
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a";
    regex::language::ast<pool_allocator<regex::language::token>> expression =
        regex::language::parse<pool_allocator<regex::language::token>>( ss.str() );

    for( auto _ : state ) // iterate iterations
    {
        // only code in here is benchmarked
        benchmark::DoNotOptimize( regex::compile_nfa( expression ) );
        benchmark::ClobberMemory();
    }

    // loop exits when noise is sufficiently low
}

static void benchmark_compile_dfa_from_ast( benchmark::State &state )
{
    std::int64_t n = state.range( 0 );
    std::stringstream ss;
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a?";
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a";
    std::unique_ptr<regex::nfa> expression = regex::compile_nfa( ss.str() );

    for( auto _ : state ) // iterate iterations
    {
        // only code in here is benchmarked
        benchmark::DoNotOptimize( expression->to_dfa() );
        benchmark::ClobberMemory();
    }

    // loop exits when noise is sufficiently low
}

static void benchmark_compile_nfa_from_string( benchmark::State &state )
{
    std::int64_t n = state.range( 0 );
    std::stringstream ss;
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a?";
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a";
    std::string expression = ss.str();

    for( auto _ : state ) // iterate iterations
    {
        // only code in here is benchmarked
        benchmark::DoNotOptimize( regex::compile_nfa( expression ) );
        benchmark::ClobberMemory();
    }

    // loop exits when noise is sufficiently low
}

static void benchmark_compile_dfa_from_string( benchmark::State &state )
{
    std::int64_t n = state.range( 0 );
    std::stringstream ss;
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a?";
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a";
    std::string expression = ss.str();
    
    for( auto _ : state ) // iterate iterations
    {
        // only code in here is benchmarked
        benchmark::DoNotOptimize( regex::compile_dfa( expression ) );
        benchmark::ClobberMemory();
    }

    // loop exits when noise is sufficiently low
}

static void benchmark_execute_nfa( benchmark::State &state )
{
    std::int64_t n = state.range( 0 );
    std::stringstream ss;
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a?";
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a";
    std::unique_ptr<regex::nfa> expression = regex::compile_nfa( ss.str() );
    std::string input( n, 'a' );

    for( auto _ : state ) // iterate iterations
    {
        // only code in here is benchmarked
        benchmark::DoNotOptimize( expression->execute( input ) );
        benchmark::ClobberMemory();
    }

    // loop exits when noise is sufficiently low
}

static void benchmark_execute_dfa( benchmark::State &state )
{
    std::int64_t n = state.range( 0 );
    std::stringstream ss;
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a?";
    for( std::int64_t i = 0; i < n; i++ )
        ss << "a";
    std::unique_ptr<regex::dfa> expression = regex::compile_dfa( ss.str() );
    std::string input( n, 'a' );

    for( auto _ : state ) // iterate iterations
    {
        // only code in here is benchmarked
        benchmark::DoNotOptimize( expression->execute( input ) );
        benchmark::ClobberMemory();
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

BENCHMARK( benchmark_compile_nfa_from_ast )->Arg( 1 << 0 );
BENCHMARK( benchmark_compile_nfa_from_ast )->Arg( 1 << 1 );
BENCHMARK( benchmark_compile_nfa_from_ast )->Arg( 1 << 2 );
BENCHMARK( benchmark_compile_nfa_from_ast )->Arg( 1 << 3 );
BENCHMARK( benchmark_compile_nfa_from_ast )->Arg( 1 << 4 );
BENCHMARK( benchmark_compile_nfa_from_ast )->Arg( 1 << 5 );
BENCHMARK( benchmark_compile_nfa_from_ast )->Arg( 1 << 6 );
BENCHMARK( benchmark_compile_nfa_from_ast )->Arg( 1 << 7 );
BENCHMARK( benchmark_compile_nfa_from_ast )->Arg( 1 << 8 );

BENCHMARK( benchmark_compile_dfa_from_ast )->Arg( 1 << 0 );
BENCHMARK( benchmark_compile_dfa_from_ast )->Arg( 1 << 1 );
BENCHMARK( benchmark_compile_dfa_from_ast )->Arg( 1 << 2 );
BENCHMARK( benchmark_compile_dfa_from_ast )->Arg( 1 << 3 );
BENCHMARK( benchmark_compile_dfa_from_ast )->Arg( 1 << 4 );
BENCHMARK( benchmark_compile_dfa_from_ast )->Arg( 1 << 5 );
BENCHMARK( benchmark_compile_dfa_from_ast )->Arg( 1 << 6 );
BENCHMARK( benchmark_compile_dfa_from_ast )->Arg( 1 << 7 );
BENCHMARK( benchmark_compile_dfa_from_ast )->Arg( 1 << 8 );

BENCHMARK( benchmark_compile_nfa_from_string )->Arg( 1 << 0 );
BENCHMARK( benchmark_compile_nfa_from_string )->Arg( 1 << 1 );
BENCHMARK( benchmark_compile_nfa_from_string )->Arg( 1 << 2 );
BENCHMARK( benchmark_compile_nfa_from_string )->Arg( 1 << 3 );
BENCHMARK( benchmark_compile_nfa_from_string )->Arg( 1 << 4 );
BENCHMARK( benchmark_compile_nfa_from_string )->Arg( 1 << 5 );
BENCHMARK( benchmark_compile_nfa_from_string )->Arg( 1 << 6 );
BENCHMARK( benchmark_compile_nfa_from_string )->Arg( 1 << 7 );
BENCHMARK( benchmark_compile_nfa_from_string )->Arg( 1 << 8 );

BENCHMARK( benchmark_compile_dfa_from_string )->Arg( 1 << 0 );
BENCHMARK( benchmark_compile_dfa_from_string )->Arg( 1 << 1 );
BENCHMARK( benchmark_compile_dfa_from_string )->Arg( 1 << 2 );
BENCHMARK( benchmark_compile_dfa_from_string )->Arg( 1 << 3 );
BENCHMARK( benchmark_compile_dfa_from_string )->Arg( 1 << 4 );
BENCHMARK( benchmark_compile_dfa_from_string )->Arg( 1 << 5 );
BENCHMARK( benchmark_compile_dfa_from_string )->Arg( 1 << 6 );
BENCHMARK( benchmark_compile_dfa_from_string )->Arg( 1 << 7 );
BENCHMARK( benchmark_compile_dfa_from_string )->Arg( 1 << 8 );


BENCHMARK( benchmark_execute_nfa )->Arg( 1 << 0 );
BENCHMARK( benchmark_execute_nfa )->Arg( 1 << 1 );
BENCHMARK( benchmark_execute_nfa )->Arg( 1 << 2 );
BENCHMARK( benchmark_execute_nfa )->Arg( 1 << 3 );
BENCHMARK( benchmark_execute_nfa )->Arg( 1 << 4 );
//BENCHMARK( benchmark_execute_nfa )->Arg( 1 << 5 );
//BENCHMARK( benchmark_execute_nfa )->Arg( 1 << 6 );
//BENCHMARK( benchmark_execute_nfa )->Arg( 1 << 7 );
//BENCHMARK( benchmark_execute_nfa )->Arg( 1 << 8 );

BENCHMARK( benchmark_execute_dfa )->Arg( 1 << 0 );
BENCHMARK( benchmark_execute_dfa )->Arg( 1 << 1 );
BENCHMARK( benchmark_execute_dfa )->Arg( 1 << 2 );
BENCHMARK( benchmark_execute_dfa )->Arg( 1 << 3 );
BENCHMARK( benchmark_execute_dfa )->Arg( 1 << 4 );
BENCHMARK( benchmark_execute_dfa )->Arg( 1 << 5 );
BENCHMARK( benchmark_execute_dfa )->Arg( 1 << 6 );
BENCHMARK( benchmark_execute_dfa )->Arg( 1 << 7 );
BENCHMARK( benchmark_execute_dfa )->Arg( 1 << 8 );

