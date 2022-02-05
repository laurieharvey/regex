#include <algorithm>
#include <cassert>
#include <map>
#include <memory>
#include <set>
#include <string_view>
#include <vector>

#include "state/dstate.h"

namespace regex::state {

    void dstate::connect(dstate* target, transition_label_type transition_label) {
        assert(!transitions_.contains(transition_label));
        transitions_[transition_label] = target;
    }

    const dstate::transitions_type& dstate::transitions() const { return transitions_; }

    bool execute_internal(
        const dstate* state,
        const dstate::group_type& outputs,
        std::basic_string_view<dstate::transition_label_type> target,
        std::vector<std::pair<const dstate*,
                              std::basic_string_view<dstate::transition_label_type>>>& visited) {
        if (std::find_if(std::cbegin(visited), std::cend(visited),
                         [state, target](const auto& visit) {
                             return (visit.first == state && visit.second == target);
                         }) != std::cend(visited)) {
            return false;
        }

        visited.push_back({state, target});

        if (target.empty()) {
            return outputs.contains(state);
        }

        const auto next_transitions = state->transitions().find(target[0]);

        if (next_transitions != std::cend(state->transitions())) {
            return execute_internal(next_transitions->second, outputs, target.substr(1), visited);
        } else {
            return false;
        }
    };

    bool execute(const dstate* input,
                 const dstate::group_type& ouputs,
                 std::basic_string_view<dstate::transition_label_type> target) {
        std::vector<std::pair<const dstate*, std::basic_string_view<dstate::transition_label_type>>>
            visited;

        return execute_internal(input, ouputs, target, visited);
    }
}  // namespace regex::state