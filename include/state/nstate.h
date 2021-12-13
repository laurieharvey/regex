#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string_view>
#include <vector>

#include "state/state.h"

namespace regex {
class nstate;

using group = std::set<std::shared_ptr<nstate>>;

class nstate : public state, public std::enable_shared_from_this<nstate> {
 public:
  explicit nstate(state::context ctx = state::context::rejecting);

  void connect(language::character_type symbol, std::shared_ptr<nstate> st);

  void connect(language::character_type symbol, std::weak_ptr<nstate> st);

  std::set<std::shared_ptr<nstate>> get_transitions(language::character_type symbol);

  std::set<std::shared_ptr<nstate>> get_epsilon_closure();

  std::map<language::character_type, std::set<std::shared_ptr<nstate>>> get_transitions();

  void walk(std::function<void(std::shared_ptr<nstate>)> callback,
            std::set<std::shared_ptr<nstate>> visited = std::set<std::shared_ptr<nstate>>());

  match execute(std::basic_string_view<language::character_type> str,
                std::set<std::shared_ptr<nstate>> visited = std::set<std::shared_ptr<nstate>>());

 private:
  std::map<language::character_type, std::set<std::shared_ptr<nstate>>> strong_transitions_;
  std::map<language::character_type,
           std::set<std::weak_ptr<nstate>, std::owner_less<std::weak_ptr<nstate>>>>
      weak_transitions_;
};
}  // namespace regex