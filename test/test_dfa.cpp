#include "gtest/gtest.h"

#include "regex/automata/dfa.h"
#include "regex/automata/nfa.h"

TEST( dfa, character )
{
    EXPECT_TRUE( regex::nfa::from_character( 'a' )->to_dfa()->execute( "a" ) );
    EXPECT_FALSE( regex::nfa::from_character( 'a' )->to_dfa()->execute( "b" ) );
}

TEST( dfa, epsilon )
{
    EXPECT_FALSE( regex::nfa::from_epsilon()->to_dfa()->execute( "a" ) );
}

TEST( dfa, any )
{
    EXPECT_TRUE( regex::nfa::from_any()->to_dfa()->execute( "a" ) );
    EXPECT_TRUE( regex::nfa::from_any()->to_dfa()->execute( "b" ) );
    EXPECT_TRUE( regex::nfa::from_any()->to_dfa()->execute( "c" ) );
}

TEST( dfa, concatenation )
{
    EXPECT_TRUE( regex::nfa::from_concatenation( regex::nfa::from_character( 'a' ), regex::nfa::from_character( 'b' ) )
                     ->to_dfa()
                     ->execute( "ab" ) );
    EXPECT_FALSE( regex::nfa::from_concatenation( regex::nfa::from_character( 'a' ), regex::nfa::from_character( 'b' ) )
                      ->to_dfa()
                      ->execute( "ac" ) );
}

TEST( dfa, alternation )
{
    EXPECT_TRUE(
        regex::nfa::from_alternation(
            regex::nfa::from_concatenation( regex::nfa::from_character( 'a' ), regex::nfa::from_character( 'b' ) ),
            regex::nfa::from_concatenation( regex::nfa::from_character( 'a' ), regex::nfa::from_character( 'c' ) ) )
            ->to_dfa()
            ->execute( "ac" ) );
    EXPECT_FALSE(
        regex::nfa::from_alternation(
            regex::nfa::from_concatenation( regex::nfa::from_character( 'a' ), regex::nfa::from_character( 'b' ) ),
            regex::nfa::from_concatenation( regex::nfa::from_character( 'a' ), regex::nfa::from_character( 'c' ) ) )
            ->to_dfa()
            ->execute( "a" ) );
}

TEST( dfa, kleene )
{
    EXPECT_TRUE( regex::nfa::from_kleene( regex::nfa::from_character( 'a' ) )->to_dfa()->execute( "" ) );
    EXPECT_TRUE( regex::nfa::from_kleene( regex::nfa::from_character( 'a' ) )->to_dfa()->execute( "a" ) );
    EXPECT_TRUE( regex::nfa::from_kleene( regex::nfa::from_character( 'a' ) )->to_dfa()->execute( "aa" ) );
    EXPECT_FALSE( regex::nfa::from_kleene( regex::nfa::from_character( 'a' ) )->to_dfa()->execute( "b" ) );
    EXPECT_FALSE( regex::nfa::from_kleene( regex::nfa::from_character( 'a' ) )->to_dfa()->execute( "ab" ) );
    EXPECT_FALSE( regex::nfa::from_kleene( regex::nfa::from_character( 'a' ) )->to_dfa()->execute( "ba" ) );
}

TEST( dfa, complex )
{
    std::unique_ptr<regex::dfa> state_machine =
        regex::nfa::from_alternation(
            regex::nfa::from_concatenation(
                regex::nfa::from_concatenation( regex::nfa::from_character( 'a' ),
                                                regex::nfa::from_kleene( regex::nfa::from_character( 'b' ) ) ),
                regex::nfa::from_character( 'c' ) ),
            regex::nfa::from_kleene( regex::nfa::from_character( 'd' ) ) )
            ->to_dfa();

    EXPECT_TRUE( state_machine->execute( "abbc" ) );
    EXPECT_TRUE( state_machine->execute( "ac" ) );
    EXPECT_TRUE( state_machine->execute( "" ) );
    EXPECT_TRUE( state_machine->execute( "dd" ) );

    EXPECT_FALSE( state_machine->execute( "e" ) );
    EXPECT_FALSE( state_machine->execute( "bc" ) );
}