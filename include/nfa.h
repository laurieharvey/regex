#pragma once

#include <memory>
#include <string_view>

#include "state.h"
#include "stack.h"
#include "ast.h"
#include "fa.h"

namespace regex
{
    class nfa : public fa
    {
        std::shared_ptr<state> input_;
        std::shared_ptr<state> output_;

        static const regex::character_type epsilon = 0x01;
        static const regex::character_type any     = 0x02;

    public:
        using character_type = regex::character_type;

        explicit nfa(std::shared_ptr<state> input, std::shared_ptr<state> output);
        explicit nfa(const nfa &other) = delete;
        explicit nfa(nfa &&other) = delete;
        /*
         *      +---+      c     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         */
        static std::shared_ptr<nfa> from_character(character_type c);
        /*
         *      +---+      e     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         */
        static std::shared_ptr<nfa> from_epsilon();
        /*
         *      +---+      a     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         */
        static std::shared_ptr<nfa> from_any();
        /*
         *   +--------------------------+      +--------------------------+
         *   |  +---+            +---+  |   e  |  +---+            +---+  |
         *   |  | i |            | o |------>-----| i |            | o |  |
         *   |  +---+            +---+  |      |  +---+            +---+  |
         *   +--------------------------+      +--------------------------+
         */
        static std::shared_ptr<nfa> from_concatenation(std::shared_ptr<nfa> lhs, std::shared_ptr<nfa> rhs);
        /*
         *          +--------------------------+
         *       e  |  +---+            +---+  |  e
         *   +--->-----| i |            | o |----->---+
         *   |      |  +---+            +---+  |      |
         * +---+    +--------------------------+    +---+
         * | i |                                    | o |
         * +---+    +--------------------------+    +---+
         *   |      |  +---+            +---+  |      |
         *   +--->-----| i |            | o |----->---+
         *       e  |  +---+            +---+  |  e
         *          +--------------------------+
         */
        static std::shared_ptr<nfa> from_alternation(std::shared_ptr<nfa> lhs, std::shared_ptr<nfa> rhs);
        /*
         *                                     e                 
         *                    +----------------<----------------+
         *               +----|---------------------+           |
         * +---+      e  |  +---+            +---+  |   e     +---+
         * | i |------>-----| i |            | o |------>-----| o |
         * +---+         |  +---+            +---+  |         +---+
         *   |           +--------------------------+           |
         *   +--------------------------->----------------------+
         *                               e
         */
        static std::shared_ptr<nfa> from_kleene(std::shared_ptr<nfa> expression);

        void walk(std::function<void(std::weak_ptr<state>)> callback) override;

        match run(std::basic_string_view<character_type> str) override;
    };

    struct generator
    {
        stack<std::shared_ptr<nfa>> s_;

        void callback(const regex::token &token);

        std::shared_ptr<nfa> result();
    };
} // namespace fa
