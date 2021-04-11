#include <gtest/gtest.h>

#include "utilities/compile.h"
#include "utilities.h"

TEST( compile_nfa, character )
{
    EXPECT_EQ( regex::compile( std::stringstream( "a" ), regex::compile_flag::nfa )->execute( "a" ), regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a" ), regex::compile_flag::nfa )->execute( "b" ), regex::match::rejected );
}

TEST( compile_nfa, any )
{
    EXPECT_EQ( regex::compile( std::stringstream( "." ),  regex::compile_flag::nfa )->execute( "a" ),  regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( ".." ), regex::compile_flag::nfa )->execute( "ab" ), regex::match::accepted );
}

TEST( compile_nfa, alternation )
{
    EXPECT_EQ( regex::compile( std::stringstream( "a|b" ), regex::compile_flag::nfa )->execute( "a" ), regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a|b" ), regex::compile_flag::nfa )->execute( "b" ), regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a|b" ), regex::compile_flag::nfa )->execute( "c" ), regex::match::rejected );
}

TEST( compile_nfa, question )
{
    EXPECT_EQ( regex::compile( std::stringstream( "a?" ), regex::compile_flag::nfa )->execute( "" ),   regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a?" ), regex::compile_flag::nfa )->execute( "a" ),  regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a?" ), regex::compile_flag::nfa )->execute( "b" ),  regex::match::rejected );
    EXPECT_EQ( regex::compile( std::stringstream( "a?" ), regex::compile_flag::nfa )->execute( "aa" ), regex::match::rejected );
}

TEST( compile_nfa, kleene )
{
    EXPECT_EQ( regex::compile( std::stringstream( "a*" ), regex::compile_flag::nfa )->execute( "" ),   regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a*" ), regex::compile_flag::nfa )->execute( "a" ),  regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a*" ), regex::compile_flag::nfa )->execute( "b" ),  regex::match::rejected );
    EXPECT_EQ( regex::compile( std::stringstream( "a*" ), regex::compile_flag::nfa )->execute( "aa" ), regex::match::accepted );
}

TEST( compile_nfa, one_or_more )
{
    EXPECT_EQ( regex::compile( std::stringstream( "a+" ), regex::compile_flag::nfa )->execute( "" ),   regex::match::rejected );
    EXPECT_EQ( regex::compile( std::stringstream( "a+" ), regex::compile_flag::nfa )->execute( "a" ),  regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a+" ), regex::compile_flag::nfa )->execute( "b" ),  regex::match::rejected );
    EXPECT_EQ( regex::compile( std::stringstream( "a+" ), regex::compile_flag::nfa )->execute( "aa" ), regex::match::accepted );
}

TEST( compile_nfa, complex )
{
    EXPECT_EQ( regex::compile( std::stringstream( "a?.*(c*|d+)b*e" ), regex::compile_flag::nfa )->execute( "afffbbe" ), regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a?.*(c*|d+)b*e" ), regex::compile_flag::nfa )->execute( "adbbe" ),   regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a?.*(c+|d+)b*e" ), regex::compile_flag::nfa )->execute( "afffbbe" ), regex::match::rejected );
}

TEST( compile_dfa, character )
{
    EXPECT_EQ( regex::compile( std::stringstream( "a" ), regex::compile_flag::dfa )->execute( "a" ), regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a" ), regex::compile_flag::dfa )->execute( "b" ), regex::match::rejected );
}

TEST( compile_dfa, any )
{
    EXPECT_EQ( regex::compile( std::stringstream( "." ), regex::compile_flag::dfa )->execute( "a" ),   regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( ".." ), regex::compile_flag::dfa )->execute( "ab" ), regex::match::accepted );
}

TEST( compile_dfa, alternation )
{
    EXPECT_EQ( regex::compile( std::stringstream( "a|b" ), regex::compile_flag::dfa )->execute( "a" ), regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a|b" ), regex::compile_flag::dfa )->execute( "b" ), regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a|b" ), regex::compile_flag::dfa )->execute( "c" ), regex::match::rejected );
}

TEST( compile_dfa, question )
{
    EXPECT_EQ( regex::compile( std::stringstream( "a?" ), regex::compile_flag::dfa )->execute( "" ),   regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a?" ), regex::compile_flag::dfa )->execute( "a" ),  regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a?" ), regex::compile_flag::dfa )->execute( "b" ),  regex::match::rejected );
    EXPECT_EQ( regex::compile( std::stringstream( "a?" ), regex::compile_flag::dfa )->execute( "aa" ), regex::match::rejected );
}

TEST( compile_dfa, kleene )
{
    EXPECT_EQ( regex::compile( std::stringstream( "a*" ), regex::compile_flag::dfa )->execute( "" ),   regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a*" ), regex::compile_flag::dfa )->execute( "a" ),  regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a*" ), regex::compile_flag::dfa )->execute( "b" ),  regex::match::rejected );
    EXPECT_EQ( regex::compile( std::stringstream( "a*" ), regex::compile_flag::dfa )->execute( "aa" ), regex::match::accepted );
}

TEST( compile_dfa, one_or_more )
{
    EXPECT_EQ( regex::compile( std::stringstream( "a+" ), regex::compile_flag::dfa )->execute( "" ),   regex::match::rejected );
    EXPECT_EQ( regex::compile( std::stringstream( "a+" ), regex::compile_flag::dfa )->execute( "a" ),  regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a+" ), regex::compile_flag::dfa )->execute( "b" ),  regex::match::rejected );
    EXPECT_EQ( regex::compile( std::stringstream( "a+" ), regex::compile_flag::dfa )->execute( "aa" ), regex::match::accepted );
}

TEST( compile_dfa, complex )
{
    EXPECT_EQ( regex::compile( std::stringstream( "a?.*(c*|d+)b*e" ), regex::compile_flag::dfa )->execute( "afffbbe" ), regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a?.*(c*|d+)b*e" ), regex::compile_flag::dfa )->execute( "adbbe" ),   regex::match::accepted );
    EXPECT_EQ( regex::compile( std::stringstream( "a?.*(c+|d+)b*e" ), regex::compile_flag::dfa )->execute( "afffbbe" ), regex::match::rejected );
}