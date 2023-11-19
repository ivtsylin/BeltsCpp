#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <set>
#include <map>
#include <vector>
#include <unordered_map>
#include <optional>
#include <cmath>
#include <variant>
#include "test_runner.h"

using namespace std;

namespace Json {

  class Node : std::variant<std::vector<Node>,
                            std::map<std::string, Node>,
                            int,
                            std::string,
                            double,
                            bool> {
  public:
    using variant::variant;
    // Const type
    const auto& AsArray() const {
        try {
            return std::get<std::vector<Node>>(*this);
        } catch(...) {
            stringstream ss;
            ss << ToString() << " try to read AsArray";
            throw invalid_argument(ss.str());
        }
    }
    const auto& AsMap() const {
        try {
            return std::get<std::map<std::string, Node>>(*this);
        } catch(...) {
            stringstream ss;
            ss << ToString() << " try to read AsMap";
            throw invalid_argument(ss.str());
        }
    }
    int AsInt() const {
        try {
            return std::get<int>(*this);
        } catch(...) {
            stringstream ss;
            ss << ToString() << " try to read AsInt";
            throw invalid_argument(ss.str());
        }
    }
    const auto& AsString() const {
        try {
            return std::get<std::string>(*this);
        } catch(...) {
            stringstream ss;
            ss << ToString() << " try to read AsString";
            throw invalid_argument(ss.str());
        }
    }
    double AsDouble() const {
        try {
            if (IsInt())
                return std::get<int>(*this);
            return std::get<double>(*this);
        } catch(...) {
            stringstream ss;
            ss << ToString() << " try to read AsDouble";
            throw invalid_argument(ss.str());
        }
    }
    bool AsBool() const {
        try {
            return std::get<bool>(*this);
        } catch(...) {
            stringstream ss;
            ss << ToString() << " try to read AsBool";
            throw invalid_argument(ss.str());
        }
    }
    // Getters
    bool IsArray() const {
      return std::holds_alternative<std::vector<Node>>(*this);
    }
    bool IsMap() const {
      return std::holds_alternative<std::map<std::string, Node>>(*this);
    }
    bool IsInt() const {
      return std::holds_alternative<int>(*this);
    }
    bool IsString() const {
      return std::holds_alternative<std::string>(*this);
    }
    bool IsDouble() const {
      return std::holds_alternative<double>(*this);
    }
    bool IsBool() const {
      return std::holds_alternative<bool>(*this);
    }
    // for printing
    std::string ToString() const {
      std::stringstream ss;
      if (IsArray()) {
        ss << '[';
        for (auto& x : AsArray()) ss << x.ToString() << ',';
          auto out = ss.str();
          if (out == "[")
            out.push_back(']');
          else
            out.back() = ']';
          return out;
      } else if (IsMap()) {
        ss << '{';
        for (const auto& [key, value] : AsMap())
          ss << "\"" << key << "\":" << value.ToString() << ',';
          auto out = ss.str();
          if (out == "{")
            out.push_back('}');
          else
            out.back() = '}';
          return out;
      } else if (IsDouble()) {
        ss << fixed << setprecision(7) << AsDouble();
        return ss.str();
      } else if (IsInt()) {
        ss << AsInt();
        return ss.str();
      } else if (IsString()) {
        ss << '"' << AsString() << '"';
        return ss.str();
      } else if (IsBool())
        return AsBool() ? "true" : "false";
      else
        return "";
    }
  };

  class Document {
  public:
    explicit Document(Node root);

    const Node& GetRoot() const;

  private:
    Node root;
  };

  Document Load(std::istream& input);

  ostream& operator<<(ostream& out, const Document& doc) {
    out << doc.GetRoot().ToString();
    return out;
  }
}


namespace Json {

  Document::Document(Node root) : root(move(root)) {
  }

  const Node& Document::GetRoot() const {
    return root;
  }

  Node LoadNode(istream& input);

