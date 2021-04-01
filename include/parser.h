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
     * 3 { '(', ')' } 2 { '*', '?', '+' } 1 { '-' }
     */
    const std::array<int, 128> precedence{
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 2, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    bool is_operator( character_type token );

    bool is_unary_operator( character_type token );

    bool is_character( character_type token );

    /*
     *	Convert an implicit regex expression to an explicit expression
     * 	Implicit concatonation operator -> explicit '-'
     */
    std::basic_stringstream<character_type> make_explicit( std::basic_istream<character_type>& input );

    /*
     *	Parse an explicit regex expression using the shunting yard algorithm to produce the AST
     *	https://en.wikipedia.org/wiki/Shunting-yard_algorithm
     */
    std::unique_ptr<regex::token> parse( std::basic_istream<character_type, std::char_traits<character_type>>& expression );
}