#include <gtest/gtest.h>

#include "compile.h"
#include "utilities.h"

TEST(compile_nfa, character)
{
    EXPECT_EQ(compile(std::stringstream("a"), compile_flag::nfa)->run("a"), regex::match::accepted);
}

TEST(compile_nfa, any)
{
    EXPECT_EQ(compile(std::stringstream("."), compile_flag::nfa)->run("a"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream(".."), compile_flag::nfa)->run("ab"), regex::match::accepted);
}

TEST(compile_nfa, alternation)
{
    EXPECT_EQ(compile(std::stringstream("a|b"), compile_flag::nfa)->run("a"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a|b"), compile_flag::nfa)->run("b"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a|b"), compile_flag::nfa)->run("c"), regex::match::rejected);
}

TEST(compile_nfa, question)
{
    EXPECT_EQ(compile(std::stringstream("a?"), compile_flag::nfa)->run(""), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a?"), compile_flag::nfa)->run("a"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a?"), compile_flag::nfa)->run("b"), regex::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a?"), compile_flag::nfa)->run("aa"), regex::match::rejected);
}

TEST(compile_nfa, kleene)
{
    EXPECT_EQ(compile(std::stringstream("a*"), compile_flag::nfa)->run(""), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a*"), compile_flag::nfa)->run("a"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a*"), compile_flag::nfa)->run("b"), regex::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a*"), compile_flag::nfa)->run("aa"), regex::match::accepted);
}

TEST(compile_nfa, one_or_more)
{
    EXPECT_EQ(compile(std::stringstream("a+"), compile_flag::nfa)->run(""), regex::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a+"), compile_flag::nfa)->run("a"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a+"), compile_flag::nfa)->run("b"), regex::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a+"), compile_flag::nfa)->run("aa"), regex::match::accepted);
}

TEST(compile_nfa, complex)
{
    EXPECT_EQ(compile(std::stringstream("a?.*(c*|d+)b*e"), compile_flag::nfa)->run("afffbbe"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a?.*(c*|d+)b*e"), compile_flag::nfa)->run("adbbe"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a?.*(c+|d+)b*e"), compile_flag::nfa)->run("afffbbe"), regex::match::rejected);
}

TEST(compile_dfa, character)
{
    EXPECT_EQ(compile(std::stringstream("a"), compile_flag::dfa)->run("a"), regex::match::accepted);
}

TEST(compile_dfa, any)
{
    // EXPECT_EQ(compile(std::stringstream("."), compile_flag::dfa)->run("a"), regex::match::accepted);
    // EXPECT_EQ(compile(std::stringstream(".."), compile_flag::dfa)->run("ab"), regex::match::accepted);
}

TEST(compile_dfa, alternation)
{
    EXPECT_EQ(compile(std::stringstream("a|b"), compile_flag::dfa)->run("a"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a|b"), compile_flag::dfa)->run("b"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a|b"), compile_flag::dfa)->run("c"), regex::match::rejected);
}

TEST(compile_dfa, question)
{
    EXPECT_EQ(compile(std::stringstream("a?"), compile_flag::dfa)->run(""), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a?"), compile_flag::dfa)->run("a"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a?"), compile_flag::dfa)->run("b"), regex::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a?"), compile_flag::dfa)->run("aa"), regex::match::rejected);
}

TEST(compile_dfa, kleene)
{
    EXPECT_EQ(compile(std::stringstream("a*"), compile_flag::dfa)->run(""), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a*"), compile_flag::dfa)->run("a"), regex::match::accepted);
    EXPECT_EQ(compile(std::stringstream("a*"), compile_flag::dfa)->run("b"), regex::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a*"), compile_flag::dfa)->run("aa"), regex::match::accepted);
}

TEST(compile_dfa, one_or_more)
{
    // EXPECT_EQ(compile(std::stringstream("a+"), compile_flag::dfa)->run(""), regex::match::rejected);
    EXPECT_EQ(compile(std::stringstream("a+"), compile_flag::dfa)->run("a"), regex::match::accepted);
    // EXPECT_EQ(compile(std::stringstream("a+"), compile_flag::dfa)->run("b"), regex::match::rejected);
    // EXPECT_EQ(compile(std::stringstream("a+"), compile_flag::dfa)->run("aa"), regex::match::accepted);
}

TEST(compile_dfa, complex)
{
    auto r = generate_table(compile(std::stringstream("a*a*"), compile_flag::nfa));
    std::cout << r.first << std::endl;
    // EXPECT_EQ(compile(std::stringstream("a*a*"), compile_flag::nfa)->run("afffbbe"), regex::match::accepted);
    // EXPECT_EQ(compile(std::stringstream("a?.*(c*|d+)b*e"), compile_flag::dfa)->run("adbbe"), regex::match::accepted);
    // EXPECT_EQ(compile(std::stringstream("a?.*(c+|d+)b*e"), compile_flag::dfa)->run("afffbbe"), regex::match::rejected);
}