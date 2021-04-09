#pragma once

#include <array>
#include <istream>
#include <memory>
#include <ostream>

#include "language/ast.h"
#include "stack.h"

namespace regex
{
    /*
     *	Parse an explicit regex expression using the shunting yard algorithm to produce the AST
     *	https://en.wikipedia.org/wiki/Shunting-yard_algorithm
     */
    std::unique_ptr<regex::language::token> parse( std::basic_istream<language::character_type, std::char_traits<language::character_type>>& expression );
}