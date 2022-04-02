#pragma once

#include "regex/language/ast.h"

namespace regex
{
    class fa
    {
      public:
        virtual ~fa() = default;
        /*
         *  Run target against the automata
         */
        virtual bool execute( std::basic_string_view<language::character_type> target ) = 0;
    };
} // namespace regex
