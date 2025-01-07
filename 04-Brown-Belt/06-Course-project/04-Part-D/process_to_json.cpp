#include <sstream>
#include <iomanip>

#include "process_to_json.h"

using namespace std;

void process_to_json(const BusCatalog &bus_catalog, json::Array& requests, json::Array& responses) {
    
    for (const auto& request : requests) {
        const auto& request_map = request.AsMap();
        int response_id = request_map.at("id").AsInt();
        json::Node response;
        if (request_map.at("type").AsString() == "Bus") {
            publicate_to_json(
                              get_bus_stats(bus_catalog, request_map.at("name").AsString()),
                              response,
                              response_id
                              );
        } else if (request_map.at("type").AsString() == "Stop") {
            publicate_to_json(
                                get_stop_stats(bus_catalog, request_map.at("name").AsString()),
                                response,
                                response_id
                                );
        }
        responses.push_back(std::move(response));
    }
}
