#include <sstream>
#include <gtest/gtest.h>

#include "language/parser.h"

TEST( parse, character )
{
    std::stringstream input( "a" );
    std::stringstream output;

    regex::language::parse( input )->print( output );

    EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse, any )
{
    std::stringstream input( "." );
    std::stringstream output;

    regex::language::parse( input )->print( output );

    EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse, alternation )
{
    std::stringstream input( "a|b" );
    std::stringstream output;

    regex::language::parse( input )->print( output );

    EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse, question )
{
    std::stringstream input( "a?" );
    std::stringstream output;

    regex::language::parse( input )->print( output );

    EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse, kleene )
{
    std::stringstream input( "a*" );
    std::stringstream output;

    regex::language::parse( input )->print( output );

    EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse, one_or_more )
{
    std::stringstream input( "a+" );
    std::stringstream output;

    regex::language::parse( input )->print( output );

    EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse, parenthesis )
{
    std::stringstream input( "(c)" );
    std::stringstream output;

    regex::language::parse( input )->print( output );

    EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse, complex )
{
    std::stringstream input( "a?.(c*|d+)b*e" );
    std::stringstream output;

    regex::language::parse( input )->print( output );

    EXPECT_EQ( output.str( ), std::string( "a?.(c*|d+)b*e" ) );
}

TEST( parse, bad )
{
    std::stringstream input( "?" );

    EXPECT_THROW( regex::language::parse( input ), regex::language::exception );
}