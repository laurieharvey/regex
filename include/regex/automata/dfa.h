#pragma once

#include <memory>
#include <set>
#include <stack>
#include <string_view>

#include "regex/automata/fa.h"
#include "regex/language/ast.h"
#include "regex/state/dstate.h"

namespace regex
{
    class dfa : public fa
    {
        std::set<std::unique_ptr<state::dstate>> states_;
        state::dstate *input_;
        state::dstate::group_type outputs_;

      public:
        explicit dfa( state::dstate *input, state::dstate::group_type outputs,
                      std::set<std::unique_ptr<state::dstate>> states );
        explicit dfa( const dfa &other ) = delete;
        explicit dfa( dfa &&other ) = delete;
        /*
         * Run target against the automata
         */
        bool execute( std::basic_string_view<language::character_type> target ) override;
    };
} // namespace regex
