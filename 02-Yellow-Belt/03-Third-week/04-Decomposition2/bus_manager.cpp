//
//  bus_manager.cpp
//  YellowBelt
//
//  Created by Ivan Tsylin on 18.02.2023.
//

#include "bus_manager.h"

void BusManager::AddBus(const string& bus, const vector<string>& stops) {
    buses_to_stops[bus] = stops;
    for (const string& stop : stops) {
        stops_to_buses[stop].push_back(bus);
    }
};

BusesForStopResponse BusManager::GetBusesForStop(const string& stop) const {
    // Реализуйте этот метод
    BusesForStopResponse empty;
    if (stops_to_buses.count(stop) == 0) {
        return empty;
    }
    return {stops_to_buses.at(stop)};
};

StopsForBusResponse BusManager::GetStopsForBus(const string& bus) const {
    // Реализуйте этот метод
    StopsForBusResponse result;
    if (buses_to_stops.count(bus) == 0) {
        return result;
    }
    auto& resp = result.stops_to_buses_list;
    for (const auto &stop : buses_to_stops.at(bus)) {
        string resp_key = stop;
        vector<string> resp_value;
        for (const auto& other_bus : stops_to_buses.at(stop)) {
            if (other_bus != bus) {
                resp_value.push_back(other_bus);
            }
        }
        resp.push_back(make_pair(resp_key, resp_value));
    }
    return result;
};

AllBusesResponse BusManager::GetAllBuses() const {
    AllBusesResponse result;
    string resp_key;
    vector<string> resp_value;
    
    for (const auto& [bus, route] : buses_to_stops) {
        resp_key = bus;
        resp_value = route;
        result.buses_to_stop_list.push_back(make_pair(bus, route));
    }
    return result;
}
