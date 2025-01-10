#include <sstream>
#include <iomanip>

#include "process_to_json.h"

using namespace std;

void process_to_json(const BusCatalog &bus_catalog, json::Node& document, json::Node& responses) {
    
    json::Array result;
    const auto& requests = document.AsMap().at("stat_requests").AsArray();
    const auto& routing_settings = document.AsMap().at("routing_settings").AsMap();
    
    RequestStat request_stat(bus_catalog);
    RequestRoute request_route(
                               bus_catalog,
                               routing_settings.at("bus_wait_time").AsDouble(),
                               routing_settings.at("bus_velocity").AsDouble()
                               );
    
    
    for (const auto& request : requests) {
        const auto& request_map = request.AsMap();
        int response_id = request_map.at("id").AsInt();
        json::Node response;
        if (request_map.at("type").AsString() == "Bus") {
            publicate_to_json(
                              request_stat.get_bus_stats(request_map.at("name").AsString()),
                              response,
                              response_id
                              );
        } else if (request_map.at("type").AsString() == "Stop") {
            publicate_to_json(
                                request_stat.get_stop_stats(request_map.at("name").AsString()),
                                response,
                                response_id
                                );
        } else if (request_map.at("type").AsString() == "Route") {
            const string& stop_from = request_map.at("from").AsString();
            const string& stop_to = request_map.at("to").AsString();
            
            publicate_to_json(
                                request_route.get_route(stop_from, stop_to),
                                response,
                                response_id
                                );
        }
        result.push_back(std::move(response));
    }
    
    responses = std::move(result);
}
