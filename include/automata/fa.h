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
         *  Recursively walk the automata passing each state to callback 
         */
        virtual void walk( std::function<void( std::shared_ptr<state> )> callback ) = 0;
        /*
         *  Run target against the automata
         */
        virtual match execute( std::basic_string_view<language::character_type> target ) = 0;
    };
}