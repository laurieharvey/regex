#pragma once

#include <memory>
#include <set>
#include <stack>
#include <string_view>

#include "language/ast.h"
#include "state/nstate.h"
#include "automata/dfa.h"
#include "automata/fa.h"

namespace regex {
    class nfa : public fa {
        std::set<std::unique_ptr<state::nstate>> states_;
        state::nstate* input_;
        state::nstate* output_;

    public:
        explicit nfa(state::nstate* input,
                     state::nstate* output,
                     std::set<std::unique_ptr<state::nstate>> states);
        explicit nfa(const nfa& other) = delete;
        explicit nfa(nfa&& other) = delete;
        /*
         *
         */
        bool execute(std::basic_string_view<language::character_type> target) override;
        /*
         *
         */
        static std::unique_ptr<dfa> to_dfa(std::unique_ptr<nfa> lhs);
        /*
         *
         *
         *      +---+      c     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         *
         *
         */
        static std::unique_ptr<nfa> from_character(language::character_type character);
        /*
         *
         *
         *      +---+      e     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         *
         *
         */
        static std::unique_ptr<nfa> from_epsilon();
        /*
         *                 a
         *             ---->---
         *      +---+/          \+---+
         *      | i |----- b ----| o |
         *      +---+\          /+---+
         *             ---->---
         *                 c
         */
        static std::unique_ptr<nfa> from_any();
        /*
         *
         *
         *      +---+            +---+      e     +---+            +---+
         *      | i |------>-----|   |------>-----|   |------>-----| o |
         *      +---+            +---+            +---+            +---+
         *
         *
         */
        static std::unique_ptr<nfa> from_concatenation(std::unique_ptr<nfa> lhs,
                                                       std::unique_ptr<nfa> rhs);
        /*
         *                   e   +---+            +---+    e
         *              ----->---|   |------>-----|   |---->----
         *      +---+ /          +---+            +---+          \ +---+
         *      | i |                                              | o |
         *      +---+ \          +---+            +---+          / +---+
         *              ----->---|   |------>-----|   |---->----
         *                   e   +---+            +---+    e
         */
        static std::unique_ptr<nfa> from_alternation(std::unique_ptr<nfa> lhs,
                                                     std::unique_ptr<nfa> rhs);
        /*
         *                         +----------------<----------------+
         *                         |                e                |
         *      +---+      e     +---+            +---+      e     +---+
         *      | i |------>-----|   |------------|   |------>-----| o |
         *      +---+            +---+            +---+            +---+
         *        |                           e                      |
         *        +--------------------------->----------------------+
         */
        static std::unique_ptr<nfa> from_kleene(std::unique_ptr<nfa> expression);
    };
}  // namespace regex
