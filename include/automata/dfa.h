#pragma once

#include <memory>
#include <set>
#include <stack>
#include <string_view>

#include "automata/fa.h"
#include "language/ast.h"
#include "state/dstate.h"

namespace regex
{
    class dfa : public fa
    {
        std::set<std::unique_ptr<state::dstate>> states_;
        state::dstate *input_;
        state::dstate::group_type outputs_;

      public:
        explicit dfa( state::dstate *input, const state::dstate::group_type &outputs,
                      std::set<std::unique_ptr<state::dstate>> states );
        explicit dfa( const dfa &other ) = delete;
        explicit dfa( dfa &&other ) = delete;
        /*
         *
         */
        bool execute( std::basic_string_view<language::character_type> target ) override;
    };
} // namespace regex
