#include <memory>
#include <gtest/gtest.h>

#include "state/dstate.h"

TEST( dstate, connect )
{
    auto start = std::make_shared<regex::dstate>( regex::state::context::rejecting );

    regex::dstate::connect( start, std::make_shared<regex::dstate>( regex::state::context::rejecting ), 'a' );

    EXPECT_EQ( start->execute( "a" ), regex::match::rejected );
    EXPECT_EQ( start->execute( "b" ), regex::match::rejected );

    regex::dstate::connect( start, std::make_shared<regex::dstate>( regex::state::context::accepting ), 'a' );

    EXPECT_EQ( start->execute( "a" ), regex::match::accepted );
    EXPECT_EQ( start->execute( "b" ), regex::match::rejected );

    regex::dstate::connect( start, std::make_shared<regex::dstate>( regex::state::context::accepting ), 'b' );

    EXPECT_EQ( start->execute( "a" ), regex::match::accepted );
    EXPECT_EQ( start->execute( "b" ), regex::match::accepted );

    auto loop = std::make_shared<regex::dstate>( regex::state::context::accepting );

    regex::dstate::connect( loop, loop, 'a' );

    EXPECT_EQ( loop->execute( "" ),   regex::match::accepted );
    EXPECT_EQ( loop->execute( "a" ),  regex::match::accepted );
    EXPECT_EQ( loop->execute( "aa" ), regex::match::accepted );
}

TEST( dstate, merge )
{
    auto state_11 = std::make_shared<regex::dstate>( regex::state::context::rejecting );
    auto state_12 = std::make_shared<regex::dstate>( regex::state::context::rejecting );
    auto state_13 = std::make_shared<regex::dstate>( regex::state::context::accepting );
    auto state_14 = std::make_shared<regex::dstate>( regex::state::context::accepting );

    regex::dstate::connect( state_11, state_12, 'a' );
    regex::dstate::connect( state_12, state_14, 'd' );
    regex::dstate::connect( state_11, state_13, 'b' );

    auto state_21 = std::make_shared<regex::dstate>( regex::state::context::accepting );
    auto state_22 = std::make_shared<regex::dstate>( regex::state::context::accepting );
    auto state_23 = std::make_shared<regex::dstate>( regex::state::context::accepting );
    auto state_24 = std::make_shared<regex::dstate>( regex::state::context::accepting );

    regex::dstate::connect( state_21, state_22, 'a' );
    regex::dstate::connect( state_22, state_24, 'e' );
    regex::dstate::connect( state_21, state_23, 'c' );

    regex::dstate::merge( state_21, state_11 );

    EXPECT_EQ( state_11->execute( "a" ),  regex::match::accepted );
    EXPECT_EQ( state_11->execute( "b" ),  regex::match::accepted );
    EXPECT_EQ( state_11->execute( "c" ),  regex::match::accepted );
    EXPECT_EQ( state_11->execute( "ad" ), regex::match::accepted );
    EXPECT_EQ( state_11->execute( "ae" ), regex::match::accepted );


    auto loop = std::make_shared<regex::dstate>( regex::state::context::accepting );

    regex::dstate::connect( loop, loop, 'a' );
    regex::dstate::merge( loop, loop );

    EXPECT_EQ( loop->execute( "" ),   regex::match::accepted );
    EXPECT_EQ( loop->execute( "a" ),  regex::match::accepted );
    EXPECT_EQ( loop->execute( "aa" ), regex::match::accepted );
}