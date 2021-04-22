#include <gtest/gtest.h>

#include "utilities/compile.h"
#include "table.h"
#include "table/utilities.hpp"
#include "utilities.h"

TEST( table, update )
{
    auto ntb = generate_nfa_table( regex::compile_nfa( std::stringstream( "(ab)*" ) ) );

    std::cout << ntb << std::endl;

    auto dtb = generate_dfa_table( regex::compile_dfa( std::stringstream( "(ab)*" ) ) );

    std::cout << dtb << std::endl;
}