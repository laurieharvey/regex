#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string_view>
#include <vector>

namespace regex::state {

    class nstate {
    public:
        using transition_label_type = char;
        using group_type = std::set<const nstate*>;
        using transitions_type = std::map<transition_label_type, group_type>;
        /*
         * The null transition which consumes no characters
         */
        static const transition_label_type epsilon;

        explicit nstate() = default;
        explicit nstate(const nstate&) = delete;
        explicit nstate(nstate&&) = delete;
        /*
         * Connect this to target via transition_label
         */
        void connect(nstate* target, transition_label_type transition_label);
        /*
         * Get the next nstate transitions
         */
        const transitions_type& transitions() const;

    private:
        transitions_type transitions_;
    };

    /*
     * Execute target string, returning on a match or false otherwise
     */
    bool execute(const nstate* start,
                 const nstate* finish,
                 std::basic_string_view<nstate::transition_label_type> target);

}  // namespace regex