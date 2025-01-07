#include "publisher_to_json.h"

using namespace std;

void publicate_to_json(const BusRequest& bus_request, json::Node& node, int req_id) {
    
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

void publicate_to_json(const StopRequest& stop_request, json::Node& node, int req_id) {
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
