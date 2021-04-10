#pragma once

#include <memory>
#include <string_view>
#include <stack>

#include "state/nstate.h"
#include "language/ast.h"
#include "automata/fa.h"

namespace regex
{
    class nfa : public fa
    {
        std::shared_ptr<nstate> input_;
        std::shared_ptr<nstate> output_;

        static const regex::language::character_type epsilon = 0x01;
        static const regex::language::character_type any = 0x02;

    public:
        explicit nfa( std::shared_ptr<nstate> input, std::shared_ptr<nstate> output );
        explicit nfa( const nfa &other ) = delete;
        explicit nfa( nfa &&other ) = delete;
        /*
         *
         *
         *      +---+      c     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         *
         *
         */
        static std::shared_ptr<nfa> from_character( language::character_type c );
        /*
         *
         *
         *      +---+      e     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         *
         *
         */
        static std::shared_ptr<nfa> from_epsilon( );
        /*
         *                 a
         *             ---->---
         *      +---+/          \+---+
         *      | i |----- b ----| o |
         *      +---+\          /+---+
         *             ---->---
         *                 c
         */
        static std::shared_ptr<nfa> from_any( );
        /*
         *
         *
         *      +---+            +---+      e     +---+            +---+
         *      | i |------>-----|   |------>-----|   |------>-----| o |
         *      +---+            +---+            +---+            +---+
         *
         *
         */
        static std::shared_ptr<nfa> from_concatenation( std::shared_ptr<nfa> lhs, std::shared_ptr<nfa> rhs );
        /*
         *                   e   +---+            +---+    e
         *              ----->---|   |------>-----|   |---->----
         *      +---+ /          +---+            +---+          \ +---+
         *      | i |                                              | o |
         *      +---+ \          +---+            +---+          / +---+
         *              ----->---|   |------>-----|   |---->----
         *                   e   +---+            +---+    e
         */
        static std::shared_ptr<nfa> from_alternation( std::shared_ptr<nfa> lhs, std::shared_ptr<nfa> rhs );
        /*
         *                         +----------------<----------------+
         *                         |                e                |
         *      +---+      e     +---+            +---+      e     +---+
         *      | i |------>-----|   |------------|   |------>-----| o |
         *      +---+            +---+            +---+            +---+
         *        |                           e                      |
         *        +--------------------------->----------------------+
         */
        static std::shared_ptr<nfa> from_kleene( std::shared_ptr<nfa> expression );

        void walk( std::function<void( std::shared_ptr<state> )> callback ) override;

        match execute( std::basic_string_view<language::character_type> target ) override;
    };

    struct nfa_generator
    {
        std::stack<std::shared_ptr<nfa>> s_;

        void callback( const regex::language::token &token );

        std::shared_ptr<nfa> result( );
    };
}  // namespace fa
