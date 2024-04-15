#pragma once

#include <cstdlib>
#include <deque>
#include <vector>

template <typename It>
class Range {
public:
  using ValueType = typename std::iterator_traits<It>::value_type;

  Range(It begin, It end) : begin_(begin), end_(end) {}
  It begin() const { return begin_; }
  It end() const { return end_; }

private:
  It begin_;
  It end_;
};

namespace Graph {

  using VertexId = size_t;
  using EdgeId = size_t;

  template <typename Weight>
  struct Edge {
    VertexId from;
    VertexId to;
    Weight weight;
  };



  template <typename Weight>
  class DirectedWeightedGraph {
  private:
      using IncidenceList = std::vector<EdgeId>;
      using IncidentEdgesRange = Range<typename IncidenceList::const_iterator>;
      
      std::vector<IncidenceList> incidence_lists_;

  public:
      std::vector<Edge<Weight>> edges_;
      DirectedWeightedGraph(size_t vertex_count) : incidence_lists_(vertex_count) {};
      EdgeId AddEdge(const Edge<Weight>& edge) {
          edges_.push_back(edge);
          const EdgeId id = edges_.size() - 1;
          incidence_lists_[edge.from].push_back(id);
          return id;
      }

      size_t GetVertexCount() const {
          return incidence_lists_.size();
      }
      size_t GetEdgeCount() const {
          return edges_.size();
      }
      const Edge<Weight>& GetEdge(EdgeId edge_id) const {
          return edges_[edge_id];
      }
      IncidentEdgesRange GetIncidentEdges(VertexId vertex) const {
          const auto& edges = incidence_lists_[vertex];
          return {std::begin(edges), std::end(edges)};
      }
  };
}
