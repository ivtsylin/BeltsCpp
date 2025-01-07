#include <sstream>

#include "reader_from_json.h"
#include "bus_catalog.h"

using namespace std;

void read_stop(BusCatalog &bus_catalog, const json::Map& node_map) {
  
    //Пример
    //{
    //  "type": "Stop",
    //  "name": "Tolstopaltsevo",
    //  "latitude": 55.611087,
    //  "longitude": 37.20829,
    //  "road_distances": {
    //      "Marushkino": 3900
    //  }
    //}
    
    // Stop& add_stop(std::string&& name, double lat, double lon)

    Stop& stop = bus_catalog.add_stop(
                                      string(node_map.at("name").AsString()),
                                      node_map.at("latitude").AsDouble(),
                                      node_map.at("longitude").AsDouble()
                         );
    
    if (auto it = node_map.find("road_distances"); it != node_map.end() && !it->second.AsMap().empty()) {
        for (const auto& [stop_name_str, stop_dst_node] : it->second.AsMap()) {
            double distance = stop_dst_node.AsDouble();
            Stop& dst_stop = bus_catalog.get_or_insert_stop(stop_name_str);
            stop.add_distance_to_stop(dst_stop, distance);
            dst_stop.assume_distance_to_stop(stop, distance);
        }
    }
}

void read_bus(BusCatalog &bus_catalog, const json::Map& node_map) {
    
    
    //Пример
    //{
    //"type": "Bus",
    //"name": "256",
    //"stops": [
    //  "Biryulyovo Zapadnoye",
    //  "Biryusinka",
    //  "Universam",
    //  "Biryulyovo Tovarnaya",
    //  "Biryulyovo Passazhirskaya",
    //  "Biryulyovo Zapadnoye"
    //],
    //"is_roundtrip": true
    //}
    
    ROUTE_TYPE rt = node_map.at("is_roundtrip").AsBool() ? ROUTE_TYPE::CIRCLE : ROUTE_TYPE::TWO_WAY;
    
    const auto& node_route = node_map.at("stops").AsArray();
    
    vector<Stop*> route;
    for (const auto& node_stop_name : node_route) {
        route.push_back(&bus_catalog.get_or_insert_stop(node_stop_name.AsString()));
    }
    
    bus_catalog.add_bus(string(node_map.at("name").AsString()), rt, move(route));
}

void read_bus_catalog_from_json(BusCatalog &bus_catalog, const json::Array& node_array) {
    
    for (const auto& node_req : node_array) {
        const string& type = node_req.AsMap().at("type").AsString();
        if (type == "Stop") {
            read_stop(bus_catalog, node_req.AsMap());
        } else if (type == "Bus") {
            read_bus(bus_catalog, node_req.AsMap());
        }
    }
}

