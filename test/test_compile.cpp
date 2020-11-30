#include <gtest/gtest.h>

#include "compile.h"

TEST(compile_test, character)
{
    EXPECT_EQ(compile(std::stringstream("a"))->run("a"), fa::match::accepted);
}

TEST(compile_test, any)
{
    EXPECT_EQ(compile(std::stringstream("."))->run("a"), fa::match::accepted);
    EXPECT_EQ(compile(std::stringstream(".."))->run("ab"), fa::match::accepted);
}

TEST(compile_test, alternation)
{
    EXPECT_EQ(compile(std::stringstream("a|b"))->run("a"), fa::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a|b"))->run("b"), fa::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a|b"))->run("c"), fa::match::rejected);
}

TEST(compile_test, question)
{
    EXPECT_EQ(compile(std::stringstream("a?"))->run(""), fa::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a?"))->run("a"), fa::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a?"))->run("b"), fa::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a?"))->run("aa"), fa::match::rejected);
}

TEST(compile_test, kleene)
{
    EXPECT_EQ(compile(std::stringstream("a*"))->run(""), fa::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a*"))->run("a"), fa::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a*"))->run("b"), fa::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a*"))->run("aa"), fa::match::accepted);
}

TEST(compile_test, one_or_more)
{
    EXPECT_EQ(compile(std::stringstream("a+"))->run(""), fa::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a+"))->run("a"), fa::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a+"))->run("b"), fa::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a+"))->run("aa"), fa::match::accepted);
}

TEST(compile_test, complex)
{
    EXPECT_EQ(compile(std::stringstream("a?.*(c*|d+)b*e"))->run("afffbbe"), fa::match::accepted);
}