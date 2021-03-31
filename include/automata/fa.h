#pragma once

#include "ast.h"
#include "state/state.h"

namespace regex
{
    class fa
    {
    public:
        using character_type = regex::character_type;

        virtual void walk( std::function<void( std::shared_ptr<state> )> callback ) = 0;

        virtual match run( std::basic_string_view<character_type> str ) = 0;
    };
}  // namespace regex