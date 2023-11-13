#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <set>
#include <vector>
#include <unordered_map>
#include <optional>
#include <cmath>
#include "test_runner.h"

using namespace std;

string STRING_FOR_STEPIC;

pair<string_view, optional<string_view>> SplitTwoStrict(string_view s, string_view delimiter = " ") {
  const size_t pos = s.find(delimiter);
  if (pos == s.npos) {
    return {s, nullopt};
  } else {
    return {s.substr(0, pos), s.substr(pos + delimiter.length())};
  }
}

pair<string_view, string_view> SplitTwo(string_view s, string_view delimiter = " ") {
  const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
  return {lhs, rhs_opt.value_or("")};
}

string_view ReadToken(string_view& s, string_view delimiter = " ") {
  const auto [lhs, rhs] = SplitTwo(s, delimiter);
  s = rhs;
  return lhs;
}

int ConvertToInt(string_view str) {
  // use std::from_chars when available to git rid of string copy
  size_t pos;
    int result = 0;
    result = stoi(string(str), &pos);
  if (pos != str.length()) {
    std::stringstream error;
    error << "string " << str << " contains " << (str.length() - pos) << " trailing chars";
    throw invalid_argument(error.str());
  }
  return result;
}

double ConvertToDouble(string_view str) {
  // use std::from_chars when available to git rid of string copy
  size_t pos;
  const double result = stod(string(str), &pos);
  if (pos != str.length()) {
    std::stringstream error;
    error << "string " << str << " contains " << (str.length() - pos) << " trailing chars";
    throw invalid_argument(error.str());
  }
  return result;
}

struct SphericalCoordinates {
    double lat;
    double lon;
};

ostream& operator<<(ostream& output, const SphericalCoordinates &SphericalCoordinates) {
    output << SphericalCoordinates.lat << ", " << SphericalCoordinates.lon;
    return output;
}

constexpr double PI = 3.1415926535;

double compute_distance(SphericalCoordinates lhs, SphericalCoordinates rhs) {
    return acos(sin(lhs.lat * PI / 180.) * sin(rhs.lat * PI / 180.) +
                cos(lhs.lat * PI / 180.) * cos(rhs.lat * PI / 180.) *
                cos(abs(lhs.lon * PI / 180. - rhs.lon * PI / 180.))
               ) * 6'371'000;
}

struct StopInfo {
private:
    SphericalCoordinates coordinates;
    set<string> buses_names;
    unordered_map<string, double> distance_data;
public:
    StopInfo operator=(StopInfo) = delete;
    const SphericalCoordinates& get_coordinates() const {
        return coordinates;
    }
    const auto& get_distance_data() const {
        return distance_data;
    }
    void read_coordinates(string_view& str_in) {
        coordinates.lat = ConvertToDouble(ReadToken(str_in, ", "));
        coordinates.lon = ConvertToDouble(ReadToken(str_in, ", "));
    }
    void set_coordinates(const SphericalCoordinates& rhs) {
        coordinates = rhs;
    }
    const auto& get_buses_names() const {
        return buses_names;
    }
    void add_bus_name(const string& bus_name) {
        buses_names.insert(bus_name);
    }
    void add_distance_to_stop(const string& stop_name, double distance) {
        distance_data[stop_name] = distance;
    }
    void assume_distance_to_stop(const string& stop_name, double distance) {
        if (!distance_data.count(stop_name))
            distance_data[stop_name] = distance;
    }
    optional<double> get_distance_to_stop(const string& stop_name) const {
        auto it = distance_data.find(stop_name);
        if (it == distance_data.end()) {
            return nullopt;
        }
        return it->second;
    }
};

ostream& operator<<(ostream& output, const StopInfo &stop) {
    output << "[" << "{" << stop.get_coordinates() << "}" << stop.get_buses_names() << "map=" << stop.get_distance_data() << "]";
    return output;
}

class Stops {
private:
    unordered_map<string, StopInfo> data;
public:
    Stops operator=(Stops) = delete;
    const auto& get_data() const {
        return data;
    }
    void add_stop_bus_name (const string &stop_name, const string& bus_name) {
        data[stop_name].add_bus_name(bus_name);
    }
    void read_stop_and_coordinates (string_view& str_in) {
        string_view stop_name_vw = ReadToken(str_in, ": ");
        string stop_name = string(stop_name_vw);
        data[stop_name].read_coordinates(str_in);
        if (str_in != "") {
            string_view token = ReadToken(str_in, ", ");
            while (token != "") {
                double distance = ConvertToDouble(ReadToken(token, "m to "));
                string stop_name_rhs = string(token);
                data[stop_name].add_distance_to_stop(stop_name_rhs, distance);
                data[stop_name_rhs].assume_distance_to_stop(stop_name, distance);
                token = ReadToken(str_in, ", ");
            }
        }
    }
    optional<const set<string>*> find_stop_and_get_buses(const string& stop_name) const {
        auto it = data.find(stop_name);
        if (it == data.end()) {
            return nullopt;
        }
        return &it->second.get_buses_names();
    }
    optional<const SphericalCoordinates*> find_stop_and_get_coordinates(const string &stop_name) const {
        auto it = data.find(stop_name);
        if (it == data.end()) {
            return nullopt;
        }
        return &it->second.get_coordinates();
    }
    optional<double> find_real_distance(const string &stop_name_from, const string &stop_name_to) const {
        auto it_from = data.find(stop_name_from);
        if (it_from == data.end()) {
            return nullopt;
        }
        return it_from->second.get_distance_to_stop(stop_name_to);
    }
};


