#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string_view>
#include <vector>

#include "language/ast.h"
#include "state/state.h"

namespace regex {
class dstate : public state, public std::enable_shared_from_this<dstate> {
 public:
  explicit dstate(state::context ctx = state::context::rejecting);
  explicit dstate(const dstate&);
  explicit dstate(dstate&&) = delete;

  using transitions_type = std::map<language::character_type, std::shared_ptr<dstate>>;
  /*
   * Connect src state to dest state via symbol, any pre-existing transition is
   * overwritten
   */
  static void connect(std::shared_ptr<dstate> src,
                      std::shared_ptr<dstate> dest,
                      regex::language::character_type symbol);
  /*
   * Merge src state into target state, all src transitions are transferred to
   * target, if target has an existing transition for a src symbol then the
   * child states are merged recursively to completion Src is left in an invalid
   * state
   */
  static void merge(
      std::shared_ptr<dstate> src,
      std::shared_ptr<dstate> target,
      std::set<std::shared_ptr<dstate>> visited = std::set<std::shared_ptr<dstate>>());
  /*
   * Shallow copy src transitions to target leaving src unchanged
   * If target has an existing transition of a src symbol then the child states
   * are copied recursively to completion
   */
  static void copy(std::shared_ptr<const dstate> src,
                   std::shared_ptr<dstate> target,
                   std::set<std::shared_ptr<dstate>> visited = std::set<std::shared_ptr<dstate>>());
  /*
   * Create an identical state machine, deep copying src
   */
  static std::shared_ptr<dstate> duplicate(std::shared_ptr<const dstate> src);
  /*
   * Get transition for the symbol
   */
  const transitions_type& get_transitions() const;

  void walk(std::function<void(std::shared_ptr<dstate>)> callback,
            std::set<std::shared_ptr<dstate>> visited = std::set<std::shared_ptr<dstate>>());

  match execute(std::basic_string_view<language::character_type> str);

 private:
  transitions_type transitions_;
};
}  // namespace regex