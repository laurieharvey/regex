#pragma once

#include <memory>
#include <set>
#include <string_view>

#include "state/dstate.h"
#include "stack.h"
#include "ast.h"
#include "automata/fa.h"
#include "automata/nfa.h"

namespace regex
{
    class dfa : public fa
    {
        std::shared_ptr<dstate> input_;
        std::set<std::shared_ptr<dstate>> outputs_;

        static const regex::character_type any     = 0x02;

    public:
        using character_type = regex::character_type;

        explicit dfa(std::shared_ptr<dstate> input, const std::set<std::shared_ptr<dstate>> &outputs);
        explicit dfa(const dfa &other);
        explicit dfa(dfa &&other) = delete;
        /*
         *
         * 
         *      +---+      c     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         * 
         * 
         */
        static std::shared_ptr<dfa> from_character(character_type);
        /*
         *
         * 
         *      +---+      a     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         * 
         * 
         */
        static std::shared_ptr<dfa> from_any();
        /*
         *                       +---+
         *                       |   |
         *      +---+            +---+            +---+
         *      | i |----->----<       >----->----| o |
         *      +---+            +---+            +---+
         *                       |   |
         *                       +---+
         */
        static std::shared_ptr<dfa> from_concatenation(std::shared_ptr<dfa> lhs, std::shared_ptr<dfa> rhs);
        /*
         *                       +---+            +---+
         *                       |   |------>-----| o |
         *      +---+            +---+            +---+
         *      | i |----->----<
         *      +---+            +---+            +---+
         *                       |   |------>-----| o |
         *                       +---+            +---+
         */
        static std::shared_ptr<dfa> from_alternation(std::shared_ptr<dfa> lhs, std::shared_ptr<dfa> rhs);
        /*
         *        
         *                         +--------<-------+
         *                         |                |
         *      +---+            +---+            +---+
         *      |i o|------------|   |------>-----| o |
         *      +---+            +---+            +---+
         * 
         */
        static std::shared_ptr<dfa> from_kleene(std::shared_ptr<dfa> expression);
        /*
         *
         * 
         *      +---+            +---+
         *      |i o|------>-----| o |
         *      +---+            +---+
         * 
         * 
         */
        static std::shared_ptr<dfa> from_zero_or_one(std::shared_ptr<dfa> expression);

        void walk(std::function<void(std::shared_ptr<state>)> callback) override;

        match run(std::basic_string_view<character_type> str) override;
    };

    struct dfa_generator
    {
        stack<std::shared_ptr<dfa>> s_;

        void callback(const regex::token &token);

        std::shared_ptr<dfa> result();
    };
} // namespace fa
