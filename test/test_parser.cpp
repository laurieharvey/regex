#include <sstream>
#include <gtest/gtest.h>

#include "language/parser.h"

TEST( make_explicit, character )
{
    std::stringstream implicit_regex( "a" );
    std::stringstream explicit_regex = regex::make_explicit( implicit_regex );

    EXPECT_EQ( explicit_regex.str( ), implicit_regex.str( ) );
}

TEST( make_explicit, any )
{
    std::stringstream implicit_regex( "." );
    std::stringstream explicit_regex = regex::make_explicit( implicit_regex );

    EXPECT_EQ( explicit_regex.str( ), implicit_regex.str( ) );
}

TEST( make_explicit, alternation )
{
    std::stringstream implicit_regex( "a|b" );
    std::stringstream explicit_regex = regex::make_explicit( implicit_regex );

    EXPECT_EQ( explicit_regex.str( ), implicit_regex.str( ) );
}

TEST( make_explicit, question )
{
    std::stringstream implicit_regex( "a?" );
    std::stringstream explicit_regex = regex::make_explicit( implicit_regex );

    EXPECT_EQ( explicit_regex.str( ), implicit_regex.str( ) );
}

TEST( make_explicit, kleene )
{
    std::stringstream implicit_regex( "a*" );
    std::stringstream explicit_regex = regex::make_explicit( implicit_regex );

    EXPECT_EQ( explicit_regex.str( ), implicit_regex.str( ) );
}

TEST( make_explicit, one_or_more )
{
    std::stringstream implicit_regex( "a+" );
    std::stringstream explicit_regex = regex::make_explicit( implicit_regex );

    EXPECT_EQ( explicit_regex.str( ), implicit_regex.str( ) );
}

TEST( make_explicit, parenthesis )
{
    std::stringstream implicit_regex( "(c)" );
    std::stringstream explicit_regex = regex::make_explicit( implicit_regex );

    EXPECT_EQ( explicit_regex.str( ), implicit_regex.str( ) );
}

TEST( make_explicit, complex )
{
    std::stringstream implicit_regex( "a?.(c*|d+)b*e" );
    std::stringstream explicit_regex = regex::make_explicit( implicit_regex );

    EXPECT_EQ( explicit_regex.str( ), std::string( "a?-.-(c*|d+)-b*-e" ) );
}

TEST( parse, character )
{
    std::stringstream input( "a" );
    std::stringstream output;

    regex::parse( input )->print( output );

    EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse, any )
{
    std::stringstream input( "." );
    std::stringstream output;

    regex::parse( input )->print( output );

    EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse, alternation )
{
    std::stringstream input( "a|b" );
    std::stringstream output;

    regex::parse( input )->print( output );

    EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse, question )
{
    std::stringstream input( "a?" );
    std::stringstream output;

    regex::parse( input )->print( output );

    EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse, kleene )
{
    std::stringstream input( "a*" );
    std::stringstream output;

    regex::parse( input )->print( output );

    EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse, one_or_more )
{
    std::stringstream input( "a+" );
    std::stringstream output;

    regex::parse( input )->print( output );

    EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse, parenthesis )
{
    std::stringstream input( "(c)" );
    std::stringstream output;

    regex::parse( input )->print( output );

    EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse, complex )
{
    std::stringstream input( "a?-.-(c*|d+)-b*-e" );
    std::stringstream output;

    regex::parse( input )->print( output );

    EXPECT_EQ( output.str( ), std::string( "a?.(c*|d+)b*e" ) );
}