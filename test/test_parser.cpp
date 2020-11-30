#include <sstream>
#include <gtest/gtest.h>

#include "parser.h"

TEST( make_explicit_test, character )
{
	std::stringstream implicit_regex( "a" );
	std::stringstream explicit_regex = ast::make_explicit( implicit_regex );

	EXPECT_EQ( explicit_regex.str(), implicit_regex.str() );
}

TEST( make_explicit_test, any )
{
	std::stringstream implicit_regex( "." );
	std::stringstream explicit_regex = ast::make_explicit( implicit_regex );

	EXPECT_EQ( explicit_regex.str(), implicit_regex.str() );
}

TEST( make_explicit_test, alternation )
{
	std::stringstream implicit_regex( "a|b" );
	std::stringstream explicit_regex = ast::make_explicit( implicit_regex );

	EXPECT_EQ( explicit_regex.str(), implicit_regex.str() );
}

TEST( make_explicit_test, question )
{
	std::stringstream implicit_regex( "a?" );
	std::stringstream explicit_regex = ast::make_explicit( implicit_regex );

	EXPECT_EQ( explicit_regex.str( ), implicit_regex.str() );
}

TEST( make_explicit_test, kleene )
{
	std::stringstream implicit_regex( "a*" );
	std::stringstream explicit_regex = ast::make_explicit( implicit_regex );

	EXPECT_EQ( explicit_regex.str( ), implicit_regex.str() );
}

TEST( make_explicit_test, one_or_more )
{
	std::stringstream implicit_regex( "a+" );
	std::stringstream explicit_regex = ast::make_explicit( implicit_regex );

	EXPECT_EQ( explicit_regex.str( ), implicit_regex.str() );
}

TEST( make_explicit_test, parenthesis )
{
	std::stringstream implicit_regex( "(c)" );
	std::stringstream explicit_regex = ast::make_explicit( implicit_regex );

	EXPECT_EQ( explicit_regex.str( ), implicit_regex.str() );
}

TEST( make_explicit_test, complex )
{
	std::stringstream implicit_regex( "a?.(c*|d+)b*e" );
	std::stringstream explicit_regex = ast::make_explicit( implicit_regex );

	EXPECT_EQ( explicit_regex.str( ), std::string( "a?-.-(c*|d+)-b*-e" ) );
}

TEST( parse_test, character )
{
	std::stringstream input( "a" );
	std::stringstream output;

	ast::parse( input )->print( output );

	EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse_test, any )
{
	std::stringstream input( "." );
	std::stringstream output;

	ast::parse( input )->print( output );

	EXPECT_EQ( output.str( ), input.str() );
}

TEST( parse_test, alternation )
{
	std::stringstream input( "a|b" );
	std::stringstream output;

	ast::parse( input )->print( output );

	EXPECT_EQ( output.str( ), input.str( ) );
}

TEST( parse_test, question )
{
	std::stringstream input( "a?" );
	std::stringstream output;

	ast::parse( input )->print( output );

	EXPECT_EQ( output.str( ), input.str() );
}

TEST( parse_test, kleene )
{
	std::stringstream input( "a*" );
	std::stringstream output;

	ast::parse( input )->print( output );

	EXPECT_EQ( output.str( ), input.str() );
}

TEST( parse_test, one_or_more )
{
	std::stringstream input( "a+" );
	std::stringstream output;

	ast::parse( input )->print( output );

	EXPECT_EQ( output.str( ), input.str() );
}

TEST( parse_test, parenthesis )
{
	std::stringstream input( "(c)" );
	std::stringstream output;

	ast::parse( input )->print( output );

	EXPECT_EQ( output.str( ), input.str() );
}

TEST( parse_test, complex )
{
	std::stringstream input( "a?-.-(c*|d+)-b*-e" );
	std::stringstream output;

	ast::parse( input )->print( output );

	EXPECT_EQ( output.str( ), std::string( "a?.(c*|d+)b*e" ) );
}