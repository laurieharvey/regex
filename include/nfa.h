#pragma once

#include <memory>
#include <string_view>

#include "state.h"
#include "stack.h"
#include "ast.h"

namespace fa
{
    class nfa
    {
        std::shared_ptr<state> input_;
        std::shared_ptr<state> output_;

        static const ast::character_type epsilon = 0x01;

    public:
        using character_type = ast::character_type;

        explicit nfa(std::shared_ptr<state> input, std::shared_ptr<state> output);

        nfa(const nfa &other);

        nfa(nfa &&other);

        nfa &operator=(const nfa &other);
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

        void walk(std::function<void(std::weak_ptr<state>)> callback);

        match run(std::basic_string_view<character_type> str);
    };

    struct generator
    {
        stack<std::shared_ptr<nfa>> s_;

        void callback(const ast::token &token);

        std::shared_ptr<nfa> result();
    };
} // namespace fa