ostream& operator<<(ostream& s_out, const Stops& stops) {
    s_out << stops.get_data();
    return s_out;
}

enum class ROUT_TYPE {
    CIRCLE,
    TWO_WAY
};

class BusRoute {
private:
    vector<string> data;
    ROUT_TYPE rt;
    int stops_count;
    int unique_stops_count;
    mutable optional<double> route_geo_length;
    mutable optional<double> route_real_length;
    mutable optional<double> curvature;
    template <class T>
    void add_stop_without_recalc (const string& stop_name, T apply_functor) {
        apply_functor(stop_name);
        data.emplace_back(stop_name);
    };
    void compute_length_and_curvature(const Stops& stops) const {
        if (data.size() <= 1) {
            route_geo_length = 0.0;
            route_real_length = 0.0;
            curvature = 1.0;
            return;
        }
        
        double geo_length = 0.0;
        double real_length = 0.0;
        
        const SphericalCoordinates *ppoint_cur = stops.find_stop_and_get_coordinates(data[0]).value();
        const SphericalCoordinates *ppoint_prev = nullptr;

        for (uint32_t i = 0; i + 1 < data.size(); ++i) {
            ppoint_prev = ppoint_cur;
            ppoint_cur = stops.find_stop_and_get_coordinates(data[i + 1]).value();
            auto real_distance = stops.find_real_distance(data[i], data[i + 1]);
            double geo_distance = compute_distance(*ppoint_prev, *ppoint_cur);
            
            geo_length += (rt == ROUT_TYPE::TWO_WAY ? 2 * geo_distance : geo_distance);
            
            if (real_distance) {
                real_length += real_distance.value();
                if (rt == ROUT_TYPE::TWO_WAY)
                    real_length += stops.find_real_distance(data[i + 1], data[i]).value();
            } else {
                real_length += (rt == ROUT_TYPE::TWO_WAY ? 2 * geo_distance : geo_distance);
            }

        }
        
        route_geo_length = geo_length;
        route_real_length = real_length;
        curvature = real_length / geo_length;
    }
    
public:
    BusRoute operator=(BusRoute a) = delete;
    int get_stops_count() const {
        return stops_count;
    }
    int get_unique_stops_count() const {
        return unique_stops_count;
    }
    double get_geo_length(const Stops& stops) const {
        if (route_geo_length)
            return route_geo_length.value();
        
        compute_length_and_curvature(stops);
        return route_geo_length.value();
    }
    double get_real_length(const Stops& stops) const {
        if (route_real_length)
            return route_real_length.value();
        
        compute_length_and_curvature(stops);
        return route_real_length.value();
    }
    double get_curvature(const Stops& stops) const {
        if (curvature)
            return curvature.value();
        
        compute_length_and_curvature(stops);
        return curvature.value();
    }
    const auto& get_data() const {
        return data;
    }
    template<class T>
    void read_bus_route (string_view &str_in, T apply_functor) {
        if(data.size()) {
            throw invalid_argument("Initialization twice");
        }
        set<string_view> count_set;
        string delimiter = " - ";
        rt = ROUT_TYPE::TWO_WAY;
        if (str_in.find('-') == string::npos) { // CIRCLE
            rt = ROUT_TYPE::CIRCLE;
            delimiter = " > ";
        }
        string_view stop_name = ReadToken(str_in, delimiter);
        while (str_in != "") {
            add_stop_without_recalc(string(stop_name), apply_functor);
            count_set.insert(stop_name);
            stop_name = ReadToken(str_in, delimiter);
        }
        if (rt == ROUT_TYPE::CIRCLE) { // CIRCLE
            add_stop_without_recalc(data[0], apply_functor);
            stops_count = data.size();
        } else { // TWO_WAY
            add_stop_without_recalc(string(stop_name), apply_functor);
            count_set.insert(stop_name);
            stops_count = 2 * data.size() - 1;
        }
        unique_stops_count = count_set.size();
    }
};

ostream& operator<<(ostream& s_out, const BusRoute& bus_route) {
    s_out << "<" << bus_route.get_data() << ">";
    return s_out;
}

struct BusStats {
    int stops_count;
    int unique_stops_count;
    double route_real_length;
    double curvature;
};

