#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <unordered_map>
#include <optional>
#include "json.h"

struct SphericalCoordinates {
    double lat;
    double lon;
};

constexpr double PI = 3.1415926535;
constexpr double GRAD = PI / 180.;

double compute_distance(SphericalCoordinates lhs, SphericalCoordinates rhs);

class Stops {
private:
    
    struct StopInfo {
    private:
        
        SphericalCoordinates coordinates;
        std::set<std::string> buses_names;
        std::unordered_map<std::string, double> distance_data;
    public:
        StopInfo operator=(StopInfo) = delete;
        friend std::ostream& operator<<(std::ostream& output, const Stops::StopInfo &stop);
        const auto& get_distance_data() const {
            return distance_data;
        }
        const SphericalCoordinates& get_coordinates() const {
            return coordinates;
        }
        void set_coordinates(const SphericalCoordinates& rhs) {
            coordinates = rhs;
        }
        const auto& get_buses_names() const {
            return buses_names;
        }
        void add_bus_name(const std::string& bus_name) {
            buses_names.insert(bus_name);
        }
        void add_distance_to_stop(const std::string& stop_name, double distance) {
            distance_data[stop_name] = distance;
        }
        void assume_distance_to_stop(const std::string& stop_name, double distance) {
            if (!distance_data.count(stop_name))
                distance_data[stop_name] = distance;
        }
    };
    std::unordered_map<std::string, StopInfo> data;
public:
    Stops operator=(Stops) = delete;
    friend std::ostream& operator<<(std::ostream& s_out, const Stops& stops);
    friend std::ostream& operator<<(std::ostream& output, const Stops::StopInfo &stop);
    const auto& get_data() const {
        return data;
    }
    void add_stop_bus_name (const std::string &stop_name, const std::string& bus_name) {
        data[stop_name].add_bus_name(bus_name);
    }
    void read_stop_and_coordinates (const Json::Node& node_in) {
        
        const auto& map_in = node_in.AsMap();
        std::string stop_name = map_in.at("name").AsString();
        auto& stop_info = data[stop_name];
        stop_info.set_coordinates({map_in.at("latitude").AsDouble(), map_in.at("longitude").AsDouble()});
        const auto& distances = map_in.at("road_distances").AsMap();
        for (const auto& [stop_name_rhs, distance] : distances) {
            data[stop_name].add_distance_to_stop(stop_name_rhs, distance.AsInt());
            data[stop_name_rhs].assume_distance_to_stop(stop_name, distance.AsInt());
        }
    }
    std::optional<const std::set<std::string>*> find_stop_and_get_buses(const std::string& stop_name) const {
        auto it = data.find(stop_name);
        if (it == data.end()) {
            return std::nullopt;
        }
        return &it->second.get_buses_names();
    }
    double find_real_distance(const std::string &stop_name_from, const std::string &stop_name_to) const {
        const auto &distance_data = data.at(stop_name_from).get_distance_data();
        if (auto it = distance_data.find(stop_name_to); it != distance_data.end()) {
            return it->second;
        }
        return find_geo_distance(stop_name_from, stop_name_to);
    }
    double find_geo_distance(const std::string &stop_name_from, const std::string &stop_name_to) const {
        return compute_distance(data.at(stop_name_from).get_coordinates(), data.at(stop_name_to).get_coordinates());
    }
};

std::ostream& operator<<(std::ostream& output, const SphericalCoordinates &SphericalCoordinates);
std::ostream& operator<<(std::ostream& output, const Stops::StopInfo &stop);
std::ostream& operator<<(std::ostream& s_out, const Stops& stops);
