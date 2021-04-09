#include <gtest/gtest.h>

#include "automata/dfa.h"
#include "utilities.h"

TEST( dfa, character )
{
    EXPECT_EQ( regex::dfa::from_character( 'a' )->execute( "a" ), regex::match::accepted );
    EXPECT_EQ( regex::dfa::from_character( 'a' )->execute( "b" ), regex::match::rejected );
}

TEST( dfa, any )
{
    EXPECT_EQ( regex::dfa::from_any( )->execute( "a" ), regex::match::accepted );
    EXPECT_EQ( regex::dfa::from_any( )->execute( "b" ), regex::match::accepted );
    EXPECT_EQ( regex::dfa::from_any( )->execute( "c" ), regex::match::accepted );
}

TEST( dfa, concatenation )
{
    EXPECT_EQ( regex::dfa::from_concatenation( regex::dfa::from_character( 'a' ), regex::dfa::from_character( 'b' ) )->execute( "ab" ), regex::match::accepted );
    EXPECT_EQ( regex::dfa::from_concatenation( regex::dfa::from_character( 'a' ), regex::dfa::from_character( 'b' ) )->execute( "ac" ), regex::match::rejected );
}

TEST( dfa, alternation )
{
    EXPECT_EQ( regex::dfa::from_alternation( regex::dfa::from_concatenation( regex::dfa::from_character( 'a' ), regex::dfa::from_character( 'b' ) ),
                                             regex::dfa::from_concatenation( regex::dfa::from_character( 'a' ), regex::dfa::from_character( 'c' ) ) )->execute( "ac" ), 
                                             regex::match::accepted );
    EXPECT_EQ( regex::dfa::from_alternation( regex::dfa::from_concatenation( regex::dfa::from_character( 'a' ), regex::dfa::from_character( 'b' ) ),
                                             regex::dfa::from_concatenation( regex::dfa::from_character( 'a' ), regex::dfa::from_character( 'c' ) ) )->execute( "a" ),  
                                             regex::match::rejected );
}

TEST( dfa, kleene )
{
    EXPECT_EQ( regex::dfa::from_kleene( regex::dfa::from_character( 'a' ) )->execute( "" ),   regex::match::accepted );
    EXPECT_EQ( regex::dfa::from_kleene( regex::dfa::from_character( 'a' ) )->execute( "a" ),  regex::match::accepted );
    EXPECT_EQ( regex::dfa::from_kleene( regex::dfa::from_character( 'a' ) )->execute( "aa" ), regex::match::accepted );
    EXPECT_EQ( regex::dfa::from_kleene( regex::dfa::from_character( 'a' ) )->execute( "b" ),  regex::match::rejected );
    EXPECT_EQ( regex::dfa::from_kleene( regex::dfa::from_character( 'a' ) )->execute( "ab" ), regex::match::rejected );
    EXPECT_EQ( regex::dfa::from_kleene( regex::dfa::from_character( 'a' ) )->execute( "ba" ), regex::match::rejected );
}

TEST( dfa, zero_or_one )
{
    EXPECT_EQ( regex::dfa::from_zero_or_one( regex::dfa::from_character( 'a' ) )->execute( "" ),   regex::match::accepted );
    EXPECT_EQ( regex::dfa::from_zero_or_one( regex::dfa::from_character( 'a' ) )->execute( "a" ),  regex::match::accepted );
    EXPECT_EQ( regex::dfa::from_zero_or_one( regex::dfa::from_character( 'a' ) )->execute( "aa" ), regex::match::rejected );
    EXPECT_EQ( regex::dfa::from_zero_or_one( regex::dfa::from_character( 'a' ) )->execute( "b" ),  regex::match::rejected );
    EXPECT_EQ( regex::dfa::from_zero_or_one( regex::dfa::from_character( 'a' ) )->execute( "ab" ), regex::match::rejected );
    EXPECT_EQ( regex::dfa::from_zero_or_one( regex::dfa::from_character( 'a' ) )->execute( "ba" ), regex::match::rejected );
}

TEST( dfa, complex )
{
    auto state_machine = regex::dfa::from_alternation( 
        regex::dfa::from_concatenation( 
            regex::dfa::from_concatenation( 
                    regex::dfa::from_character( 'a' ), 
                    regex::dfa::from_kleene( regex::dfa::from_character( 'b' ) ) ),                 
            regex::dfa::from_character( 'c' ) ), 
        regex::dfa::from_kleene( regex::dfa::from_character( 'd' ) ) );

    EXPECT_EQ( state_machine->execute( "abbc" ), regex::match::accepted );
    EXPECT_EQ( state_machine->execute( "ac" ),   regex::match::accepted );
    EXPECT_EQ( state_machine->execute( "" ),     regex::match::accepted );
    EXPECT_EQ( state_machine->execute( "dd" ),   regex::match::accepted );
    EXPECT_EQ( state_machine->execute( "e" ),    regex::match::rejected );
    EXPECT_EQ( state_machine->execute( "bc" ),   regex::match::rejected );
}