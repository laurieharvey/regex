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
        explicit dstate( state::context ctx = state::context::rejecting );
        explicit dstate( const dstate& );
        explicit dstate( dstate&& ) = delete;

        friend void connect( std::shared_ptr<dstate> src, std::shared_ptr<dstate> dest, regex::language::character_type symbol );

        friend void merge( std::shared_ptr<dstate> src, std::shared_ptr<dstate> target );

        friend void shallow_copy( std::shared_ptr<const dstate> src, std::shared_ptr<dstate> target, std::set<std::shared_ptr<dstate>> );

        friend std::pair<std::shared_ptr<dstate>, std::set<std::shared_ptr<dstate>>> duplicate( std::shared_ptr<const dstate> src );

        group get_transitions( language::character_type symbol ) override;

        group get_epsilon_closure( ) override;

        std::map<language::character_type, group> get_transitions( ) override;

        void walk( std::function<void( std::shared_ptr<state> )> callback, std::set<std::shared_ptr<state>> visited = std::set<std::shared_ptr<state>>( ) ) override;

        match execute( std::basic_string_view<language::character_type> str );

    private:
        std::map<language::character_type, std::shared_ptr<dstate>> transitions_;
    };
}  // namespace regex