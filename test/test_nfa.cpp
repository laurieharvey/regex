#include <sstream>
#include <gtest/gtest.h>

#include "nfa.h"

TEST(nfa_test, character)
{
	EXPECT_EQ(fa::nfa::from_character('a')->run("a"),
			  fa::match::accepted);

	EXPECT_EQ(fa::nfa::from_character('a')->run("b"),
			  fa::match::rejected);
}

TEST(nfa_test, epsilon)
{
	EXPECT_EQ(fa::nfa::from_epsilon()->run("a"),
			  fa::match::rejected);
}

TEST(nfa_test, concatenation)
{
	EXPECT_EQ(fa::nfa::from_concatenation(fa::nfa::from_character('a'), fa::nfa::from_character('b'))->run("ab"),
			  fa::match::accepted);

	EXPECT_EQ(fa::nfa::from_concatenation(fa::nfa::from_character('a'), fa::nfa::from_character('b'))->run("ac"),
			  fa::match::rejected);
}

TEST(nfa_test, alternation)
{
	EXPECT_EQ(fa::nfa::from_alternation(fa::nfa::from_character('a'), fa::nfa::from_character('b'))->run("a"),
			  fa::match::accepted);

	EXPECT_EQ(fa::nfa::from_alternation(fa::nfa::from_character('a'), fa::nfa::from_character('b'))->run("c"),
			  fa::match::rejected);
}

TEST(nfa_test, kleene)
{
	EXPECT_EQ(fa::nfa::from_kleene(fa::nfa::from_character('a'))->run(""),
			  fa::match::accepted);

	EXPECT_EQ(fa::nfa::from_kleene(fa::nfa::from_character('a'))->run("a"),
			  fa::match::accepted);

	EXPECT_EQ(fa::nfa::from_kleene(fa::nfa::from_character('a'))->run("aa"),
			  fa::match::accepted);

	EXPECT_EQ(fa::nfa::from_kleene(fa::nfa::from_character('a'))->run("b"),
			  fa::match::rejected);

	EXPECT_EQ(fa::nfa::from_kleene(fa::nfa::from_character('a'))->run("ab"),
			  fa::match::rejected);

	EXPECT_EQ(fa::nfa::from_kleene(fa::nfa::from_character('a'))->run("ba"),
			  fa::match::rejected);
}

TEST(nfa_test, complex)
{
	auto state_machine = fa::nfa::from_alternation(
		fa::nfa::from_concatenation(
			fa::nfa::from_concatenation(
				fa::nfa::from_character('a'),
				fa::nfa::from_kleene(fa::nfa::from_character('b'))),
			fa::nfa::from_character('c')),
		fa::nfa::from_kleene(fa::nfa::from_character('d')));

	EXPECT_EQ(state_machine->run("abbc"),
			  fa::match::accepted);

	EXPECT_EQ(state_machine->run("ac"),
			  fa::match::accepted);

	EXPECT_EQ(state_machine->run(""),
			  fa::match::accepted);

	EXPECT_EQ(state_machine->run("dd"),
			  fa::match::accepted);

	EXPECT_EQ(state_machine->run("e"),
			  fa::match::rejected);

	EXPECT_EQ(state_machine->run("bc"),
			  fa::match::rejected);
}