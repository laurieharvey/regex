#include <gtest/gtest.h>

#include "table.h"
#include "compile.h"
#include "utilities.h"

TEST(table, update)
{
    auto ta = generate_nfa_table(compile(std::stringstream("(ab)*")));

    std::cout << ta.first << std::endl;

    auto tb = generate_dfa_table(compile(std::stringstream("(abc)*")));

    std::cout << tb.first << std::endl;
}