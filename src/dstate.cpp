#include <string_view>
#include <memory>
#include <map>
#include <algorithm>
#include <vector>
#include <set>

#include "dstate.h"

namespace regex
{
    dstate::dstate(accepting acc)
        : acc_(acc),
          transitions_()
    {
    }

    void dstate::set(accepting acc)
    {
        acc_ = acc;
    }

    bool dstate::is_accepting() const
    {
        return acc_ == accepting::accepting;
    }

    void dstate::connect(regex::character_type symbol, std::shared_ptr<dstate> st)
    {
        transitions_[symbol] = st;
        acc_ = accepting::nonaccepting;
    }

    void merge(std::shared_ptr<dstate> source, std::shared_ptr<dstate> target)
    {
        target->set(source->is_accepting() || target->is_accepting()
                        ? dstate::accepting::accepting
                        : dstate::accepting::nonaccepting);

        target->transitions_.merge(source->transitions_);

        for (const auto &source_transition : source->transitions_)
        {
            merge(source_transition.second, target->transitions_.find(source_transition.first)->second);
        }
    }

    void copy(std::shared_ptr<const dstate> source, std::shared_ptr<dstate> target)
    {
        for (const auto &source_transition : source->transitions_)
        {
            const auto &result = target->transitions_.insert(source_transition);
            const auto &existed = !result.second;
            const auto &position = result.first;

            if (existed)
            {
                copy(source_transition.second, position->second);
            }
        }
    }

    match dstate::next(std::basic_string_view<regex::character_type> str)
    {
        if (str.empty())
        {
            return acc_ == accepting::accepting ? match::accepted : match::rejected;
        }

        if (transitions_.find(str[0]) != std::cend(transitions_))
        {
            return transitions_.find(str[0])->second->next(str.substr(1));
        }
        else
        {
            return match::rejected;
        }
    }

    std::map<character_type, std::shared_ptr<dstate>> dstate::get_transitions()
    {
        return transitions_;
    }
} // namespace fa