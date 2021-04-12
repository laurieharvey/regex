#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string_view>
#include <vector>

#include "language/ast.h"

namespace regex
{
    enum class match
    {
        accepted,
        rejected
    };

    class state
    {
    public:
        enum class context
        {
            accepting,
            rejecting,
        };

        explicit state( context ctx );

        context get_type( ) const;

        void set( state::context ctx );

    private:
        context ctx_;
    };
}  // namespace fa