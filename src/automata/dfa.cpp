#include <memory>
#include <queue>
#include <iostream>

#include "automata/dfa.h"
#include "automata/nfa.h"
#include "state/dstate.h"
#include "utilities.h"

namespace regex
{
    dfa::dfa(std::shared_ptr<dstate> input, const std::set<std::shared_ptr<dstate>> &outputs)
        : input_(input), outputs_(outputs)
    {
    }

    dfa::dfa(const dfa &other)
        : input_()
        , outputs_()
    {
        std::tie( input_, outputs_ ) = duplicate(other.input_);
    }

    std::shared_ptr<dfa> dfa::from_character(character_type c)
    {
        auto input = std::make_shared<dstate>(state::context::rejecting);
        auto output = std::make_shared<dstate>(state::context::accepting);

        input->connect(c, output);

        return std::make_shared<dfa>(input, std::set<std::shared_ptr<dstate>>{output});
    }

    std::shared_ptr<dfa> dfa::from_any()
    {
        return from_character(dfa::any);
    }

    std::shared_ptr<dfa> dfa::from_concatenation(std::shared_ptr<dfa> lhs, std::shared_ptr<dfa> rhs)
    {
        for (auto &lhs_output : lhs->outputs_)
        {
            merge(rhs->input_, lhs_output);

            if (rhs->input_->get_type() == state::context::rejecting)
            {
                lhs_output->set(state::context::rejecting);
            }
        }

        std::set<std::shared_ptr<dstate>> new_outputs;

        for (const auto &old_output : rhs->outputs_)
        {
            if (old_output == rhs->input_)
            {
                new_outputs.merge(lhs->outputs_);
            }
            else
            {
                new_outputs.insert(old_output);
            }
        }

        return std::make_shared<dfa>(lhs->input_, new_outputs);
    }

    std::shared_ptr<dfa> dfa::from_alternation(std::shared_ptr<dfa> lhs, std::shared_ptr<dfa> rhs)
    {
        merge(lhs->input_, rhs->input_);
        rhs->outputs_.merge(lhs->outputs_);
        return std::make_shared<dfa>(rhs->input_, rhs->outputs_);
    }

    std::shared_ptr<dfa> dfa::from_kleene(std::shared_ptr<dfa> expression)
    {
        for (auto &output : expression->outputs_)
        {
            copy(expression->input_, output);
        }

        expression->input_->set(state::context::accepting);
        expression->outputs_.insert(expression->input_);

        return std::make_shared<dfa>(expression->input_, expression->outputs_);
    }

    std::shared_ptr<dfa> dfa::from_zero_or_one(std::shared_ptr<dfa> expression)
    {
        expression->input_->set(state::context::accepting);

        return expression;
    }

    match dfa::run(std::basic_string_view<character_type> str)
    {
        return input_->next(str);
    }

    void dfa::walk(std::function<void(std::shared_ptr<state>)> callback)
    {
        input_->walk(callback);
    }

    void dfa_generator::callback(const regex::token &token)
    {
        std::shared_ptr<regex::dfa> lhs;
        std::shared_ptr<regex::dfa> rhs;
        std::vector<regex::character_type> alphabet = regex::get_alphabet();

        switch (token.get_type())
        {
        case regex::type::character:
            s_.push(dfa::from_character(token.get_token()));
            break;
        case regex::type::any:
            s_.push(dfa::from_any());
            break;
        case regex::type::alternation:
            rhs = s_.pop();
            lhs = s_.pop();
            s_.push(dfa::from_alternation(lhs, rhs));
            break;
        case regex::type::concatenation:
            rhs = s_.pop();
            lhs = s_.pop();
            s_.push(dfa::from_concatenation(lhs, rhs));
            break;
        case regex::type::kleene:
            s_.push(dfa::from_kleene(s_.pop()));
            break;
        case regex::type::zero_or_one:
            s_.push(dfa::from_zero_or_one(s_.pop()));
            break;
        case regex::type::one_or_more:
            lhs = s_.pop();
            rhs = std::make_shared<dfa>(*lhs);
            s_.push(dfa::from_concatenation(lhs, dfa::from_kleene(rhs)));
            break;
       case regex::type::parenthesis:
            break;
        }
    }

    std::shared_ptr<dfa> dfa_generator::result()
    {
        return s_.pop();
    }
}