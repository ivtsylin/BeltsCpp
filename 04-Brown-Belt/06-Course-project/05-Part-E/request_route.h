#pragma once

#include <optional>
#include <variant>
#include <vector>
#include <unordered_map>
#include <memory>

#include "bus_catalog.h"
#include "graph.h"
#include "router.h"

struct RequestRouteBusItem {
    std::string_view bus_name;
    double time;
    int span_count;
};

struct RequestRouteWaitItem {
    std::string_view stop_name;
    double time;
};

using RequestRouteItem = std::variant<RequestRouteBusItem, RequestRouteWaitItem>;

class RequestRoute;

struct RequestRouteResult {
    
    class RequestRouteIterator {
    private:
        size_t i;
        const RequestRoute& request_route;
        
    public:
        RequestRouteIterator(const RequestRoute& request_route_, int i_ = 0) : request_route(request_route_), i(i_) {};
        
        bool operator==(const RequestRouteIterator& lhs) const {
            return i == lhs.i;
        }
        bool operator!=(const RequestRouteIterator& lhs) const {
            return i != lhs.i;
        }
        RequestRouteItem operator*() const;
        
        RequestRouteIterator& operator++() {
            ++i;
            return *this;
        }
    };
    
    class RequestRouteItems {
    private:
        const RequestRoute& request_route;
        
    public:
        RequestRouteItems(const RequestRoute& request_route_) : request_route(request_route_) {};
        
        const RequestRouteIterator begin() const{
            return RequestRouteIterator(request_route);
        }
        const RequestRouteIterator end() const;
    };
    
    RequestRouteItems items;
    double total_time;
};

class RequestRoute {
    
    friend class RequestRouteResult::RequestRouteItems;
    friend class RequestRouteResult::RequestRouteIterator;
    
private:
    const BusCatalog& bus_catalog;
    double bus_wait_time;
    double bus_velocity;
    Graph::DirectedWeightedGraph<double> edges;
    std::unique_ptr<Graph::Router<double>> p_router;
    std::optional<Graph::Router<double>::RouteInfo> result;
    
    enum class EXCHANGE_TYPE {
        BUS,
        WAIT
    };
    
    struct EdgeInfo {
        EXCHANGE_TYPE e_type = EXCHANGE_TYPE::WAIT;
        std::string_view bus_name;
        int span_count = 0;
    };
    
    using VertexId = size_t;
    std::vector<const Stop*> vertex_indeces;
    std::unordered_map<std::string_view, VertexId> terminal_vertex_indeces;
    std::unordered_map<Graph::EdgeId, EdgeInfo> edge_info;
    
    
    
    void init_graph() {
        
        for (const auto& stop : bus_catalog.get_stops()) {
            uint32_t i = vertex_indeces.size();
            vertex_indeces.push_back(&stop);
            terminal_vertex_indeces[stop.get_name()] = i;
            vertex_indeces.push_back(&stop);
            auto edge_id = edges.AddEdge({i, i + 1, bus_wait_time});
            edge_info[edge_id] = {EXCHANGE_TYPE::WAIT};
        }
        
        for (const auto& bus : bus_catalog.get_buses()) {
            const auto& bus_route = bus.get_route();
            for (int i = 0; i + 1 < bus_route.size(); ++i) {
                double weight = 0.;
                for (int j = i + 1; j < bus_route.size(); ++j) {
                    const Stop& stop_prev = *bus_route[j - 1];
                    const Stop& stop_cur = *bus_route[j];
                    weight += stop_prev.get_real_distance(stop_cur) / 1000. / bus_velocity * 60;
                    
                    auto edge_id = edges.AddEdge({
                        terminal_vertex_indeces[bus_route[i]->get_name()] + 1,
                        terminal_vertex_indeces[bus_route[j]->get_name()],
                        weight
                    });
                    edge_info[edge_id] = {EXCHANGE_TYPE::BUS, bus.get_name(), j - i};
                }
            }
        }
        
        p_router = std::make_unique<Graph::Router<double>>(edges);
    }
    
public:
    
    RequestRoute(const BusCatalog& bus_catalog_, double bus_wait_time_, double bus_velocity_) :
    bus_catalog(bus_catalog_),
    bus_wait_time(bus_wait_time_),
    bus_velocity(bus_velocity_),
    edges(bus_catalog.get_stops().size() * 2)
    {
        init_graph();
    };
    
    std::optional<RequestRouteResult> get_route(std::string_view stop_from, std::string_view stop_to) {
        
        if (result) {
            const auto& result_struct = result.value();
            p_router->ReleaseRoute(result_struct.id);
        }
        
        result = p_router->BuildRoute(
                                      terminal_vertex_indeces[stop_from],
                                      terminal_vertex_indeces[stop_to]
                                      );
        
        if (result) {
            return RequestRouteResult{
                .items = RequestRouteResult::RequestRouteItems(*this),
                .total_time = result.value().weight
            };
        }
        return std::nullopt;
    }
};




