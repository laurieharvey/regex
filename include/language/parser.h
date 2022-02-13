#pragma once

#include <array>
#include <exception>
#include <istream>
#include <memory>
#include <ostream>
#include <string>

#include "language/ast.h"

namespace regex
{
    namespace language
    {
        class ill_formed : std::runtime_error
        {
          public:
            /*
             * Thrown if the expression is ill formed
             */
            explicit ill_formed( const std::string & );
            explicit ill_formed( const ill_formed & ) = delete;
            explicit ill_formed( ill_formed && ) = delete;
        };

        using istream = std::basic_istream<language::character_type, std::char_traits<language::character_type>>;
        /*
         *	Create the regex AST from its string representation
         */
        std::unique_ptr<regex::language::token> parse( istream &&expression );
    } // namespace language
} // namespace regex