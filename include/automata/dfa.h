#pragma once

#include <memory>
#include <set>
#include <string_view>

#include "state/dstate.h"
#include "stack.h"
#include "language/ast.h"
#include "automata/fa.h"
#include "automata/nfa.h"

namespace regex
{
    class dfa : public fa
    {
        std::shared_ptr<dstate> input_;
        std::set<std::shared_ptr<dstate>> outputs_;

    public:
        explicit dfa( std::shared_ptr<dstate> input, const std::set<std::shared_ptr<dstate>> &outputs );
        explicit dfa( const dfa &other );
        explicit dfa( dfa &&other ) = delete;
        /*
         *
         *
         *      +---+      c     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         *
         *
         */
        static std::shared_ptr<dfa> from_character( language::character_type );
        /*
         *                 a
         *             ---->---
         *      +---+/          \+---+
         *      | i |----- b ----| o |
         *      +---+\          /+---+
         *             ---->---
         *                 c
         */
        static std::shared_ptr<dfa> from_any( );
        /*
         *                       +---+
         *                       |   |
         *      +---+            +---+            +---+
         *      | i |----->----<       >----->----| o |
         *      +---+            +---+            +---+
         *                       |   |
         *                       +---+
         */
        static std::shared_ptr<dfa> from_concatenation( std::shared_ptr<dfa> lhs, std::shared_ptr<dfa> rhs );
        /*
         *                       +---+            +---+
         *                       |   |------>-----| o |
         *      +---+            +---+            +---+
         *      | i |----->----<
         *      +---+            +---+            +---+
         *                       |   |------>-----| o |
         *                       +---+            +---+
         */
        static std::shared_ptr<dfa> from_alternation( std::shared_ptr<dfa> lhs, std::shared_ptr<dfa> rhs );
        /*
         *
         *                         +--------<-------+
         *                         |                |
         *      +---+            +---+            +---+
         *      |i o|------------|   |------>-----| o |
         *      +---+            +---+            +---+
         *
         */
        static std::shared_ptr<dfa> from_kleene( std::shared_ptr<dfa> expression );
        /*
         *
         *
         *      +---+            +---+
         *      |i o|------>-----| o |
         *      +---+            +---+
         *
         *
         */
        static std::shared_ptr<dfa> from_zero_or_one( std::shared_ptr<dfa> expression );

        void walk( std::function<void( std::shared_ptr<state> )> callback ) override;

        match execute( std::basic_string_view<language::character_type> target ) override;
    };

    struct dfa_generator
    {
        stack<std::shared_ptr<dfa>> s_;

        void callback( const regex::language::token &token );

        std::shared_ptr<dfa> result( );
    };
}  // namespace regex
