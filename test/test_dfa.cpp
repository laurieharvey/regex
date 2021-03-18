#include <gtest/gtest.h>
#include "automata/dfa.h"

TEST(dfa, character)
{
	EXPECT_EQ(regex::dfa::from_character('a')->run("a"),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_character('a')->run("b"),
			  regex::match::rejected);
}

TEST(dfa, any)
{

}

TEST(dfa, concatenation)
{
	EXPECT_EQ(regex::dfa::from_concatenation(regex::dfa::from_character('a'), regex::dfa::from_character('b'))->run("ab"),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_concatenation(regex::dfa::from_character('a'), regex::dfa::from_character('b'))->run("ac"),
			  regex::match::rejected);
}

TEST(dfa, alternation)
{
	EXPECT_EQ(regex::dfa::from_alternation(
				  regex::dfa::from_concatenation(regex::dfa::from_character('a'), regex::dfa::from_character('b')),
				  regex::dfa::from_concatenation(regex::dfa::from_character('a'), regex::dfa::from_character('c')))
				  ->run("ac"),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_alternation(
				  regex::dfa::from_concatenation(regex::dfa::from_character('a'), regex::dfa::from_character('b')),
				  regex::dfa::from_concatenation(regex::dfa::from_character('a'), regex::dfa::from_character('c')))
				  ->run("a"),
			  regex::match::rejected);
}

TEST(dfa, kleene)
{
	EXPECT_EQ(regex::dfa::from_kleene(regex::dfa::from_character('a'))->run(""),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_kleene(regex::dfa::from_character('a'))->run("a"),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_kleene(regex::dfa::from_character('a'))->run("aa"),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_kleene(regex::dfa::from_character('a'))->run("b"),
			  regex::match::rejected);

	EXPECT_EQ(regex::dfa::from_kleene(regex::dfa::from_character('a'))->run("ab"),
			  regex::match::rejected);

	EXPECT_EQ(regex::dfa::from_kleene(regex::dfa::from_character('a'))->run("ba"),
			  regex::match::rejected);
}

TEST(dfa, zero_or_one)
{
	EXPECT_EQ(regex::dfa::from_zero_or_one(regex::dfa::from_character('a'))->run(""),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_zero_or_one(regex::dfa::from_character('a'))->run("a"),
			  regex::match::accepted);

	EXPECT_EQ(regex::dfa::from_zero_or_one(regex::dfa::from_character('a'))->run("aa"),
			  regex::match::rejected);

	EXPECT_EQ(regex::dfa::from_zero_or_one(regex::dfa::from_character('a'))->run("b"),
			  regex::match::rejected);

	EXPECT_EQ(regex::dfa::from_zero_or_one(regex::dfa::from_character('a'))->run("ab"),
			  regex::match::rejected);

	EXPECT_EQ(regex::dfa::from_zero_or_one(regex::dfa::from_character('a'))->run("ba"),
			  regex::match::rejected);
}

TEST(dfa, complex)
{
	auto state_machine = regex::dfa::from_alternation(
		regex::dfa::from_concatenation(
			regex::dfa::from_concatenation(
				regex::dfa::from_character('a'),
				regex::dfa::from_kleene(regex::dfa::from_character('b'))),
			regex::dfa::from_character('c')),
		regex::dfa::from_kleene(regex::dfa::from_character('d')));

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