#include <gtest/gtest.h>

#include "regex/automata/dfa.h"
#include "regex/automata/nfa.h"

TEST(dfa, character) {
    EXPECT_TRUE(regex::nfa::to_dfa(regex::nfa::from_character('a'))->execute("a"));
    EXPECT_FALSE(regex::nfa::to_dfa(regex::nfa::from_character('a'))->execute("b"));
}

TEST(dfa, epsilon) {
    EXPECT_FALSE(regex::nfa::to_dfa(regex::nfa::from_epsilon())->execute("a"));
}

TEST(dfa, any) {
    EXPECT_TRUE(regex::nfa::to_dfa(regex::nfa::from_any())->execute("a"));
    EXPECT_TRUE(regex::nfa::to_dfa(regex::nfa::from_any())->execute("b"));
    EXPECT_TRUE(regex::nfa::to_dfa(regex::nfa::from_any())->execute("c"));
}

TEST(dfa, concatenation) {
    EXPECT_TRUE(regex::nfa::to_dfa(regex::nfa::from_concatenation(regex::nfa::from_character('a'),
                                                                  regex::nfa::from_character('b')))
                    ->execute("ab"));
    EXPECT_FALSE(regex::nfa::to_dfa(regex::nfa::from_concatenation(regex::nfa::from_character('a'),
                                                                   regex::nfa::from_character('b')))
                     ->execute("ac"));
}

TEST(dfa, alternation) {
    EXPECT_TRUE(
        regex::nfa::to_dfa(regex::nfa::from_alternation(
                               regex::nfa::from_concatenation(regex::nfa::from_character('a'),
                                                              regex::nfa::from_character('b')),
                               regex::nfa::from_concatenation(regex::nfa::from_character('a'),
                                                              regex::nfa::from_character('c'))))
            ->execute("ac"));
    EXPECT_FALSE(
        regex::nfa::to_dfa(regex::nfa::from_alternation(
                               regex::nfa::from_concatenation(regex::nfa::from_character('a'),
                                                              regex::nfa::from_character('b')),
                               regex::nfa::from_concatenation(regex::nfa::from_character('a'),
                                                              regex::nfa::from_character('c'))))
            ->execute("a"));
}

TEST(dfa, kleene) {
    EXPECT_TRUE(
        regex::nfa::to_dfa(regex::nfa::from_kleene(regex::nfa::from_character('a')))->execute(""));
    EXPECT_TRUE(
        regex::nfa::to_dfa(regex::nfa::from_kleene(regex::nfa::from_character('a')))->execute("a"));
    EXPECT_TRUE(regex::nfa::to_dfa(regex::nfa::from_kleene(regex::nfa::from_character('a')))
                    ->execute("aa"));
    EXPECT_FALSE(
        regex::nfa::to_dfa(regex::nfa::from_kleene(regex::nfa::from_character('a')))->execute("b"));
    EXPECT_FALSE(regex::nfa::to_dfa(regex::nfa::from_kleene(regex::nfa::from_character('a')))
                     ->execute("ab"));
    EXPECT_FALSE(regex::nfa::to_dfa(regex::nfa::from_kleene(regex::nfa::from_character('a')))
                     ->execute("ba"));
}

TEST(dfa, complex) {
    auto state_machine = regex::nfa::to_dfa(regex::nfa::from_alternation(
        regex::nfa::from_concatenation(
            regex::nfa::from_concatenation(
                regex::nfa::from_character('a'),
                regex::nfa::from_kleene(regex::nfa::from_character('b'))),
            regex::nfa::from_character('c')),
        regex::nfa::from_kleene(regex::nfa::from_character('d'))));

    EXPECT_TRUE(state_machine->execute("abbc"));
    EXPECT_TRUE(state_machine->execute("ac"));
    EXPECT_TRUE(state_machine->execute(""));
    EXPECT_TRUE(state_machine->execute("dd"));
    EXPECT_FALSE(state_machine->execute("e"));
    EXPECT_FALSE(state_machine->execute("bc"));
}