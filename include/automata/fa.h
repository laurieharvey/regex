#pragma once

#include "state/state.h"
#include "language/ast.h"

namespace regex
{
    class fa
    {
    public:
        virtual void walk( std::function<void( std::shared_ptr<state> )> callback ) = 0;

        virtual match execute( std::basic_string_view<language::character_type> str ) = 0;
    };
}