class Buses {
private:
    unordered_map<string, BusRoute> data;
public:
    Buses operator=(Buses) = delete;
    const auto& get_data() const {
        return data;
    }
    
    void read_bus_and_bus_route (string_view& str_in, Stops& stops) {
        string_view bus_name_vw = ReadToken(str_in, ": ");
        string bus_name = string(bus_name_vw);
        class AddToStopInfoAboutBus {
        private:
            Stops& stops;
            string_view bus_name;
        public:
            AddToStopInfoAboutBus(Stops& stops_, string_view bus_name_) : stops(stops_), bus_name(bus_name_) {};
            void operator()(const string& stop_name) {
                stops.add_stop_bus_name(stop_name, string(bus_name));
            }
        } apply_functor(stops, bus_name_vw);
        data[move(bus_name)].read_bus_route(str_in, apply_functor);
    }
    optional<const BusRoute*> find_bus_and_get_bus_route(string_view bus_name) const {
        return find_bus_and_get_bus_route(string(bus_name));
    }
    optional<const BusRoute*> find_bus_and_get_bus_route(const string &bus_name) const {
        auto it = data.find(bus_name);
        if (it == data.end()) {
            return nullopt;
        }
        return &it->second;
    }
    optional<BusStats> get_bus_stats(string_view bus_name, const Stops& stops) const {
        return get_bus_stats(string(bus_name), stops);
    }
    optional<BusStats> get_bus_stats(const string &bus_name, const Stops& stops) const {
        if (auto it = data.find(bus_name); it != data.end()) {
            return BusStats{
                .stops_count = it->second.get_stops_count(),
                .unique_stops_count = it->second.get_unique_stops_count(),
                .route_real_length = it->second.get_real_length(stops),
                .curvature = it->second.get_curvature(stops)
            };
        }
        return nullopt;
    }
};

ostream& operator<<(ostream& s_out, const Buses& buses) {
    s_out << buses.get_data();
    return s_out;
}

enum class COMMAND {
    STOP,
    BUS
};

class BusManager {
private:
    Buses buses;
    Stops stops;
public:
    void read_bus_catalog() {
        string line;
        string_view subline, token;
        int count;
        
        getline(cin, line);
        STRING_FOR_STEPIC = line;
        subline = line;
        count = ConvertToInt(ReadToken(subline));
        
        for (int i = 0; i < count; ++i) {
            getline(cin, line);
            subline = line;
            token = ReadToken(subline);
            if (token == "Stop") {
                stops.read_stop_and_coordinates(subline);
            } else if (token == "Bus") {
                buses.read_bus_and_bus_route(subline, stops);
            } else {
                stringstream error;
                error << "string *" << line << "* can not be recognized\n";
                throw invalid_argument(error.str());
            }
            //cout << "STOPS: " << stops << endl;
            //cout << "BUSES: " << buses << endl;
        }
        
    }
    void process_and_print_requests() {
        string line;
        string_view subline, token;
        int count;
        string bus_name, stop_name;
        
        getline(cin, line);
        STRING_FOR_STEPIC = line;
        subline = line;
        count = ConvertToInt(ReadToken(subline));
        for (int i = 0; i < count; ++i) {
            getline(cin, line);
            subline = line;
            token = ReadToken(subline);
            if (token == "Bus") {
                cout << line << ": ";
                bus_name = subline;
                
                auto bus_stats = buses.get_bus_stats(bus_name, stops);
                if (bus_stats) {
                    cout << bus_stats.value().stops_count << " stops on route, " << bus_stats.value().unique_stops_count << " unique stops, " << bus_stats.value().route_real_length << " route length, " << bus_stats.value().curvature << " curvature" << endl;
                } else {
                    cout << "not found" << endl;
                }
            } else if (token == "Stop") {
                cout << line << ": ";
                stop_name = subline;
                auto stop_request_result = stops.find_stop_and_get_buses(stop_name);
                if (stop_request_result) {
                    if (stop_request_result.value()->empty()) {
                        cout << "no buses" << endl;
                    } else {
                        cout << "buses";
                        for (auto it = stop_request_result.value()->begin(); it != stop_request_result.value()->end(); ++it) {
                            cout << " " << *it;
                        }
                        cout << endl;
                    }
                } else {
                    cout << "not found" << endl;
                }
            } else {
                stringstream error;
                error << "string *" << line << "* can not be recognized\n";
                throw invalid_argument(error.str());
            }
        }
    }
};

void UnitTest() {
}

int main() {
    TestRunner tr;
    //RUN_TEST(tr, UnitTest);
    try {
        cout << fixed << setprecision(6);
        BusManager bus_manager;
        bus_manager.read_bus_catalog();
        bus_manager.process_and_print_requests();
    } catch (...) {
        cerr << STRING_FOR_STEPIC << endl;
        throw invalid_argument(STRING_FOR_STEPIC);
    }
    return 0;
}