  Node LoadArray(istream& input) {
    vector<Node> result;

    for (char c; input >> c && c != ']'; ) {
      if (c != ',') {
        input.putback(c);
      }
      result.push_back(LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadNumber(istream& input) {
      int integer = 0;
      double real = 0.;
      input >> integer;
      if (input.peek() == '.') {
          input >> real;
          return Node(real + integer);
      }
      return Node(integer);
  }

  Node LoadBool(istream& input) {
    bool result;
    input >> std::boolalpha >> result;
    return Node(result);
  }

  Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
    return Node(move(line));
  }

  Node LoadDict(istream& input) {
    map<string, Node> result;

    for (char c; input >> c && c != '}'; ) {
      if (c == ',') {
        input >> c;
      }

      string key = LoadString(input).AsString();
      input >> c;
      result.emplace(move(key), LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
      return LoadArray(input);
    } else if (c == '{') {
      return LoadDict(input);
    } else if (c == '"') {
      return LoadString(input);
    } else if (std::isdigit(c) || c == '-'){
      input.putback(c);
      return LoadNumber(input);
    } else {
      input.putback(c);
      return LoadBool(input);
    }
  }

  Document Load(istream& input) {
    return Document{LoadNode(input)};
  }
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
    void read_stop_and_coordinates (const Json::Node& node_in) {
        
        const auto& map_in = node_in.AsMap();
        string stop_name = map_in.at("name").AsString();
        auto& stop_info = data[stop_name];
        stop_info.set_coordinates({map_in.at("latitude").AsDouble(), map_in.at("longitude").AsDouble()});
        const auto& distances = map_in.at("road_distances").AsMap();
        for (const auto& [stop_name_rhs, distance] : distances) {
            data[stop_name].add_distance_to_stop(stop_name_rhs, distance.AsInt());
            data[stop_name_rhs].assume_distance_to_stop(stop_name, distance.AsInt());
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
    void read_bus_route (const Json::Node& node_in, T apply_functor) {
        const auto& node_map = node_in.AsMap();
        rt = node_map.at("is_roundtrip").AsBool() ? ROUT_TYPE::CIRCLE : ROUT_TYPE::TWO_WAY;
        const auto& stops_array = node_map.at("stops").AsArray();
        set<string_view> count_set;
        for (const auto& stop_name : stops_array) {
            data.push_back(stop_name.AsString());
            apply_functor(stop_name.AsString());
            count_set.insert(string_view(stop_name.AsString()));
        }
        if (rt == ROUT_TYPE::CIRCLE) { // CIRCLE
            stops_count = data.size();
        } else { // TWO_WAY
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
    
    void read_bus_and_bus_route (const Json::Node& node_in, Stops& stops) {
        
        const auto& node_map = node_in.AsMap();
        string bus_number = node_map.at("name").AsString();
        
        data[bus_number].read_bus_route(node_in, [&stops, &bus_number](const string& stop_name) {stops.add_stop_bus_name(stop_name, bus_number);});
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
    friend ostream& operator<< (ostream& os, const BusManager& bm);
    void read_bus_catalog(const Json::Document& doc) {
        const auto& root = doc.GetRoot().AsMap();
        if (root.find("base_requests") == root.end()) return;
        const auto& requests = root.at("base_requests").AsArray();
        for (const auto& request : requests) {
            if (request.AsMap().at("type").AsString() == "Stop") {
                stops.read_stop_and_coordinates(request);
            } else if (request.AsMap().at("type").AsString() == "Bus") {
                buses.read_bus_and_bus_route(request, stops);
            } else {
                stringstream error;
                error << "string *" << request.AsMap().at("type").AsString() << "* can not be recognized\n";
                throw invalid_argument(error.str());
            }
        }
    }
    
    Json::Document process_and_print_requests(const Json::Document& doc) {
        const auto& root_map = doc.GetRoot().AsMap();
        vector<Json::Node> output;
        if (root_map.find("stat_requests") == root_map.end()) return Json::Document(Json::Node(std::move(output)));
        
        const auto& stat_requests = root_map.at("stat_requests").AsArray();
        
        for (const auto& stat_req : stat_requests) {
            const auto& stat_req_map = stat_req.AsMap();
            map<string, Json::Node> res;
            if (stat_req_map.at("type").AsString() == "Bus") {
                auto bus_stats = buses.get_bus_stats(stat_req_map.at("name").AsString(), stops);
                res["request_id"] = stat_req_map.at("id").AsInt();
                if(bus_stats) {
                    res["route_length"] = bus_stats.value().route_real_length;
                    res["curvature"] = bus_stats.value().curvature;
                    res["stop_count"] = bus_stats.value().stops_count;
                    res["unique_stop_count"] = bus_stats.value().unique_stops_count;
                } else {
                    res["error_message"] = string("not found");
                }
            } else if (stat_req_map.at("type").AsString() == "Stop") {
                auto stop_request_result = stops.find_stop_and_get_buses(stat_req_map.at("name").AsString());
                res["request_id"] = stat_req_map.at("id").AsInt();
                if(stop_request_result) {
                    vector<Json::Node> buses_output;
                    for (const auto& bus_output : *(stop_request_result.value())) {
                        buses_output.push_back(bus_output);
                    }
                    res["buses"] = move(buses_output);
                } else {
                    res["error_message"] = string("not found");
                }
            } else {
                string err = "Invalid type of stat request/n";
                throw invalid_argument(err);
            }
            output.emplace_back(move(res));
        }
        return Json::Document(Json::Node(std::move(output)));
    }
};

ostream& operator<< (ostream& out, const BusManager& bus_man) {
    out << bus_man.stops << endl;
    out << bus_man.buses << endl;
    return out;
}

void UnitTest() {
}

int main() {
    TestRunner tr;
    //RUN_TEST(tr, UnitTest);
    //try {
        cout << fixed << setprecision(7);
        Json::Document doc = Json::Load(cin);
        //cout << doc << endl;
        BusManager bus_manager;
        bus_manager.read_bus_catalog(doc);
        //cout << bus_manager << endl;
        cout << bus_manager.process_and_print_requests(doc) << endl;
        
    //} catch (...) {
    //    throw invalid_argument("Unknown problem");
    //}
    return 0;
}
