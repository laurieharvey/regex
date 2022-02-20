#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string_view>
#include <vector>

#include "language/alphabet.h"

namespace regex::state
{
    class dstate
    {
      public:
        using transition_label_type = language::character_type;
        using group_type = std::set<const dstate *>;
        using transitions_type = std::map<transition_label_type, dstate *>;

        explicit dstate() = default;
        explicit dstate( const dstate & ) = delete;
        explicit dstate( dstate && ) = delete;
        /*
         * Connect this to target via transition_label
         */
        void connect( dstate *target, transition_label_type transition_label );
        /*
         * Get the next nstate transitions
         */
        const transitions_type &transitions() const;

      private:
        transitions_type transitions_;
    };
    /*
     * Execute target string, returning on a match or false otherwise
     */
    bool execute( const dstate *input, const dstate::group_type &ouputs,
                  std::basic_string_view<dstate::transition_label_type> target );
} // namespace regex::state