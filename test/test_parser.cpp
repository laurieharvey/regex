#include <gtest/gtest.h>
#include <sstream>

#include "regex/language/ast.h"

TEST( parse, character )
{
    std::string input( "a" );
    std::string output = regex::language::to_string( regex::language::parse<pool_allocator<regex::language::token>>( input ) );

    EXPECT_EQ( output, input );
}

TEST( parse, any )
{
    std::string input( "." );
    std::string output = regex::language::to_string( regex::language::parse<pool_allocator<regex::language::token>>( input ) );

    EXPECT_EQ( output, input );
}

TEST( parse, alternation )
{
    std::string input( "a|b" );
    std::string output = regex::language::to_string( regex::language::parse<pool_allocator<regex::language::token>>( input ) );

    EXPECT_EQ( output, input );
}

TEST( parse, question )
{
    std::string input( "a?" );
    std::string output = regex::language::to_string( regex::language::parse<pool_allocator<regex::language::token>>( input ) );

    EXPECT_EQ( output, input );
}

TEST( parse, kleene )
{
    std::string input( "a*" );
    std::string output = regex::language::to_string( regex::language::parse<pool_allocator<regex::language::token>>( input ) );

    EXPECT_EQ( output, input );
}

TEST( parse, one_or_more )
{
    std::string input( "a+" );
    std::string output = regex::language::to_string( regex::language::parse<pool_allocator<regex::language::token>>( input ) );

    EXPECT_EQ( output, input );
}

TEST( parse, parenthesis )
{
    std::string input( "(c)" );
    std::string output = regex::language::to_string( regex::language::parse<pool_allocator<regex::language::token>>( input ) );

    EXPECT_EQ( output, input );
}

TEST( parse, complex )
{
    std::string input( "a?.(c*|d+)b*e" );
    std::string output = regex::language::to_string( regex::language::parse<pool_allocator<regex::language::token>>( input ) );

    EXPECT_EQ( output, input );
}

TEST( parse, invalid )
{
    EXPECT_THROW( regex::language::parse<pool_allocator<regex::language::token>>( "" ),   std::runtime_error );
    EXPECT_THROW( regex::language::parse<pool_allocator<regex::language::token>>( "?" ),  std::runtime_error );
    EXPECT_THROW( regex::language::parse<pool_allocator<regex::language::token>>( "|" ),  std::runtime_error );
    EXPECT_THROW( regex::language::parse<pool_allocator<regex::language::token>>( "|b" ), std::runtime_error );
    EXPECT_THROW( regex::language::parse<pool_allocator<regex::language::token>>( "a|" ), std::runtime_error );
    EXPECT_THROW( regex::language::parse<pool_allocator<regex::language::token>>( "*" ),  std::runtime_error );
    EXPECT_THROW( regex::language::parse<pool_allocator<regex::language::token>>( "+" ),  std::runtime_error );
}