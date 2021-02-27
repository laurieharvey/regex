#include <string_view>
#include <memory>
#include <map>
#include <algorithm>
#include <vector>
#include <set>

#include "state.h"
#include "ast.h"

namespace fa
{
    state::state(accepting acc)
        : acc_(acc),
          strong_transitions_(),
          weak_transitions_()
    {
    }

    void state::set(accepting acc)
    {
        acc_ = acc;
    }

    void state::connect(ast::character_type symbol, std::shared_ptr<state> st)
    {
        strong_transitions_[symbol].insert(st);
    }

    void state::connect(ast::character_type symbol, std::weak_ptr<state> st)
    {
        weak_transitions_[symbol].insert(st);
    }

    auto state::get_transitions(ast::character_type symbol)
    {
        std::set<std::weak_ptr<state>, std::owner_less<std::weak_ptr<state>>> result;

        typename std::map<ast::character_type, std::set<std::shared_ptr<state>>>::const_iterator strong_transitions = strong_transitions_.find(symbol);

        if (strong_transitions != std::cend(strong_transitions_))
        {
            std::copy(strong_transitions->second.cbegin(), strong_transitions->second.cend(), std::inserter(result, result.begin()));
        }

        typename std::map<ast::character_type, std::set<std::weak_ptr<state>, std::owner_less<std::weak_ptr<state>>>>::const_iterator weak_transitions = weak_transitions_.find(symbol);

        if (weak_transitions != std::cend(weak_transitions_))
        {
            std::copy(weak_transitions->second.cbegin(), weak_transitions->second.cend(), std::inserter(result, result.begin()));
        }

        return result;
    }

    std::set<std::weak_ptr<state>, std::owner_less<std::weak_ptr<state>>> state::get_epsilon_closure()
    {
        std::set<std::weak_ptr<state>, std::owner_less<std::weak_ptr<state>>> epsilon_closure;

        epsilon_closure.insert(std::enable_shared_from_this<state>::weak_from_this());

        std::set<std::weak_ptr<state>, std::owner_less<std::weak_ptr<state>>> epsilon_transitions = get_transitions(0x01);

        for (const std::weak_ptr<state> &epsilon_transition : epsilon_transitions)
        {
            if (epsilon_closure.find(epsilon_transition) == std::end(epsilon_closure))
            {
                epsilon_closure.insert(epsilon_transition);
                std::set<std::weak_ptr<state>, std::owner_less<std::weak_ptr<state>>> next_closure = epsilon_transition.lock()->get_epsilon_closure();
                std::copy(next_closure.begin(), next_closure.end(), std::inserter(epsilon_closure, epsilon_closure.begin()));
            }
        }

        return epsilon_closure;
    }

    auto state::get_transitions()
    {
        std::map<ast::character_type, std::set<std::weak_ptr<state>, std::owner_less<std::weak_ptr<state>>>> result = weak_transitions_;

        for (auto &states_for_transition : strong_transitions_)
        {
            for (const std::weak_ptr<state> &transition : states_for_transition.second)
            {
                result[states_for_transition.first].insert(transition);
            }
        }

        return result;
    }

    void state::walk(std::function<void(std::weak_ptr<state>)> callback, std::set<const state *> visited)
    {
        if (visited.find(this) != std::end(visited))
        {
            return;
        }
        else
        {
            visited.insert(this);

            std::weak_ptr<state> temp = std::enable_shared_from_this<state>::weak_from_this();

            callback(temp);

            for (auto &transitionsForCharacter : strong_transitions_)
            {
                for (auto &transition : transitionsForCharacter.second)
                {
                    transition->walk(callback, visited);
                }
            }

            for (auto &transitionsForCharacter : weak_transitions_)
            {
                for (auto &transition : transitionsForCharacter.second)
                {
                    transition.lock()->walk(callback, visited);
                }
            }
        }
    }

    match state::next(std::basic_string_view<ast::character_type> str, std::set<state *> visited)
    {
        if (visited.find(this) != std::end(visited))
        {
            return match::rejected;
        }

        visited.insert(this);

        if (str.empty())
        {
            if (acc_ == accepting::accepting)
            {
                return match::accepted;
            }

            for (const std::shared_ptr<state> &next : strong_transitions_[0x01])
            {
                if (next->next(str, visited) == match::accepted)
                {
                    return match::accepted;
                }
            }

            for (const std::weak_ptr<state> &next : weak_transitions_[0x01])
            {
                if (next.lock()->next(str, visited) == match::accepted)
                {
                    return match::accepted;
                }
            }

            return match::rejected;
        }

        for (const std::shared_ptr<state> &next : strong_transitions_[str[0]])
        {
            if (next->next(str.substr(1)) == match::accepted)
            {
                return match::accepted;
            }
        }

        for (const std::weak_ptr<state> &next : weak_transitions_[str[0]])
        {
            if (next.lock()->next(str.substr(1)) == match::accepted)
            {
                return match::accepted;
            }
        }

        for (const std::shared_ptr<state> &next : strong_transitions_[0x01])
        {
            if (next->next(str, visited) == match::accepted)
            {
                return match::accepted;
            }
        }

        for (const std::weak_ptr<state> &next : weak_transitions_[0x01])
        {
            if (next.lock()->next(str, visited) == match::accepted)
            {
                return match::accepted;
            }
        }

        return match::rejected;
    }
} // namespace fa