#pragma once

#include <exception>
#include <map>
#include <vector>

namespace regex {
class graph {
 public:
  using edge_label = char;
  using vertex = std::vector<int>::size_type;
  using edges_type = std::multimap<edge_label, vertex>;
  /*
   * Construct an empty graph
   */
  explicit graph() = default;
  explicit graph(const graph&) = delete;
  explicit graph(graph&&) = delete;
  /*
   * Add a a new vertex to the graph and return its assigned label
   */
  vertex add_vertex();
  /*
   * Connect two existing vertices with label, throw if either do not already exist
   */
  void add_edge(vertex from, vertex to, edge_label label);
  /*
   * Get the edges from vertex v
   */
  const edges_type& get_edges(vertex v);

 private:
  std::vector<edges_type> vertices_;
};

graph::vertex graph::add_vertex() {
  vertices_.push_back({});
  return vertices_.size() - 1;
}

void graph::add_edge(vertex from, vertex to, edge_label label) {
  if (from >= vertices_.size() || to >= vertices_.size()) {
    throw std::exception();
  } else {
    vertices_[from].emplace(label, to);
  }
}

const graph::edges_type& graph::get_edges(vertex v) {
  if (v >= vertices_.size()) {
    throw std::exception();
  } else {
    return vertices_[v];
  }
}
}  // namespace regex