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
#define TESTRUNNERTSYLIN

#ifdef TESTRUNNERTSYLIN
#include "test_runner.h"
#endif //TESTRUNNERTSYLIN

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

#ifdef TESTRUNNERTSYLIN
ostream& operator<<(ostream& output, const SphericalCoordinates &SphericalCoordinates) {
    output << SphericalCoordinates.lat << ", " << SphericalCoordinates.lon;
    return output;
}
#endif

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
public:
    StopInfo operator=(StopInfo) = delete;
    const SphericalCoordinates& get_coordinates() const {
        return coordinates;
    }
    void read_coordinates(string_view& str_in,
                          string_view delimiter = ", ") {
        coordinates.lat = ConvertToDouble(ReadToken(str_in, delimiter));
        coordinates.lon = ConvertToDouble(str_in);
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
};

#ifdef TESTRUNNERTSYLIN
ostream& operator<<(ostream& output, const StopInfo &stop) {
    output << "[" << "{" << stop.get_coordinates() << "}" << stop.get_buses_names() << "]";
    return output;
}
#endif

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
        string_view stop_name = ReadToken(str_in, ": ");
        data[string(stop_name)].read_coordinates(str_in);
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
};

#ifdef TESTRUNNERTSYLIN
ostream& operator<<(ostream& s_out, const Stops& stops) {
    s_out << stops.get_data();
    return s_out;
}
#endif

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
    mutable optional<double> route_length;
    template <class T>
    void add_stop_without_recalc (const string& stop_name, T apply_functor) {
        apply_functor(stop_name);
        data.emplace_back(stop_name);
    };
    
public:
    BusRoute operator=(BusRoute a) = delete;
    int get_stops_count() const {
        return stops_count;
    }
    int get_unique_stops_count() const {
        return unique_stops_count;
    }
    double get_geo_length(const Stops& stops) const {
        if (route_length)
            return route_length.value();
        
        if (data.size() <= 1) {
            route_length = 0.0;
            return route_length.value();
        }
        
        double length = 0.0;
        auto opt = stops.find_stop_and_get_coordinates(data[0]);
        if (opt == nullopt){
            string err = "NULLOPT " + STRING_FOR_STEPIC;
            throw invalid_argument(STRING_FOR_STEPIC);
        }
        
        const SphericalCoordinates *ppoint_cur = opt.value();
        const SphericalCoordinates *ppoint_prev = nullptr;
        
        if (ppoint_cur == nullptr) {
            string err = "NULLPTR " + STRING_FOR_STEPIC;
            throw invalid_argument(err);
        }

        for (uint32_t i = 0; i + 1 < data.size(); ++i) {
            ppoint_prev = ppoint_cur;
            opt = stops.find_stop_and_get_coordinates(data[i + 1]);
            if (opt == nullopt){
                string err = "NULLOPT " + STRING_FOR_STEPIC;
                throw invalid_argument(STRING_FOR_STEPIC);
            }
            
            ppoint_cur = stops.find_stop_and_get_coordinates(data[i + 1]).value();
            if (ppoint_cur == nullptr) {
                string err = "NULLPTR " + STRING_FOR_STEPIC;
                throw invalid_argument(err);
            }
            length += compute_distance(*ppoint_prev, *ppoint_cur);
        }
        if (rt == ROUT_TYPE::TWO_WAY)
            length *= 2;
        
        route_length = length;
        return route_length.value();
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

#ifdef TESTRUNNERTSYLIN
ostream& operator<<(ostream& s_out, const BusRoute& bus_route) {
    s_out << "<" << bus_route.get_data() << ">";
    return s_out;
}
#endif

struct BusStats {
    int stops_count;
    int unique_stops_count;
    double route_geo_length;
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
                //cout << stop_name << endl;
                stops.add_stop_bus_name(stop_name, string(bus_name));
                //cout << stops << endl;
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
                .route_geo_length = it->second.get_geo_length(stops)
            };
        }
        return nullopt;
    }
};

#ifdef TESTRUNNERTSYLIN
ostream& operator<<(ostream& s_out, const Buses& buses) {
    s_out << buses.get_data();
    return s_out;
}
#endif

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
                    cout << bus_stats.value().stops_count << " stops on route, " << bus_stats.value().unique_stops_count << " unique stops, " << bus_stats.value().route_geo_length << " route length" << endl;
                } else {
                    cout << "not found" << endl;
                }
            } else if (token == "Stop") {
                cout << line << ": ";
                stop_name = subline;
                auto stop_request_result = stops.find_stop_and_get_buses(stop_name);
                if (stop_request_result) {
                    if (stop_request_result.value() == nullptr) {
                        string err = "NULLPTR " + STRING_FOR_STEPIC;
                        throw invalid_argument(err);
                    }
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
#ifdef TESTRUNNERTSYLIN
    TestRunner tr;
    //RUN_TEST(tr, UnitTest);
#endif
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
