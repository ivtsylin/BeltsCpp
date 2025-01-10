#pragma once

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <unordered_map>
#include "geo_point.h"

class Bus;

class Stop {
    
private:
    std::string name;
    GeoPoint point;
    std::map<std::string_view, const Bus*> buses;
    std::unordered_map<std::string_view, double> distance_data;
    
    static constexpr GeoPoint EmptyPoint = {0., 0.};
    
public:
    //Stop(const std::string& _name) : name(_name), point(EmptyPoint) {};
    //Stop(const std::string& _name, double _lat, double _lon) : name(_name), point({_lat, _lon}) {};
    Stop(const std::string& _name, double _lat = 0., double _lon = 0.) = delete;
    Stop(std::string&& _name) : name(std::move(_name)), point(EmptyPoint) {};
    Stop(std::string&& _name, double _lat, double _lon) : name(std::move(_name)), point({_lat, _lon}) {};
    
    //Stop(const Stop& rhs) : name(rhs.name), point(rhs.point), distance_data(rhs.distance_data) {};
    Stop(const Stop& rhs) = delete;
    Stop(Stop&& rhs) : name(std::move(rhs.name)), point(rhs.point), distance_data(std::move(rhs.distance_data)), buses(std::move(rhs.buses)) {};
    
    ~Stop() {};
    
    const std::string& get_name() const {
        return name;
    }
    
    const GeoPoint& get_point() const {
        return point;
    }
    void set_point(const GeoPoint& rhs) {
        point = rhs;
    }
    
    void add_distance_to_stop(const Stop& dst_stop, double distance) {
        distance_data[dst_stop.get_name()] = distance;
    }
    void assume_distance_to_stop(const Stop& dst_stop, double distance) {
        if (!distance_data.count(dst_stop.get_name()))
            distance_data[dst_stop.get_name()] = distance;
    }

    double get_geo_distance(const Stop& rhs) const {
        return point.compute_distance(rhs.point);
    }
    double get_real_distance(const Stop& rhs) const {
        if (distance_data.count(rhs.name))
            return distance_data.at(rhs.name);
        
        return get_geo_distance(rhs);
    }
    const auto& get_distance_data() const {
        return distance_data;
    }
    template <class B>
    void add_bus(const B& bus) {
        buses[bus.get_name()] = &bus;
    }
    const auto& get_buses() const {
        return buses;
    }
    
};
