#include "automata/nfa.h"

#include <memory>
#include <string_view>

#include "language/ast.h"
#include "state/state.h"

namespace regex {
    nfa::nfa(state::nstate* input,
             state::nstate* output,
             std::set<std::unique_ptr<state::nstate>> states)
        : states_(std::move(states)), input_(input), output_(output) {}

    std::unique_ptr<nfa> nfa::from_character(language::character_type character) {
        auto input = std::make_unique<state::nstate>();
        auto output = std::make_unique<state::nstate>();

        input->connect(output.get(), character);

        auto i = input.get();
        auto o = output.get();

        std::set<std::unique_ptr<state::nstate>> states;
        states.insert(std::move(input));
        states.insert(std::move(output));

        return std::make_unique<nfa>(i, o, std::move(states));
    }

    static state::nstate* copy(const state::nstate* src, std::map<const state::nstate*, std::unique_ptr<state::nstate>>& visited)
    {
        const auto preexisting = visited.find(src);

        if(preexisting != std::cend(visited))
        {
            return preexisting->second.get();
        }
        else
        {
            auto new_state = visited.insert({src, std::make_unique<state::nstate>()}).first->second.get();

            for(const auto& label_to_states : src->transitions())
            {
                for(const auto& st: label_to_states.second)
                {
                    new_state->connect(copy(st, visited), label_to_states.first);
                }
            }

            return new_state;
        }
    }

     nfa::nfa(const nfa& other)
     {
        std::map<const state::nstate*, std::unique_ptr<state::nstate>> visited;

        input_ = copy(other.input_, visited);
        output_ = visited.find(other.output_)->second.get();

        std::transform(std::begin(visited), std::end(visited), std::inserter(states_, std::end(states_)), [](auto& st){
            return std::move(st.second);
        });
     }

    std::unique_ptr<nfa> nfa::from_epsilon() { return from_character(state::nstate::epsilon); }

    std::unique_ptr<nfa> nfa::from_any() {
        auto input = std::make_unique<state::nstate>();
        auto output = std::make_unique<state::nstate>();

        for (const auto character : language::alphabet) {
            input->connect(output.get(), character);
        }

        auto i = input.get();
        auto o = output.get();

        std::set<std::unique_ptr<state::nstate>> states;
        states.insert(std::move(input));
        states.insert(std::move(output));

        return std::make_unique<nfa>(i, o, std::move(states));
    }

    std::unique_ptr<nfa> nfa::from_concatenation(std::unique_ptr<nfa> lhs,
                                                 std::unique_ptr<nfa> rhs) {
        lhs->output_->connect(rhs->input_, state::nstate::epsilon);
        lhs->output_ = rhs->output_;
        lhs->states_.merge(std::move(rhs->states_));

        return lhs;
    }

    std::unique_ptr<nfa> nfa::from_alternation(std::unique_ptr<nfa> lhs, std::unique_ptr<nfa> rhs) {
        std::unique_ptr<state::nstate> input = std::make_unique<state::nstate>();
        std::unique_ptr<state::nstate> output = std::make_unique<state::nstate>();

        input->connect(lhs->input_, state::nstate::epsilon);
        input->connect(rhs->input_, state::nstate::epsilon);
        lhs->output_->connect(output.get(), state::nstate::epsilon);
        rhs->output_->connect(output.get(), state::nstate::epsilon);

        lhs->states_.merge(std::move(rhs->states_));

        lhs->input_ = input.get();
        lhs->output_ = output.get();

        lhs->states_.insert(std::move(input));
        lhs->states_.insert(std::move(output));

        return lhs;
    }

    std::unique_ptr<nfa> nfa::from_kleene(std::unique_ptr<nfa> expression) {
        auto input = std::make_unique<state::nstate>();
        auto output = std::make_unique<state::nstate>();

        input->connect(expression->input_, state::nstate::epsilon);
        input->connect(output.get(), state::nstate::epsilon);
        expression->output_->connect(output.get(), state::nstate::epsilon);
        output->connect(expression->input_, state::nstate::epsilon);

        expression->input_ = input.get();
        expression->output_ = output.get();

        expression->states_.insert(std::move(input));
        expression->states_.insert(std::move(output));

        return expression;
    }

    bool nfa::execute(std::basic_string_view<language::character_type> target) {
        return regex::state::execute(input_, output_, target);
    }

    static state::nstate::group_type epsilon_closure(const state::nstate* start) {
        state::nstate::group_type closure;

        auto fetch = [&closure](const state::nstate* start, auto fetch) {
            const bool did_not_exist = closure.insert(start).second;
            if (!did_not_exist) {
                return;
            } else {
                const auto next_epsilons = start->transitions().find(state::nstate::epsilon);
                if (next_epsilons != std::cend(start->transitions())) {
                    for (const auto next_epsilon : next_epsilons->second) {
                        fetch(next_epsilon, fetch);
                    }
                }
            }
        };

        fetch(start, fetch);

        return closure;
    }

    static state::dstate* subset_construction(
        const state::nstate::group_type& start,
        std::map<state::nstate::group_type, state::dstate*>& epsilon_closures_processing,
        std::set<std::unique_ptr<state::dstate>>& new_deterministic_states,
        const state::nstate* non_deterministic_output,
        std::set<const state::dstate*>& deterministic_outputs) {
        state::nstate::group_type closure;

        for (const auto st : start) {
            closure.merge(epsilon_closure(st));
        }

        const auto existing_closure = epsilon_closures_processing.find(closure);

        if (existing_closure != std::cend(epsilon_closures_processing)) {
            return existing_closure->second;
        }

        auto& new_deterministic_state =
            *new_deterministic_states.insert(std::make_unique<state::dstate>()).first;

        epsilon_closures_processing.insert({closure, new_deterministic_state.get()});

        if (closure.contains(non_deterministic_output)) {
            deterministic_outputs.insert(new_deterministic_state.get());
        }

        std::map<language::character_type, state::nstate::group_type> new_transitions;

        for (const auto state : closure) {
            for (auto next : state->transitions()) {
                if (next.first != state::nstate::epsilon) {
                    new_transitions[next.first].merge(next.second);
                }
            }
        }

        for (const auto& next : new_transitions) {
            new_deterministic_state->connect(
                subset_construction(next.second, epsilon_closures_processing,
                                    new_deterministic_states, non_deterministic_output,
                                    deterministic_outputs),
                next.first);
        }

        return new_deterministic_state.get();
    }

    std::unique_ptr<dfa> nfa::to_dfa(std::unique_ptr<nfa> src) {
        std::map<state::nstate::group_type, state::dstate*> epsilon_closures_processed;
        std::set<std::unique_ptr<state::dstate>> new_deterministic_states;
        std::set<const state::dstate*> deterministic_outputs;

        auto dfa_input =
            subset_construction(state::nstate::group_type{src->input_}, epsilon_closures_processed,
                                new_deterministic_states, src->output_, deterministic_outputs);

        auto a = std::make_unique<dfa>(dfa_input, std::move(deterministic_outputs),
                                       std::move(new_deterministic_states));

        return a;
    }
}  // namespace regex
