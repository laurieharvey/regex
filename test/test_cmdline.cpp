#include <gtest/gtest.h>

#include <array>

#include "cmdline.h"

TEST( cmdline, good )
{
    regex::cmd::cmdline args("description");

    args.add_optional( "-t", "type", regex::cmd::cmdline::type::string, "nfa", "Type of finite automata" );
    args.add_optional( "-l", "limit", regex::cmd::cmdline::type::integral, 1, "Limit" );
    args.add_flag( "-v", "verbose", "Verbose logging" );
    args.add_flag( "-o", "optimise", "Optimise" );
    args.add_positional( "regex", regex::cmd::cmdline::type::string, "Regular expression" );
    args.add_positional( "target", regex::cmd::cmdline::type::string, "Target to match" );

    const char* cmdline[]{ "regex", "-t", "nfa", "-v", "a+b*cc", "acc" };

    args.parse( 6, cmdline );

    EXPECT_EQ( args.get_argument<std::string>( "type" ), "nfa" );
    EXPECT_EQ( args.get_argument<int>( "limit" ), 1 );
    EXPECT_EQ( args.get_argument<bool>( "verbose" ), true );
    EXPECT_EQ( args.get_argument<bool>( "optimise" ), false );
    EXPECT_EQ( args.get_argument<std::string>( "regex" ), "a+b*cc" );
    EXPECT_EQ( args.get_argument<std::string>( "target" ), "acc" );
}

TEST( cmdline, too_few_positionals )
{
    regex::cmd::cmdline args("description");

    args.add_optional( "-t", "type", regex::cmd::cmdline::type::string, "nfa", "Type of finite automata" );
    args.add_optional( "-l", "limit", regex::cmd::cmdline::type::integral, 1, "Limit" );
    args.add_flag( "-v", "verbose", "Verbose logging" );
    args.add_flag( "-o", "optimise", "Optimise" );
    args.add_positional( "regex", regex::cmd::cmdline::type::string, "Regular expression" );
    args.add_positional( "target", regex::cmd::cmdline::type::string, "Target to match" );

    const char* cmdline[]{ "regex", "-t", "nfa", "-v", "a+b*cc" };

    EXPECT_THROW( args.parse( 5, cmdline ), regex::cmd::exception );
}

TEST( cmdline, too_many_positionals )
{
    regex::cmd::cmdline args("description");

    args.add_optional( "-t", "type", regex::cmd::cmdline::type::string, "nfa", "Type of finite automata" );
    args.add_optional( "-l", "limit", regex::cmd::cmdline::type::integral, 1, "Limit" );
    args.add_flag( "-v", "verbose", "Verbose logging" );
    args.add_flag( "-o", "optimise", "Optimise" );
    args.add_positional( "regex", regex::cmd::cmdline::type::string, "Regular expression" );
    args.add_positional( "target", regex::cmd::cmdline::type::string, "Target to match" );

    const char* cmdline[]{ "regex", "-t", "nfa", "-v", "a+b*cc", "acc", "acc" };

    EXPECT_THROW( args.parse( 5, cmdline ), regex::cmd::exception );
}
