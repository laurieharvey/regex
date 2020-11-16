#pragma once

#include <memory>
#include <string_view>

#include "state.h"
#include "stack.h"
#include "ast.h"

namespace fa
{
    template <typename CharT>
    class nfa
    {
        std::shared_ptr<state<CharT>> input_;
        std::shared_ptr<state<CharT>> output_;

        static const CharT epsilon = 0x01;

    public:
        explicit nfa(std::shared_ptr<state<CharT>> input, std::shared_ptr<state<CharT>> output)
            : input_(input),
              output_(output)
        {
        }

        nfa(const nfa<CharT> &other)
            : input_(std::make_shared<state<CharT>>(*input_)),
              output_(std::make_shared<state<CharT>>(*output_))
        {
        }

        nfa(nfa<CharT> &&other)
            : input_(other.input_),
              output_(other.output_)
        {
        }

        /*
         *      +---+      c     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         */
        static std::shared_ptr<nfa<CharT>> from_character(CharT c)
        {
            auto input = std::make_shared<state<CharT>>(state<CharT>::accepting::nonaccepting);
            auto output = std::make_shared<state<CharT>>(state<CharT>::accepting::accepting);

            input->connect(c, output);

            return std::make_shared<nfa<CharT>>(input, output);
        }

        /*
         *      +---+      e     +---+
         *      | i |------>-----| o |
         *      +---+            +---+
         */
        static std::shared_ptr<nfa<CharT>> from_epsilon()
        {
            return from_character(epsilon);
        }

        /*
         *   +--------------------------+      +--------------------------+
         *   |  +---+            +---+  |   e  |  +---+            +---+  |
         *   |  | i |            | o |------>-----| i |            | o |  |
         *   |  +---+            +---+  |      |  +---+            +---+  |
         *   +--------------------------+      +--------------------------+
         */
        static std::shared_ptr<nfa<CharT>> from_concatenation(std::shared_ptr<nfa<CharT>> lhs, std::shared_ptr<nfa<CharT>> rhs)
        {
            lhs->output_->connect(epsilon, rhs->input_);

            lhs->output_->set(state<CharT>::accepting::nonaccepting);
            rhs->output_->set(state<CharT>::accepting::accepting);

            return std::make_shared<nfa<CharT>>(lhs->input_, rhs->output_);
        }

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
        static std::shared_ptr<nfa<CharT>> from_alternation(std::shared_ptr<nfa<CharT>> lhs, std::shared_ptr<nfa<CharT>> rhs)
        {
            std::shared_ptr<state<CharT>> input = std::make_shared<state<CharT>>(state<CharT>::accepting::nonaccepting);
            std::shared_ptr<state<CharT>> output = std::make_shared<state<CharT>>(state<CharT>::accepting::accepting);

            input->connect(epsilon, lhs->input_);
            input->connect(epsilon, rhs->input_);

            lhs->output_->connect(epsilon, output);
            lhs->output_->set(state<CharT>::accepting::nonaccepting);
            rhs->output_->connect(epsilon, output);
            rhs->output_->set(state<CharT>::accepting::nonaccepting);

            return std::make_shared<nfa<CharT>>(input, output);
        }

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
        static std::shared_ptr<nfa<CharT>> from_kleene(std::shared_ptr<nfa<CharT>> expression)
        {
            std::shared_ptr<state<CharT>> input = std::make_shared<state<CharT>>(state<CharT>::accepting::nonaccepting);
            std::shared_ptr<state<CharT>> output = std::make_shared<state<CharT>>(state<CharT>::accepting::accepting);

            expression->output_->set(state<CharT>::accepting::nonaccepting);

            input->connect(epsilon, expression->input_);
            input->connect(epsilon, output);
            expression->output_->connect(epsilon, output);
            output->connect(epsilon, expression->input_);

            return std::make_shared<nfa<CharT>>(input, output);
        }

        match run(std::basic_string_view<CharT> str)
        {
            return input_->next(str);
        }
    };

    template <typename CharT>
    struct generator
    {
        stack<std::shared_ptr<nfa<CharT>>> s_;

        void callback(const ast::token<CharT> &token)
        {
            std::shared_ptr<fa::nfa<CharT>> lhs;
            std::shared_ptr<fa::nfa<CharT>> rhs;

            switch (token.type())
            {
            case ast::type::character:
                s_.push(nfa<CharT>::from_character(token.get_token()));
                break;
            case ast::type::alternation:
                lhs = s_.pop();
                rhs = s_.pop();
                s_.push(nfa<CharT>::from_alternation(lhs, rhs));
                break;
            case ast::type::concatenation:
                lhs = s_.pop();
                rhs = s_.pop();
                s_.push(nfa<CharT>::from_concatenation(lhs, rhs));
                break;
            case ast::type::kleene:
                s_.push(nfa<CharT>::from_kleene(s_.pop()));
                break;
            case ast::type::zero_or_one:
                s_.push(nfa<CharT>::from_alternation(nfa<CharT>::from_epsilon(), s_.pop()));
                break;
            }
        }

        std::shared_ptr<nfa<CharT>> result()
        {
            return s_.pop();
        }
    };
} // namespace fa
