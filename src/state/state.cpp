#include "state/state.h"

namespace regex {
state::state(state::context ctx) : ctx_(ctx) {}

state::context state::get_type() const {
  return ctx_;
}

void state::set(state::context ctx) {
  ctx_ = ctx;
}
}  // namespace regex