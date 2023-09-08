#pragma once

#include <memory>
#include <set>
#include <stack>
#include <string_view>

#include "regex/automata/dfa.h"
#include "regex/automata/fa.h"
#include "regex/language/ast.h"
#include "regex/state/nstate.h"

namespace regex
{
    class nfa : public fa
    {
        std::set<std::unique_ptr<state::nstate>> states_;
        state::nstate *input_;
        state::nstate *output_;

      public:
        explicit nfa( state::nstate *input, state::nstate *output, std::set<std::unique_ptr<state::nstate>> states );
        explicit nfa( const nfa &other );
        explicit nfa( nfa &&other ) = delete;
        /*
         * Run target against the automata
         */
        bool execute( std::basic_string_view<language::character_type> target ) override;
        /*
         * Construct the deterministic version from the non-deterministic version
         */
        std::unique_ptr<dfa> to_dfa( );
        /*
         *
         *
         *      +---+      c     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         *
         *
         */
        static std::unique_ptr<nfa> from_character( language::character_type character );
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
        static std::unique_ptr<nfa> from_concatenation( std::unique_ptr<nfa> lhs, std::unique_ptr<nfa> rhs );
        /*
         *                   e   +---+            +---+    e
         *              ----->---|   |------>-----|   |---->----
         *      +---+ /          +---+            +---+          \ +---+
         *      | i |                                              | o |
         *      +---+ \          +---+            +---+          / +---+
         *              ----->---|   |------>-----|   |---->----
         *                   e   +---+            +---+    e
         */
        static std::unique_ptr<nfa> from_alternation( std::unique_ptr<nfa> lhs, std::unique_ptr<nfa> rhs );
        /*
         *                         +----------------<----------------+
         *                         |                e                |
         *      +---+      e     +---+            +---+      e     +---+
         *      | i |------>-----|   |------------|   |------>-----| o |
         *      +---+            +---+            +---+            +---+
         *        |                           e                      |
         *        +--------------------------->----------------------+
         */
        static std::unique_ptr<nfa> from_kleene( std::unique_ptr<nfa> expression );
    };
} // namespace regex
