#include <memory>
#include <queue>
#include <iostream>

#include "dfa.h"
#include "nfa.h"
#include "state.h"
#include "utilities.h"

namespace regex
{
    dfa::dfa(std::shared_ptr<state> input, const std::set<std::shared_ptr<state>> &outputs)
        : input_(input), outputs_(outputs)
    {
    }

    dfa::dfa(std::shared_ptr<nfa> state_machine)
        : input_(std::make_shared<state>())
    {
        table dfa_table;
        group starting_state;

        std::tie(dfa_table, starting_state) = generate_dfa_table(state_machine);

        // std::cout << dfa_table << std::endl;

        std::queue<std::pair<group, std::shared_ptr<state>>> assembly_line;

        assembly_line.push({starting_state, input_});

        std::set<group> nfa_states_processed({starting_state});

        while (!assembly_line.empty())
        {
            group nfa_group;
            std::shared_ptr<state> dfa_state;

            std::tie(nfa_group, dfa_state) = assembly_line.front();
            assembly_line.pop();

            if (dfa_table.find(nfa_group) == std::cend(dfa_table))
            {
                dfa_state->set(state::accepting::accepting);
            }
            else
            {
                auto dfa_transitions = dfa_table.find(nfa_group)->second;

                for (row::size_type i = 0; i < dfa_transitions.size(); ++i)
                {
                    if (!dfa_transitions[i].empty())
                    {
                        auto next_state = std::make_shared<state>();
                        dfa_state->connect(dfa_table.get_schema()[i], next_state);

                        if (nfa_states_processed.find(dfa_transitions[i]) == std::cend(nfa_states_processed))
                        {
                            assembly_line.push({dfa_transitions[i], next_state});
                        }
                    }
                }

                nfa_states_processed.insert(dfa_table.find(nfa_group)->first);

                for (const auto &s : dfa_table.find(nfa_group)->first)
                {
                    if (s->is_accepting())
                    {
                        dfa_state->set(state::accepting::accepting);
                    }
                }
            }
        }
    }

    std::shared_ptr<dfa> dfa::from_nfa(std::shared_ptr<nfa> other)
    {
        return std::make_shared<dfa>(other);
    }

    std::shared_ptr<dfa> dfa::from_character(character_type c)
    {
        auto input = std::make_shared<state>(state::accepting::nonaccepting);
        auto output = std::make_shared<state>(state::accepting::accepting);

        input->connect(c, output);

        return std::make_shared<dfa>(input, std::set<std::shared_ptr<state>>{output});
    }

    std::shared_ptr<dfa> dfa::from_any()
    {
        return from_character(dfa::any);
    }

    std::shared_ptr<dfa> dfa::from_concatenation(std::shared_ptr<dfa> lhs, std::shared_ptr<dfa> rhs)
    {
        for (const auto &lhs_output : lhs->outputs_)
        {
            for (const auto &rhs_connection : rhs->input_->get_transitions())
            {
                lhs_output->connect(rhs_connection.first, *rhs_connection.second.begin());
            }
        }

        return std::make_shared<dfa>(lhs->input_, rhs->outputs_);
    }

    void dfa::walk(std::function<void(std::weak_ptr<state>)> callback)
    {
    }

    match dfa::run(std::basic_string_view<character_type> str)
    {
        return input_->next(str);
    }
}