#include <string_view>
#include <memory>
#include <map>
#include <algorithm>
#include <vector>
#include <set>

#include "state.h"
#include "ast.h"

namespace regex
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

    bool state::is_accepting() const
    {
        return acc_ == accepting::accepting;
    }

    void state::connect(regex::character_type symbol, std::shared_ptr<state> st)
    {
        strong_transitions_[symbol].insert(st);
    }

    void state::connect(regex::character_type symbol, std::weak_ptr<state> st)
    {
        weak_transitions_[symbol].insert(st);
    }

    group state::get_transitions(regex::character_type symbol)
    {
        group result;

        typename std::map<regex::character_type, std::set<std::shared_ptr<state>>>::const_iterator strong_transitions = strong_transitions_.find(symbol);

        if (strong_transitions != std::cend(strong_transitions_))
        {
            std::copy(strong_transitions->second.cbegin(), strong_transitions->second.cend(), std::inserter(result, result.begin()));
        }

        typename std::map<regex::character_type, std::set<std::weak_ptr<state>, std::owner_less<std::weak_ptr<state>>>>::const_iterator weak_transitions = weak_transitions_.find(symbol);

        if (weak_transitions != std::cend(weak_transitions_))
        {
            for(auto& weak_transition : weak_transitions->second)
            {
                result.insert(weak_transition.lock());
            }
        }

        return result;
    }

    group state::get_epsilon_closure()
    {
        group result;
        
        result.insert(std::enable_shared_from_this<state>::shared_from_this()); 

        for(const auto& next: get_transitions(0x01))
        {
            result.merge( next->get_epsilon_closure() );            
        }

        return result;
    }

    std::map<character_type, group> state::get_transitions()
    {
        std::map<character_type, group> result;

        for(const auto& weak_transition: weak_transitions_)
        {
            result.insert({weak_transition.first, get_transitions(weak_transition.first)});
        }

        for(const auto& strong_transition: strong_transitions_)
        {
            if(result.find(strong_transition.first) == std::cend(result))
            {
                result.insert({strong_transition.first, get_transitions(strong_transition.first)});
            }            
        }
        
        result[0x01] = get_epsilon_closure();

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

    match state::next(std::basic_string_view<regex::character_type> str, std::set<state *> visited)
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

        for (const std::shared_ptr<state> &next : strong_transitions_[0x02])
        {
            if (next->next(str.substr(1)) == match::accepted)
            {
                return match::accepted;
            }
        }

        for (const std::weak_ptr<state> &next : weak_transitions_[0x02])
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