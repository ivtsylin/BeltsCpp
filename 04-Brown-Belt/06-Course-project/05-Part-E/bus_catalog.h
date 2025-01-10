#pragma once

#include <unordered_map>
#include <string>
#include <utility>
#include <memory>
#include <list>

#include "stop.h"
#include "bus.h"

class BusCatalog {
private:
    std::list<Stop> stops;
    std::list<Bus> buses;
    
    std::unordered_map<std::string_view, Stop*> stops_indexer;
    std::unordered_map<std::string_view, Bus*> buses_indexer;
    
public:
    
    Stop& get_or_insert_stop(std::string_view name) {
        
        auto it = stops_indexer.find(name);
        if (it == stops_indexer.end()) {
            stops.emplace_back(std::string(name));
            stops_indexer[stops.back().get_name()] = &stops.back();
            return stops.back();
        }
        
        return *(it->second);
    }
    
    Stop& add_stop(const std::string& name, double lat, double lon) = delete;
    
    Stop& add_stop(std::string&& name, double lat, double lon) {
        
        auto it = stops_indexer.find(name);
        if (it == stops_indexer.end()) {
            stops.emplace_back(std::move(name), lat, lon);
            stops_indexer[stops.back().get_name()] = &stops.back();
            return stops.back();
        }
        
        it->second->set_point({lat, lon});
        return *(it->second);
    }
    
    Bus& add_bus(const std::string &name, ROUTE_TYPE rt, std::vector<Stop*> &&route) = delete;
    Bus& add_bus(std::string &&name, ROUTE_TYPE rt, const std::vector<Stop*> &route) = delete;
    
    Bus& add_bus(std::string &&name, ROUTE_TYPE rt, std::vector<Stop*> &&route) {
        buses.emplace_back(std::move(name), rt, std::move(route));
        buses_indexer[buses.back().get_name()] = &buses.back();
        
        return buses.back();
    }
    
    std::optional<const Bus*> get_bus(std::string_view bus_name) const {
        if(const auto it = buses_indexer.find(bus_name); it != buses_indexer.end()) {
            return it->second;
        }
        return std::nullopt;
    }
    std::optional<const Stop*> get_stop(std::string_view stop_name) const {
        if(const auto it = stops_indexer.find(stop_name); it != stops_indexer.end()) {
            return it->second;
        }
        return std::nullopt;
    }
    
    const auto& get_stops() const {
        return stops;
    }
    const auto& get_buses() const {
        return buses;
    }
};
