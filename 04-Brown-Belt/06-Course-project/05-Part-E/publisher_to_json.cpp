#include "publisher_to_json.h"

using namespace std;

void publicate_to_json(const RequestStatBus& bus_request, json::Node& node, int req_id) {
    
    using namespace json;
    if (bus_request.bus_stats.has_value()) {
        const auto& bus_stats = bus_request.bus_stats.value();
        node = Map{{"request_id", req_id},
                   {"stop_count", bus_stats.stops_count},
                   {"unique_stop_count", bus_stats.unique_stops_count},
                   {"route_length", bus_stats.route_length},
                   {"curvature", bus_stats.curvature}
        };
        return;
    }
    node = Map{{"request_id", req_id}, {"error_message", string("not found")}};
     
}

void publicate_to_json(const RequestStatStop& stop_request, json::Node& node, int req_id) {
    using namespace json;
    if (stop_request.bus_name_set.has_value()) {
        const auto& bus_name_set = stop_request.bus_name_set.value();
        Array buses;
        for (const auto bus_name : bus_name_set) {
            buses.push_back(string(bus_name));
        }
        node = Map{{"request_id", req_id}, {"buses", buses}};
        return;
    }
    node = Map{{"request_id", req_id}, {"error_message", string("not found")}};
}

void publicate_to_json(const std::optional<RequestRouteResult>& route_request, json::Node& node, int req_id) {
    using namespace json;
    if (route_request.has_value()) {
        const auto& route_items = route_request.value().items;
        Array node_array;
        
        for (const auto& route_item : route_items) {
            if (std::holds_alternative<RequestRouteWaitItem>(route_item)) {
                const auto& wait_item = get<RequestRouteWaitItem>(route_item);
                
                node_array.emplace_back(Map({
                    {"type", string("Wait")},
                    {"stop_name", string(wait_item.stop_name)},
                    {"time", wait_item.time}
                }));
            } else {
                const auto& bus_item = get<RequestRouteBusItem>(route_item);
                
                node_array.emplace_back(Map({
                    {"type", string("Bus")},
                    {"bus", string(bus_item.bus_name)},
                    {"time", bus_item.time},
                    {"span_count", bus_item.span_count}
                }));
            }
        }
        
        node = Map{
            {"items", std::move(node_array)},
            {"request_id", req_id},
            {"total_time", route_request.value().total_time}
        };
    } else {
        node = Map{{"request_id", req_id}, {"error_message", string("not found")}};
    }
}

