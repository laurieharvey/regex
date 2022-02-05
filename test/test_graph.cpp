#include <gtest/gtest.h>
#include <memory>

#include "graph/graph.h"
#include "graph/nfa.h"

TEST(graph, connect) {
  auto n = regex::nfa::from_character('a');
  EXPECT_TRUE(n->execute("a"));
  EXPECT_FALSE(n->execute("b"));
}