#include "automata/nfa.h"

#include <memory>
#include <string_view>

#include "language/ast.h"
#include "state/state.h"

namespace regex {
nfa::nfa(std::shared_ptr<nstate> input, std::shared_ptr<nstate> output)
    : input_(input), output_(output) {}

std::shared_ptr<nfa> nfa::from_character(language::character_type c) {
  auto input = std::make_shared<nstate>(state::context::rejecting);
  auto output = std::make_shared<nstate>(state::context::accepting);

  input->connect(c, output);

  return std::make_shared<nfa>(input, output);
}

std::shared_ptr<nfa> nfa::from_epsilon() {
  return from_character(epsilon);
}

std::shared_ptr<nfa> nfa::from_any() {
  auto input = std::make_shared<nstate>(state::context::rejecting);
  auto output = std::make_shared<nstate>(state::context::accepting);

  for (const auto letter : language::alphabet) {
    input->connect(letter, output);
  }

  return std::make_shared<nfa>(input, output);
}

std::shared_ptr<nfa> nfa::from_concatenation(std::shared_ptr<nfa> lhs, std::shared_ptr<nfa> rhs) {
  lhs->output_->connect(epsilon, rhs->input_);

  lhs->output_->set(state::context::rejecting);
  rhs->output_->set(state::context::accepting);

  return std::make_shared<nfa>(lhs->input_, rhs->output_);
}

std::shared_ptr<nfa> nfa::from_alternation(std::shared_ptr<nfa> lhs, std::shared_ptr<nfa> rhs) {
  std::shared_ptr<nstate> input = std::make_shared<nstate>(state::context::rejecting);
  std::shared_ptr<nstate> output = std::make_shared<nstate>(state::context::accepting);

  input->connect(epsilon, lhs->input_);
  input->connect(epsilon, rhs->input_);

  lhs->output_->connect(epsilon, output);
  lhs->output_->set(state::context::rejecting);
  rhs->output_->connect(epsilon, output);
  rhs->output_->set(state::context::rejecting);

  return std::make_shared<nfa>(input, output);
}

std::shared_ptr<nfa> nfa::from_kleene(std::shared_ptr<nfa> expression) {
  std::shared_ptr<nstate> input = std::make_shared<nstate>(state::context::rejecting);
  std::shared_ptr<nstate> output = std::make_shared<nstate>(state::context::accepting);

  expression->output_->set(state::context::rejecting);

  input->connect(epsilon, expression->input_);
  input->connect(epsilon, output);
  expression->output_->connect(epsilon, output);
  output->connect(epsilon, std::weak_ptr<nstate>(expression->input_));

  return std::make_shared<nfa>(input, output);
}

void nfa::walk(std::function<void(std::shared_ptr<nstate>)> callback) {
  input_->walk(callback);
}

match nfa::execute(std::basic_string_view<language::character_type> target) {
  return input_->execute(target);
}
}  // namespace regex
