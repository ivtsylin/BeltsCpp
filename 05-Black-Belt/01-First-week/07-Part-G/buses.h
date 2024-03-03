#pragma once

#include "stops.h"

enum class ROUT_TYPE {
    CIRCLE,
    TWO_WAY
};

class BusRoute {
private:
    std::vector<std::string> data;
    ROUT_TYPE rt;
    int stops_count;
    int unique_stops_count;
    mutable std::optional<double> route_geo_length;
    mutable std::optional<double> route_real_length;
    mutable std::optional<double> curvature;
    void compute_length_and_curvature(const Stops& stops) const {
        if (data.size() <= 1) {
            route_geo_length = 0.0;
            route_real_length = 0.0;
            curvature = 1.0;
            return;
        }
        
        double geo_length = 0.0;
        double real_length = 0.0;
        
        for (uint32_t i = 0; i + 1 < data.size(); ++i) {
            geo_length += stops.find_geo_distance(data[i], data[i + 1]);
            real_length += stops.find_real_distance(data[i], data[i + 1]);
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
    const auto get_route_type() const {
        return rt;
    }
    template<class T>
    void read_bus_route (const Json::Node& node_in, T apply_functor) {
        const auto& node_map = node_in.AsMap();
        rt = node_map.at("is_roundtrip").AsBool() ? ROUT_TYPE::CIRCLE : ROUT_TYPE::TWO_WAY;
        const auto& stops_array = node_map.at("stops").AsArray();
        
        std::set<std::string_view> count_set;
        for (int i = 0; i < stops_array.size(); ++i) {
            const auto& stop_name_ = stops_array[i].AsString();
            data.push_back(stop_name_);
            apply_functor(stop_name_);
            count_set.insert(stop_name_);
        }
        if (rt == ROUT_TYPE::TWO_WAY && stops_array.size() > 1) {
            for (int i = stops_array.size() - 2; i >= 0; --i) {
                const auto& stop_name_ = stops_array[i].AsString();
                data.push_back(stop_name_);
            }
        }
        stops_count = data.size();
        unique_stops_count = count_set.size();
    }
};

struct BusStats {
    int stops_count;
    int unique_stops_count;
    double route_real_length;
    double curvature;
};

class Buses {
private:
    std::map<std::string, BusRoute> data;
public:
    Buses operator=(Buses) = delete;
    const auto& get_data() const {
        return data;
    }
    
    void read_bus_and_bus_route (const Json::Node& node_in, Stops& stops) {
        
        const auto& node_map = node_in.AsMap();
        std::string bus_number = node_map.at("name").AsString();
        
        data[bus_number].read_bus_route(node_in, [&stops, &bus_number](const std::string& stop_name) {stops.add_stop_bus_name(stop_name, bus_number);});
    }
    
    std::optional<const BusRoute*> find_bus_and_get_bus_route(std::string_view bus_name) const {
        return find_bus_and_get_bus_route(std::string(bus_name));
    }
    std::optional<const BusRoute*> find_bus_and_get_bus_route(const std::string &bus_name) const {
        auto it = data.find(bus_name);
        if (it == data.end()) {
            return std::nullopt;
        }
        return &it->second;
    }
    std::optional<BusStats> get_bus_stats(std::string_view bus_name, const Stops& stops) const {
        return get_bus_stats(std::string(bus_name), stops);
    }
    std::optional<BusStats> get_bus_stats(const std::string &bus_name, const Stops& stops) const {
        if (auto it = data.find(bus_name); it != data.end()) {
            return BusStats{
                .stops_count = it->second.get_stops_count(),
                .unique_stops_count = it->second.get_unique_stops_count(),
                .route_real_length = it->second.get_real_length(stops),
                .curvature = it->second.get_curvature(stops)
            };
        }
        return std::nullopt;
    }
};

std::ostream& operator<<(std::ostream& s_out, const BusRoute& bus_route);
std::ostream& operator<<(std::ostream& s_out, const Buses& buses);

