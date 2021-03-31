#pragma once

#include <memory>
#include <string_view>

#include "ast.h"
#include "automata/fa.h"
#include "stack.h"
#include "state/nstate.h"

namespace regex
{
    class nfa : public fa
    {
        std::shared_ptr<nstate> input_;
        std::shared_ptr<nstate> output_;

        static const regex::character_type epsilon = 0x01;
        static const regex::character_type any = 0x02;

    public:
        using character_type = regex::character_type;

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
        static std::shared_ptr<nfa> from_character( character_type c );
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

        match run( std::basic_string_view<character_type> str ) override;
    };

    struct nfa_generator
    {
        stack<std::shared_ptr<nfa>> s_;

        void callback( const regex::token &token );

        std::shared_ptr<nfa> result( );
    };
}  // namespace fa
