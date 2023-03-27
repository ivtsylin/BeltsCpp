//
//  responses.hpp
//  YellowBelt
//
//  Created by Ivan Tsylin on 18.02.2023.
//

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <utility>

using namespace std;

struct BusesForStopResponse {
    vector<string> stops_to_buses;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r);

struct StopsForBusResponse {
    // Наполните полями эту структуру
    vector<pair<string, vector<string>>> stops_to_buses_list;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r);

struct AllBusesResponse {
    // Наполните полями эту структуру
    vector<pair<string, vector<string>>> buses_to_stop_list;
};

ostream& operator << (ostream& os, const AllBusesResponse& r);
