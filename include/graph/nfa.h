#pragma once

#include <memory>
#include <stack>
#include <string_view>

#include "graph/graph.h"

namespace regex {
class nfa {
  graph::vertex input_;
  graph::vertex output_;
  graph graph_;

  static const char epsilon = 0x01;

 public:
  explicit nfa() = default;
  explicit nfa(const nfa& other) = delete;
  explicit nfa(nfa&& other) = delete;
  /*
   *
   *
   *      +---+      c     +---+
   *      | i |------>-----| o |
   *      +---+            +---+
   *
   *
   */
  static std::shared_ptr<nfa> from_character(char c) {
    auto new_nfa = std::make_shared<nfa>();
    new_nfa->input_ = new_nfa->graph_.add_vertex();
    new_nfa->output_ = new_nfa->graph_.add_vertex();
    new_nfa->graph_.add_edge(new_nfa->input_, new_nfa->output_, c);
    return new_nfa;
  }

  /*
   *
   *
   *      +---+            +---+      e     +---+            +---+
   *      | i |------>-----|   |------>-----|   |------>-----| o |
   *      +---+            +---+            +---+            +---+
   *
   *
   */
  static std::shared_ptr<nfa> from_concatenation(std::shared_ptr<nfa> lhs,
                                                 std::shared_ptr<nfa> rhs) {
                                                     
                                                 }

  bool execute(std::basic_string_view<char> target) {
    auto exe = [this](std::basic_string_view<char> str, graph::vertex v,
                      std::set<graph::vertex> visited, auto& exe) {
      if (visited.contains(v)) {
        return false;
      }

      visited.insert(v);

      if (str.empty()) {
        if (v == output_) {
          return true;
        } else {
          return false;
        }
      }

      for (auto next = graph_.get_edges(v).lower_bound(0x01);
           next != graph_.get_edges(v).upper_bound(0x01); next++) {
        if (exe(str, next->second, visited, exe)) {
          return true;
        }
      }

      for (auto next = graph_.get_edges(v).lower_bound(str[0]);
           next != graph_.get_edges(v).upper_bound(str[0]); next++) {
        if (exe(str.substr(1), next->second, visited, exe)) {
          return true;
        }
      }

      return false;
    };

    return exe(target, input_, {}, exe);
  }
};
}  // namespace regex
