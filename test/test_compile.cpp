#include <gtest/gtest.h>

#include "compile.h"

TEST(compile_test, character)
{
    EXPECT_EQ(compile(std::stringstream("a"))->run("a"), regex::match::accepted);
}

TEST(compile_test, any)
{
    EXPECT_EQ(compile(std::stringstream("."))->run("a"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream(".."))->run("ab"), regex::match::accepted);
}

TEST(compile_test, alternation)
{
    EXPECT_EQ(compile(std::stringstream("a|b"))->run("a"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a|b"))->run("b"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a|b"))->run("c"), regex::match::rejected);
}

TEST(compile_test, question)
{
    EXPECT_EQ(compile(std::stringstream("a?"))->run(""), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a?"))->run("a"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a?"))->run("b"), regex::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a?"))->run("aa"), regex::match::rejected);
}

TEST(compile_test, kleene)
{
    EXPECT_EQ(compile(std::stringstream("a*"))->run(""), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a*"))->run("a"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a*"))->run("b"), regex::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a*"))->run("aa"), regex::match::accepted);
}

TEST(compile_test, one_or_more)
{
    EXPECT_EQ(compile(std::stringstream("a+"))->run(""), regex::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a+"))->run("a"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a+"))->run("b"), regex::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a+"))->run("aa"), regex::match::accepted);
}

TEST(compile_test, complex)
{
    EXPECT_EQ(compile(std::stringstream("a?.*(c*|d+)b*e"))->run("afffbbe"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a?.*(c*|d+)b*e"))->run("adbbe"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a?.*(c+|d+)b*e"))->run("afffbbe"), regex::match::rejected);
}