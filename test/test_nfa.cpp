#include "gtest/gtest.h"

#include "regex/automata/nfa.h"

TEST(nfa, character) {
    EXPECT_TRUE(regex::nfa::from_character('a')->execute("a"));
    EXPECT_FALSE(regex::nfa::from_character('a')->execute("b"));
}

TEST(nfa, epsilon) {
    EXPECT_FALSE(regex::nfa::from_epsilon()->execute("a"));
}

TEST(nfa, any) {
    EXPECT_TRUE(regex::nfa::from_any()->execute("a"));
    EXPECT_TRUE(regex::nfa::from_any()->execute("b"));
    EXPECT_TRUE(regex::nfa::from_any()->execute("c"));
}

TEST(nfa, concatenation) {
    EXPECT_TRUE(regex::nfa::from_concatenation(regex::nfa::from_character('a'),
                                               regex::nfa::from_character('b'))
                    ->execute("ab"));
    EXPECT_FALSE(regex::nfa::from_concatenation(regex::nfa::from_character('a'),
                                                regex::nfa::from_character('b'))
                     ->execute("ac"));
}

TEST(nfa, alternation) {
    EXPECT_TRUE(regex::nfa::from_alternation(
                    regex::nfa::from_concatenation(regex::nfa::from_character('a'),
                                                   regex::nfa::from_character('b')),
                    regex::nfa::from_concatenation(regex::nfa::from_character('a'),
                                                   regex::nfa::from_character('c')))
                    ->execute("ac"));
    EXPECT_FALSE(regex::nfa::from_alternation(
                     regex::nfa::from_concatenation(regex::nfa::from_character('a'),
                                                    regex::nfa::from_character('b')),
                     regex::nfa::from_concatenation(regex::nfa::from_character('a'),
                                                    regex::nfa::from_character('c')))
                     ->execute("a"));
}

TEST(nfa, kleene) {
    EXPECT_TRUE(regex::nfa::from_kleene(regex::nfa::from_character('a'))->execute(""));
    EXPECT_TRUE(regex::nfa::from_kleene(regex::nfa::from_character('a'))->execute("a"));
    EXPECT_TRUE(regex::nfa::from_kleene(regex::nfa::from_character('a'))->execute("aa"));
    EXPECT_FALSE(regex::nfa::from_kleene(regex::nfa::from_character('a'))->execute("b"));
    EXPECT_FALSE(regex::nfa::from_kleene(regex::nfa::from_character('a'))->execute("ab"));
    EXPECT_FALSE(regex::nfa::from_kleene(regex::nfa::from_character('a'))->execute("ba"));
}

TEST(nfa, complex) {
    auto state_machine = regex::nfa::from_alternation(
        regex::nfa::from_concatenation(
            regex::nfa::from_concatenation(
                regex::nfa::from_character('a'),
                regex::nfa::from_kleene(regex::nfa::from_character('b'))),
            regex::nfa::from_character('c')),
        regex::nfa::from_kleene(regex::nfa::from_character('d')));

    EXPECT_TRUE(state_machine->execute("abbc"));
    EXPECT_TRUE(state_machine->execute("ac"));
    EXPECT_TRUE(state_machine->execute(""));
    EXPECT_TRUE(state_machine->execute("dd"));
    EXPECT_FALSE(state_machine->execute("e"));
    EXPECT_FALSE(state_machine->execute("bc"));
}