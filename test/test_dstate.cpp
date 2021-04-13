#include <memory>
#include <gtest/gtest.h>

#include "state/dstate.h"

TEST( dstate, connect )
{
    auto start = std::make_shared<regex::dstate>( regex::state::context::rejecting );
    auto end = std::make_shared<regex::dstate>( regex::state::context::rejecting );

    connect( start, end, 'a' );

    EXPECT_EQ( start->execute( "a" ), regex::match::rejected );
}