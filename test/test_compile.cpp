#include <gtest/gtest.h>

#include "regex/utilities/compile.h"

TEST( compile_nfa, character )
{
    EXPECT_TRUE( regex::compile( "a", regex::compile_flag::nfa )->execute( "a" ) );
    EXPECT_FALSE( regex::compile( "a", regex::compile_flag::nfa )->execute( "b" ) );
}

TEST( compile_nfa, any )
{
    EXPECT_TRUE( regex::compile( ".", regex::compile_flag::nfa )->execute( "a" ) );
    EXPECT_TRUE( regex::compile( "..", regex::compile_flag::nfa )->execute( "ab" ) );
}

TEST( compile_nfa, alternation )
{
    EXPECT_TRUE( regex::compile( "a|b", regex::compile_flag::nfa )->execute( "a" ) );
    EXPECT_TRUE( regex::compile( "a|b", regex::compile_flag::nfa )->execute( "b" ) );
    EXPECT_FALSE( regex::compile( "a|b", regex::compile_flag::nfa )->execute( "c" ) );
}

TEST( compile_nfa, question )
{
    EXPECT_TRUE( regex::compile( "a?", regex::compile_flag::nfa )->execute( "" ) );
    EXPECT_TRUE( regex::compile( "a?", regex::compile_flag::nfa )->execute( "a" ) );
    EXPECT_FALSE( regex::compile( "a?", regex::compile_flag::nfa )->execute( "b" ) );
    EXPECT_FALSE( regex::compile( "a?", regex::compile_flag::nfa )->execute( "aa" ) );
}

TEST( compile_nfa, kleene )
{
    EXPECT_TRUE( regex::compile( "a*", regex::compile_flag::nfa )->execute( "" ) );
    EXPECT_TRUE( regex::compile( "a*", regex::compile_flag::nfa )->execute( "a" ) );
    EXPECT_FALSE( regex::compile( "a*", regex::compile_flag::nfa )->execute( "b" ) );
    EXPECT_TRUE( regex::compile( "a*", regex::compile_flag::nfa )->execute( "aa" ) );
}

TEST( compile_nfa, one_or_more )
{
    EXPECT_FALSE( regex::compile( "a+", regex::compile_flag::nfa )->execute( "" ) );
    EXPECT_TRUE( regex::compile( "a+", regex::compile_flag::nfa )->execute( "a" ) );
    EXPECT_FALSE( regex::compile( "a+", regex::compile_flag::nfa )->execute( "b" ) );
    EXPECT_TRUE( regex::compile( "a+", regex::compile_flag::nfa )->execute( "aa" ) );
}

TEST( compile_nfa, complex )
{
    EXPECT_TRUE( regex::compile( "a?.*(c*|d+)b*e", regex::compile_flag::nfa )->execute( "afffbbe" ) );
    EXPECT_TRUE( regex::compile( "a?.*(c*|d+)b*e", regex::compile_flag::nfa )->execute( "adbbe" ) );
    EXPECT_FALSE( regex::compile( "a?.*(c+|d+)b*e", regex::compile_flag::nfa )->execute( "afffbbe" ) );
}

TEST( compile_dfa, character )
{
    EXPECT_TRUE( regex::compile( "a", regex::compile_flag::dfa )->execute( "a" ) );
    EXPECT_FALSE( regex::compile( "a", regex::compile_flag::dfa )->execute( "b" ) );
}

TEST( compile_dfa, any )
{
    EXPECT_TRUE( regex::compile( ".", regex::compile_flag::dfa )->execute( "a" ) );
    EXPECT_TRUE( regex::compile( "..", regex::compile_flag::dfa )->execute( "ab" ) );
}

TEST( compile_dfa, alternation )
{
    EXPECT_TRUE( regex::compile( "a|b", regex::compile_flag::dfa )->execute( "a" ) );
    EXPECT_TRUE( regex::compile( "a|b", regex::compile_flag::dfa )->execute( "b" ) );
    EXPECT_FALSE( regex::compile( "a|b", regex::compile_flag::dfa )->execute( "c" ) );
}

TEST( compile_dfa, question )
{
    EXPECT_TRUE( regex::compile( "a?", regex::compile_flag::dfa )->execute( "" ) );
    EXPECT_TRUE( regex::compile( "a?", regex::compile_flag::dfa )->execute( "a" ) );
    EXPECT_FALSE( regex::compile( "a?", regex::compile_flag::dfa )->execute( "b" ) );
    EXPECT_FALSE( regex::compile( "a?", regex::compile_flag::dfa )->execute( "aa" ) );
}

TEST( compile_dfa, kleene )
{
    EXPECT_TRUE( regex::compile( "a*", regex::compile_flag::dfa )->execute( "a" ) );
    EXPECT_FALSE( regex::compile( "a*", regex::compile_flag::dfa )->execute( "b" ) );
    EXPECT_TRUE( regex::compile( "a*", regex::compile_flag::dfa )->execute( "aa" ) );
}

TEST( compile_dfa, one_or_more )
{
    EXPECT_FALSE( regex::compile( "a+", regex::compile_flag::dfa )->execute( "" ) );
    EXPECT_TRUE( regex::compile( "a+", regex::compile_flag::dfa )->execute( "a" ) );
    EXPECT_FALSE( regex::compile( "a+", regex::compile_flag::dfa )->execute( "b" ) );
    EXPECT_TRUE( regex::compile( "a+", regex::compile_flag::dfa )->execute( "aa" ) );
}

TEST( compile_dfa, complex )
{
    EXPECT_TRUE( regex::compile( "e*e", regex::compile_flag::dfa )->execute( "eeee" ) );
    EXPECT_TRUE( regex::compile( "a?.*(c*|d+)b*e", regex::compile_flag::dfa )->execute( "adbbe" ) );
    EXPECT_FALSE( regex::compile( "a?.*(c+|d+)b*e", regex::compile_flag::dfa )->execute( "afffbbe" ) );
}
