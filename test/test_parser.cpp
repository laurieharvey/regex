#include <sstream>
#include <gtest/gtest.h>

#include "parser.h"

TEST( make_explicit_test, alternation )
{
	std::stringstream implicit_regex( "a|b" );
	std::stringstream explicit_regex = ast::make_explicit( implicit_regex );

	EXPECT_EQ( explicit_regex.str(), std::string( "a|b" ) );
}

TEST( make_explicit_test, question )
{
	std::stringstream implicit_regex( "a?b" );
	std::stringstream explicit_regex = ast::make_explicit( implicit_regex );

	EXPECT_EQ( explicit_regex.str( ), std::string( "a?.b" ) );
}

TEST( make_explicit_test, kleene )
{
	std::stringstream implicit_regex( "a*b" );
	std::stringstream explicit_regex = ast::make_explicit( implicit_regex );

	EXPECT_EQ( explicit_regex.str( ), std::string( "a*.b" ) );
}

TEST( make_explicit_test, parenthesis )
{
	std::stringstream implicit_regex( "a(c)b" );
	std::stringstream explicit_regex = ast::make_explicit( implicit_regex );

	EXPECT_EQ( explicit_regex.str( ), std::string( "a.(c).b" ) );
}

TEST( make_explicit_test, complex )
{
	std::stringstream implicit_regex( "a?(c*|d)b*e" );
	std::stringstream explicit_regex = ast::make_explicit( implicit_regex );

	EXPECT_EQ( explicit_regex.str( ), std::string( "a?.(c*|d).b*.e" ) );
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
	std::stringstream input( "a?.b" );
	std::stringstream output;

	ast::parse( input )->print( output );

	EXPECT_EQ( output.str( ), std::string( "a?b" ) );
}

TEST( parse_test, kleene )
{
	std::stringstream input( "a*.b" );
	std::stringstream output;

	ast::parse( input )->print( output );

	EXPECT_EQ( output.str( ), std::string( "a*b" ) );
}

TEST( parse_test, parenthesis )
{
	std::stringstream input( "a.(c).b" );
	std::stringstream output;

	ast::parse( input )->print( output );

	EXPECT_EQ( output.str( ), std::string( "a(c)b" ) );
}

TEST( parse_test, complex )
{
	std::stringstream input( "a?.(c*|d).b*.e" );
	std::stringstream output;

	ast::parse( input )->print( output );

	EXPECT_EQ( output.str( ), std::string( "a?(c*|d)b*e" ) );
}