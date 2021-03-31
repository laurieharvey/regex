#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string_view>
#include <vector>

#include "state/state.h"

namespace regex
{
    class nstate : public state, public std::enable_shared_from_this<nstate>
    {
    public:
        explicit nstate( state::context ctx = state::context::rejecting );

        void connect( regex::character_type symbol, std::shared_ptr<nstate> st );

        void connect( regex::character_type symbol, std::weak_ptr<nstate> st );

        group get_transitions( regex::character_type symbol ) override;

        group get_epsilon_closure( ) override;

        std::map<character_type, group> get_transitions( ) override;

        void walk( std::function<void( std::shared_ptr<state> )> callback, std::set<std::shared_ptr<state>> visited = std::set<std::shared_ptr<state>>( ) ) override;

        match run( std::basic_string_view<regex::character_type> str, std::set<std::shared_ptr<nstate>> visited = std::set<std::shared_ptr<nstate>>( ) );

    private:
        std::map<regex::character_type, std::set<std::shared_ptr<nstate>>> strong_transitions_;
        std::map<regex::character_type, std::set<std::weak_ptr<nstate>, std::owner_less<std::weak_ptr<nstate>>>> weak_transitions_;
    };
}  // namespace fa