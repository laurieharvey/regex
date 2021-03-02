#pragma once

#include "state.h"
#include "ast.h"

namespace regex
{
    class fa
    {
    public:
        using character_type = regex::character_type;

        virtual match run(std::basic_string_view<character_type> str) = 0;
    };
}