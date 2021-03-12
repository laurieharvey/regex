#pragma once

#include <memory>
#include <set>
#include <string_view>

#include "state.h"
#include "stack.h"
#include "ast.h"
#include "fa.h"
#include "nfa.h"

namespace regex
{
    class dfa : public fa
    {
        std::shared_ptr<state> input_;
        std::set<std::shared_ptr<state>> outputs_;

        static const regex::character_type any     = 0x02;

    public:
        using character_type = regex::character_type;

        explicit dfa(std::shared_ptr<state> input, const std::set<std::shared_ptr<state>> &outputs);
        explicit dfa(std::shared_ptr<nfa>);
        explicit dfa(const dfa &other) = delete;
        explicit dfa(dfa &&other) = delete;

        static std::shared_ptr<dfa> from_nfa(std::shared_ptr<nfa>);
        /*
         *      +---+      c     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         */
        static std::shared_ptr<dfa> from_character(character_type c);
        /*
         *      +---+      a     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         */
        static std::shared_ptr<dfa> from_any();
        /*
         *   +----------------+---------+----------------+
         *   |  +---+         |  +---+  |         +---+  |
         *   |  | i |         |  |   |  |         | o |  |
         *   |  +---+         |  +---+  |         +---+  |
         *   +----------------+---------+----------------+
         */
        static std::shared_ptr<dfa> from_concatenation(std::shared_ptr<dfa> lhs, std::shared_ptr<dfa> rhs);

        void walk(std::function<void(std::weak_ptr<state>)> callback) override;

        match run(std::basic_string_view<character_type> str) override;
    };
} // namespace fa
