//
//  responses.cpp
//  YellowBelt
//
//  Created by Ivan Tsylin on 18.02.2023.
//

#include "responses.h"

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
  // Реализуйте эту функцию
    if (r.stops_to_buses.size() == 0) {
        os << "No stop";
    } else {
        for (const string& bus : r.stops_to_buses) {
            os << bus << " ";
        }
    }
    return os;
}

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
    // Реализуйте эту функцию
    if (r.stops_to_buses_list.size() == 0) {
        os << "No bus";
    } else {
        bool first_out = true;
        for (const auto& [stop, stops_to_bus] : r.stops_to_buses_list) {
            if (first_out) {
                first_out = false;
            } else {
                os << endl;
            }
            os << "Stop " << stop << ": ";
            if (stops_to_bus.size() == 0) {
                os << "no interchange";
            } else {
                for (const string& bus : stops_to_bus) {
                    os << bus << " ";
                }
            }
        }
    }
    return os;
}

ostream& operator << (ostream& os, const AllBusesResponse& r) {
    // Реализуйте эту функцию
    if (r.buses_to_stop_list.size() == 0) {
        os << "No buses";
    } else {
        bool first_out = true;
        for (const auto& [bus, bus_item] : r.buses_to_stop_list) {
            if (first_out) {
                first_out = false;
            } else {
                os << endl;
            }
            os << "Bus " << bus << ": ";
            for (const string& stop : bus_item) {
                os << stop << " ";
            }
        }
    }
    return os;
}
