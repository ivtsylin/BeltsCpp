#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <tuple>
#include <utility>
#include <cassert>

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

istream& operator >> (istream& is, Query& q) {
    string operation_code;
    is >> operation_code;
    if (operation_code == "NEW_BUS") {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int stop_count;
        is >> stop_count;
        q.stops.resize(stop_count);
        for (string& stop : q.stops) {
            is >> stop;
        }
    } else if (operation_code == "BUSES_FOR_STOP") {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    } else if (operation_code == "STOPS_FOR_BUS") {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    } else {
        q.type = QueryType::AllBuses;
    }
  return is;
}


struct BusesForStopResponse {
    vector<string> stops_to_buses;
};

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

struct StopsForBusResponse {
    // Наполните полями эту структуру
    vector<pair<string, vector<string>>> stops_to_buses_list;
};

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

struct AllBusesResponse {
    // Наполните полями эту структуру
    vector<pair<string, vector<string>>> buses_to_stop_list;
};

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

class BusManager {
private:
    map<string, vector<string>> buses_to_stops;
    map<string, vector<string>> stops_to_buses;
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        buses_to_stops[bus] = stops;
        for (const string& stop : stops) {
            stops_to_buses[stop].push_back(bus);
        }
    };

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        // Реализуйте этот метод
        BusesForStopResponse empty;
        if (stops_to_buses.count(stop) == 0) {
            return empty;
        }
        return {stops_to_buses.at(stop)};
    };

    StopsForBusResponse GetStopsForBus(const string& bus) const {
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

  AllBusesResponse GetAllBuses() const {
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
};

// Не меняя тела функции main, реализуйте функции и классы выше

int main() {
  int query_count;
  Query q;

  cin >> query_count;

  BusManager bm;
  for (int i = 0; i < query_count; ++i) {
    cin >> q;
    switch (q.type) {
    case QueryType::NewBus:
      bm.AddBus(q.bus, q.stops);
      break;
    case QueryType::BusesForStop:
      cout << bm.GetBusesForStop(q.stop) << endl;
      break;
    case QueryType::StopsForBus:
      cout << bm.GetStopsForBus(q.bus) << endl;
      break;
    case QueryType::AllBuses:
      cout << bm.GetAllBuses() << endl;
      break;
    }
  }

  return 0;
}


