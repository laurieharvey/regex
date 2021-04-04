#include <gtest/gtest.h>

#include "automata/nfa.h"

TEST( nfa, character )
{
    EXPECT_EQ( regex::nfa::from_character( 'a' )->execute( "a" ), regex::match::accepted );
    EXPECT_EQ( regex::nfa::from_character( 'a' )->execute( "b" ), regex::match::rejected );
}

TEST( nfa, epsilon )
{
    EXPECT_EQ( regex::nfa::from_epsilon( )->execute( "a" ), regex::match::rejected );
}

TEST( nfa, any )
{
    EXPECT_EQ( regex::nfa::from_any( )->execute( "a" ), regex::match::accepted );
    EXPECT_EQ( regex::nfa::from_any( )->execute( "b" ), regex::match::accepted );
    EXPECT_EQ( regex::nfa::from_any( )->execute( "c" ), regex::match::accepted );
}

TEST( nfa, concatenation )
{
    EXPECT_EQ( regex::nfa::from_concatenation( regex::nfa::from_character( 'a' ), regex::nfa::from_character( 'b' ) )->execute( "ab" ), regex::match::accepted );
    EXPECT_EQ( regex::nfa::from_concatenation( regex::nfa::from_character( 'a' ), regex::nfa::from_character( 'b' ) )->execute( "ac" ), regex::match::rejected );
}

TEST( nfa, alternation )
{
    EXPECT_EQ( regex::nfa::from_alternation( regex::nfa::from_concatenation( regex::nfa::from_character( 'a' ), regex::nfa::from_character( 'b' ) ),
                                             regex::nfa::from_concatenation( regex::nfa::from_character( 'a' ), regex::nfa::from_character( 'c' ) ) )->execute( "ac" ), 
                                             regex::match::accepted );
    EXPECT_EQ( regex::nfa::from_alternation( regex::nfa::from_concatenation( regex::nfa::from_character( 'a' ), regex::nfa::from_character( 'b' ) ),
                                             regex::nfa::from_concatenation( regex::nfa::from_character( 'a' ), regex::nfa::from_character( 'c' ) ) )->execute( "a" ), 
                                             regex::match::rejected );
}

TEST( nfa, kleene )
{
    EXPECT_EQ( regex::nfa::from_kleene( regex::nfa::from_character( 'a' ) )->execute( "" ),   regex::match::accepted );
    EXPECT_EQ( regex::nfa::from_kleene( regex::nfa::from_character( 'a' ) )->execute( "a" ),  regex::match::accepted );
    EXPECT_EQ( regex::nfa::from_kleene( regex::nfa::from_character( 'a' ) )->execute( "aa" ), regex::match::accepted );
    EXPECT_EQ( regex::nfa::from_kleene( regex::nfa::from_character( 'a' ) )->execute( "b" ),  regex::match::rejected );
    EXPECT_EQ( regex::nfa::from_kleene( regex::nfa::from_character( 'a' ) )->execute( "ab" ), regex::match::rejected );
    EXPECT_EQ( regex::nfa::from_kleene( regex::nfa::from_character( 'a' ) )->execute( "ba" ), regex::match::rejected );
}

TEST( nfa, complex )
{
    auto state_machine = regex::nfa::from_alternation( 
        regex::nfa::from_concatenation( 
            regex::nfa::from_concatenation( 
                    regex::nfa::from_character( 'a' ), 
                    regex::nfa::from_kleene( regex::nfa::from_character( 'b' ) ) ),                 
            regex::nfa::from_character( 'c' ) ), 
        regex::nfa::from_kleene( regex::nfa::from_character( 'd' ) ) );

    EXPECT_EQ( state_machine->execute( "abbc" ), regex::match::accepted );
    EXPECT_EQ( state_machine->execute( "ac" ),   regex::match::accepted );
    EXPECT_EQ( state_machine->execute( "" ),     regex::match::accepted );
    EXPECT_EQ( state_machine->execute( "dd" ),   regex::match::accepted );
    EXPECT_EQ( state_machine->execute( "e" ),    regex::match::rejected );
    EXPECT_EQ( state_machine->execute( "bc" ),   regex::match::rejected );
}