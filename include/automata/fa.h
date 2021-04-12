#pragma once

#include "state/state.h"
#include "language/ast.h"

namespace regex
{
    class fa
    {
    public:
        virtual ~fa( ){ };
        /*
         *  Run target against the automata
         */
        virtual match execute( std::basic_string_view<language::character_type> target ) = 0;
    };
}