// #include <sstream>
#include <gtest/gtest.h>

#include "nfa.h"
#include "dfa.h"

TEST(dfa, character)
{
    EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_character('a'))->run("a"),
              regex::match::accepted);

    EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_character('a'))->run("b"),
              regex::match::rejected);
}

TEST(dfa, any)
{
	EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_any())->run("a"),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_any())->run("b"),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_any())->run("c"),
			  regex::match::accepted);
}

TEST(dfa, concatenation)
{
	EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_concatenation(regex::nfa::from_character('a'), regex::nfa::from_character('b')))->run("ab"),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_concatenation(regex::nfa::from_character('a'), regex::nfa::from_character('b')))->run("ac"),
			  regex::match::rejected);
}

TEST(dfa, alternation)
{
	EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_alternation(regex::nfa::from_character('a'), regex::nfa::from_character('b')))->run("a"),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_alternation(regex::nfa::from_character('a'), regex::nfa::from_character('b')))->run("c"),
			  regex::match::rejected);
}

TEST(dfa, kleene)
{
	EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_kleene(regex::nfa::from_character('a')))->run(""),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_kleene(regex::nfa::from_character('a')))->run("a"),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_kleene(regex::nfa::from_character('a')))->run("aa"),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_kleene(regex::nfa::from_character('a')))->run("b"),
			  regex::match::rejected);

	EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_kleene(regex::nfa::from_character('a')))->run("ab"),
			  regex::match::rejected);

	EXPECT_EQ(regex::dfa::from_nfa(regex::nfa::from_kleene(regex::nfa::from_character('a')))->run("ba"),
			  regex::match::rejected);
}

TEST(dfa, complex)
{
	auto state_machine = regex::dfa::from_nfa(regex::nfa::from_alternation(
		regex::nfa::from_concatenation(
			regex::nfa::from_concatenation(
				regex::nfa::from_character('a'),
				regex::nfa::from_kleene(regex::nfa::from_character('b'))),
			regex::nfa::from_character('c')),
		regex::nfa::from_kleene(regex::nfa::from_character('d'))));

	EXPECT_EQ(state_machine->run("abbc"),
			  regex::match::accepted);

	EXPECT_EQ(state_machine->run("ac"),
			  regex::match::accepted);

	EXPECT_EQ(state_machine->run(""),
			  regex::match::accepted);

	EXPECT_EQ(state_machine->run("dd"),
			  regex::match::accepted);

	EXPECT_EQ(state_machine->run("e"),
			  regex::match::rejected);

	EXPECT_EQ(state_machine->run("bc"),
			  regex::match::rejected);
}