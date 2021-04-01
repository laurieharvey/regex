#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string_view>
#include <vector>

#include "language/ast.h"
#include "state/state.h"

namespace regex
{
    class dstate : public state, public std::enable_shared_from_this<dstate>
    {
    public:
        enum class accepting
        {
            accepting,
            nonaccepting
        };

        explicit dstate( state::context ctx = state::context::rejecting );
        explicit dstate( const dstate& );
        explicit dstate( dstate&& ) = delete;

        void connect( regex::character_type symbol, std::shared_ptr<dstate> st );

        friend void merge( std::shared_ptr<dstate> src, std::shared_ptr<dstate> target );

        friend void shallow_copy( std::shared_ptr<const dstate> src, std::shared_ptr<dstate> target, std::set<std::shared_ptr<dstate>> );

        friend std::pair<std::shared_ptr<dstate>, std::set<std::shared_ptr<dstate>>> duplicate( std::shared_ptr<const dstate> src );

        group get_transitions( regex::character_type symbol ) override;

        group get_epsilon_closure( ) override;

        std::map<character_type, group> get_transitions( ) override;

        // std::map<character_type, std::shared_ptr<dstate>> get_transitions();

        void walk( std::function<void( std::shared_ptr<state> )> callback, std::set<std::shared_ptr<state>> visited = std::set<std::shared_ptr<state>>( ) ) override;

        match next( std::basic_string_view<regex::character_type> str );

    private:
        std::map<regex::character_type, std::shared_ptr<dstate>> transitions_;
    };
}  // namespace regex