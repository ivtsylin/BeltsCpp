//
//  query.hpp
//  YellowBelt
//
//  Created by Ivan Tsylin on 18.02.2023.
//

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

enum class QueryType {
  NewBus,
  BusesForStop,
  StopsForBus,
  AllBuses
};

struct Query {
  QueryType type;
  string bus;
  string stop;
  vector<string> stops;
};

istream& operator >> (istream& is, Query& q);
