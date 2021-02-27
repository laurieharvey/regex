#include <memory>
#include <string_view>

#include "state.h"
#include "stack.h"
#include "ast.h"
#include "nfa.h"

namespace fa
{
    nfa::nfa(std::shared_ptr<state> input, std::shared_ptr<state> output)
        : input_(input),
          output_(output)
    {
    }

    nfa::nfa(const nfa &other)
        : input_(std::make_shared<state>(*other.input_)),
          output_(std::make_shared<state>(*other.output_))
    {
    }

    nfa::nfa(nfa &&other)
        : input_(other.input_),
          output_(other.output_)
    {
    }

    nfa &nfa::operator=(const nfa &other)
    {
        input_ = std::make_shared<state>(*input_);
        output_ = std::make_shared<state>(*output_);
        return *this;
    }

    std::shared_ptr<nfa> nfa::from_character(character_type c)
    {
        auto input = std::make_shared<state>(state::accepting::nonaccepting);
        auto output = std::make_shared<state>(state::accepting::accepting);

        input->connect(c, output);

        return std::make_shared<nfa>(input, output);
    }

    std::shared_ptr<nfa> nfa::from_epsilon()
    {
        return from_character(epsilon);
    }

    std::shared_ptr<nfa> nfa::from_concatenation(std::shared_ptr<nfa> lhs, std::shared_ptr<nfa> rhs)
    {
        lhs->output_->connect(epsilon, rhs->input_);

        lhs->output_->set(state::accepting::nonaccepting);
        rhs->output_->set(state::accepting::accepting);

        return std::make_shared<nfa>(lhs->input_, rhs->output_);
    }

    std::shared_ptr<nfa> nfa::from_alternation(std::shared_ptr<nfa> lhs, std::shared_ptr<nfa> rhs)
    {
        std::shared_ptr<state> input = std::make_shared<state>(state::accepting::nonaccepting);
        std::shared_ptr<state> output = std::make_shared<state>(state::accepting::accepting);

        input->connect(epsilon, lhs->input_);
        input->connect(epsilon, rhs->input_);

        lhs->output_->connect(epsilon, output);
        lhs->output_->set(state::accepting::nonaccepting);
        rhs->output_->connect(epsilon, output);
        rhs->output_->set(state::accepting::nonaccepting);

        return std::make_shared<nfa>(input, output);
    }

    std::shared_ptr<nfa> nfa::from_kleene(std::shared_ptr<nfa> expression)
    {
        std::shared_ptr<state> input = std::make_shared<state>(state::accepting::nonaccepting);
        std::shared_ptr<state> output = std::make_shared<state>(state::accepting::accepting);

        expression->output_->set(state::accepting::nonaccepting);

        input->connect(epsilon, expression->input_);
        input->connect(epsilon, output);
        expression->output_->connect(epsilon, output);
        output->connect(epsilon, std::weak_ptr<state>(expression->input_));

        return std::make_shared<nfa>(input, output);
    }

    void nfa::walk(std::function<void(std::weak_ptr<state>)> callback)
    {
        input_->walk(callback);
    }

    match nfa::run(std::basic_string_view<character_type> str)
    {
        return input_->next(str);
    }

    void generator::callback(const ast::token &token)
    {
        std::shared_ptr<fa::nfa> lhs;
        std::shared_ptr<fa::nfa> rhs;
        std::vector<ast::character_type> alphabet = ast::get_alphabet();

        switch (token.get_type())
        {
        case ast::type::character:
            s_.push(nfa::from_character(token.get_token()));
            break;
        case ast::type::any:
            for (auto symbol : alphabet)
            {
                s_.push(nfa::from_character(symbol));
            }
            for (int i = 0; i < alphabet.size() - 1; ++i)
            {
                rhs = s_.pop();
                lhs = s_.pop();
                s_.push(nfa::from_alternation(lhs, rhs));
            }
            break;
        case ast::type::alternation:
            rhs = s_.pop();
            lhs = s_.pop();
            s_.push(nfa::from_alternation(lhs, rhs));
            break;
        case ast::type::concatenation:
            rhs = s_.pop();
            lhs = s_.pop();
            s_.push(nfa::from_concatenation(lhs, rhs));
            break;
        case ast::type::kleene:
            s_.push(nfa::from_kleene(s_.pop()));
            break;
        case ast::type::zero_or_one:
            s_.push(nfa::from_alternation(nfa::from_epsilon(), s_.pop()));
            break;
        case ast::type::one_or_more:
            lhs = s_.pop();
            rhs = lhs;
            s_.push(nfa::from_concatenation(lhs, nfa::from_kleene(rhs)));
            break;
       case ast::type::parenthesis:
            // b
            break;
        }
    }

    std::shared_ptr<nfa> generator::result()
    {
        return s_.pop();
    }
} // namespace fa
