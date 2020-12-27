#include <string>
#include <map>
#include <tuple>
#include <vector>
#include <utility>
#include <queue>

#include "state.h"
#include "nfa.h"

// {
//   '1': { 'ε*': [ 1, 2, 5 ] },
//   '2': { a: [ 3 ], 'ε*': [ 2 ] },
//   '3': { 'ε*': [ 3, 4 ] },
//   '4': { 'ε*': [ 4 ] },
//   '5': { b: [ 6 ], 'ε*': [ 5 ] },
//   '6': { 'ε*': [ 6, 4 ] },
// }

template <typename CharT>
using nfa_table = std::map<std::weak_ptr<fa::state<CharT>>, std::map<CharT, std::set<std::weak_ptr<fa::state<CharT>>, std::owner_less<std::weak_ptr<fa::state<CharT>>>>>, std::owner_less<std::weak_ptr<fa::state<CharT>>>>;

template <typename CharT>
std::pair<nfa_table<CharT>, std::weak_ptr<fa::state<CharT>>> generate_nfa_table(std::shared_ptr<fa::nfa<CharT>> nfa)
{
    nfa_table<CharT> table;
    std::weak_ptr<fa::state<CharT>> first;

    nfa->walk([&table, &first](std::weak_ptr<fa::state<CharT>> state) {
        if (table.empty())
        {
            first = state;
        }

        if (table.find(state) != std::end(table))
        {
            return;
        }
        else
        {
            table[state] = state.lock()->get_transitions();
            table[state][0x01] = state.lock()->get_epsilon_closure();
        }
    });

    return {table, first};
}

template <typename CharT>
void print_nfa_table(const nfa_table<CharT> &table)
{
    for (auto &state_to_transitions : table)
    {
        std::cout << state_to_transitions.first.lock().get();

        for (auto &transitions : state_to_transitions.second)
        {
            std::cout << '\t' << transitions.first << " {";

            for (auto &st : transitions.second)
            {
                std::cout << ' ' << st.lock().get();
            }

            std::cout << " }";
        }

        std::cout << std::endl;
    }
}

template <typename CharT>
class epsilon_closure
{
    std::set<std::weak_ptr<fa::state<CharT>>, std::owner_less<std::weak_ptr<fa::state<CharT>>>> epsilon_closure_;

public:
    epsilon_closure(std::set<std::weak_ptr<fa::state<CharT>>, std::owner_less<std::weak_ptr<fa::state<CharT>>>> &&closure)
        : epsilon_closure_(closure)
    {
    }

    bool operator<(const epsilon_closure &rhs) const
    {
        if (epsilon_closure_.size() < rhs.epsilon_closure_.size())
        {
            return true;
        }
        else if (epsilon_closure_.size() < rhs.epsilon_closure_.size())
        {
            return false;
        }
        else
        {
            for (auto lhs = std::cbegin(epsilon_closure_), rhs = std::cbegin(rhs.epsilon_closure_); lhs != std::cend(epsilon_closure_); ++lhs, ++rhs)
            {
                if (lhs->lock() < rhs->lock())
                {
                    return true;
                }
                else if (lhs->lock() > rhs->lock())
                {
                    return false;
                }
            }

            return false;
        }
    }
};

template <typename CharT>
using dfa_table = std::map<epsilon_closure<CharT>, std::map<CharT, epsilon_closure<CharT>>>;

template <typename CharT>
std::pair<dfa_table<CharT>, std::weak_ptr<fa::state<CharT>>> generate_dfa_table(std::shared_ptr<fa::nfa<CharT>> nfa)
{
    nfa_table<CharT> table;
    std::weak_ptr<fa::state<CharT>> start;

    std::tie(table, start) = generate_nfa_table(nfa);

    std::queue<std::weak_ptr<fa::state<CharT>>> assembly_line = {start};

    while (!assembly_line.empty())
    {
        std::set<std::weak_ptr<fa::state<CharT>>, std::owner_less<std::weak_ptr<fa::state<CharT>>>> epsilon_closure = table[assembly_line.front()][0x01];

        std::shared_ptr<fa::state<CharT>> state = std::make_shared<fa::state<CharT>>(state<CharT>::accepting::nonaccepting);

        for (auto &next : epsilon_closure)
        {
            if (next.lock() != assembly_line.front().lock())
            {
                assembly_line.push(next);
            }
        }

        assembly_line.pop();

        // { 2, 5 }
    }
}