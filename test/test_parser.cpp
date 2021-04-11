#include <sstream>
#include <gtest/gtest.h>

#include "language/parser.h"

TEST( parse, character )
{
    std::string input( "a" );
    std::stringstream output;
    
    regex::language::parse( std::stringstream( input ) )->print( output );

    EXPECT_EQ( output.str( ), input );
}

TEST( parse, any )
{
    std::string input( "." );
    std::stringstream output;

    regex::language::parse( std::stringstream( input ) )->print( output );

    EXPECT_EQ( output.str( ), input );
}

TEST( parse, alternation )
{
    std::string input( "a|b" );
    std::stringstream output;

    regex::language::parse( std::stringstream( input ) )->print( output );

    EXPECT_EQ( output.str( ), input );
}

TEST( parse, question )
{
    std::string input( "a?" );
    std::stringstream output;

    regex::language::parse( std::stringstream( input ) )->print( output );

    EXPECT_EQ( output.str( ), input );
}

TEST( parse, kleene )
{
    std::string input( "a*" );
    std::stringstream output;

    regex::language::parse( std::stringstream( input ) )->print( output );

    EXPECT_EQ( output.str( ), input );
}

TEST( parse, one_or_more )
{
    std::string input( "a+" );
    std::stringstream output;

    regex::language::parse( std::stringstream( input ) )->print( output );

    EXPECT_EQ( output.str( ), input );
}

TEST( parse, parenthesis )
{
    std::string input( "(c)" );
    std::stringstream output;

    regex::language::parse( std::stringstream( input ) )->print( output );

    EXPECT_EQ( output.str( ), input );
}

TEST( parse, complex )
{
    std::string input( "a?.(c*|d+)b*e" );
    std::stringstream output;

    regex::language::parse( std::stringstream( input ) )->print( output );

    EXPECT_EQ( output.str( ), input );
}

TEST( parse, bad )
{
    EXPECT_THROW( regex::language::parse( std::stringstream( "?" ) ), regex::language::exception );
}