#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <utility>

#include "stop.h"

#include <iostream>

enum class ROUTE_TYPE {
    CIRCLE,
    TWO_WAY
};

class Bus {
private:
    std::string name;
    std::vector<Stop*> route;
    ROUTE_TYPE rt;
    
    int unique_stops_count;
    
    mutable bool is_ready = false;
    mutable double route_geo_length = 0.0;
    mutable double route_real_length = 0.0;
    mutable double curvature = 1.0;

    void compute_length_and_curvature() const {
        
        is_ready = true;
        
        route_geo_length = 0.0;
        route_real_length = 0.0;
        
        for (uint32_t i = 0; i + 1 < route.size(); ++i) {
            route_geo_length += route[i]->get_geo_distance(*route[i + 1]);
            route_real_length += route[i]->get_real_distance(*route[i + 1]);
        }
        
        curvature = (route_real_length == 0. && route_geo_length == 0.) ? 1. : route_real_length / route_geo_length;
        
    }
    
    void prepeare_ctor() {
        std::set<std::string_view> unique_stops;
        for (const auto& s : route) {
            auto [it, flag] = unique_stops.insert(s->get_name());
            if (flag) s->add_bus(*this);
        }
        unique_stops_count = unique_stops.size();
        
        if (int size = route.size(); rt == ROUTE_TYPE::TWO_WAY && size > 1) {
            for (int i = size - 2; i >= 0; --i) {
                route.push_back(route[i]);
            }
        }
    }
    
public:
    
    Bus(const std::string &_name, ROUTE_TYPE _rt, const std::vector<Stop*> &_route) : name(_name), rt(_rt), route(_route) {
        prepeare_ctor();
    };
    Bus(std::string &&_name, ROUTE_TYPE _rt, std::vector<Stop*> &&_route) : name(std::move(_name)), rt(_rt), route(std::move(_route)) {
        prepeare_ctor();
    };
    
    Bus(const Bus& rhs) : name(rhs.name), route(rhs.route), rt(rhs.rt), unique_stops_count(rhs.unique_stops_count) {
        is_ready = rhs.is_ready;
        if (is_ready) {
            route_geo_length = rhs.route_geo_length;
            route_real_length = rhs.route_real_length;
            curvature = rhs.curvature;
        }
    }
    Bus(Bus&& rhs) : name(std::move(rhs.name)), route(std::move(rhs.route)), rt(rhs.rt), unique_stops_count(rhs.unique_stops_count) {
        is_ready = rhs.is_ready;
        if (is_ready) {
            route_geo_length = rhs.route_geo_length;
            route_real_length = rhs.route_real_length;
            curvature = rhs.curvature;
        }
    }
    
    Bus& operator=(const Bus& rhs) {
        name = rhs.name;
        route = rhs.route;
        rt = rhs.rt;
        unique_stops_count = rhs.unique_stops_count;
        is_ready = rhs.is_ready;
        if (is_ready) {
            route_geo_length = rhs.route_geo_length;
            route_real_length = rhs.route_real_length;
            curvature = rhs.curvature;
        }
        return *this;
    }
    Bus& operator=(Bus&& rhs) {
        name = std::move(rhs.name);
        route = std::move(rhs.route);
        rt = rhs.rt;
        unique_stops_count = rhs.unique_stops_count;
        is_ready = rhs.is_ready;
        if (is_ready) {
            route_geo_length = rhs.route_geo_length;
            route_real_length = rhs.route_real_length;
            curvature = rhs.curvature;
        }
        return *this;
    }
    
    ~Bus() {};
    
    const std::string& get_name() const {
        return name;
    }
    const auto& get_route() const {
        return route;
    }
    bool is_circle() const {
        return rt == ROUTE_TYPE::CIRCLE;
    }
    bool is_two_way() const {
        return rt == ROUTE_TYPE::TWO_WAY;
    }
    
    int get_stops_count() const {
        return route.size();
    }
    int get_unique_stops_count() const {
        return unique_stops_count;
    }
    double get_geo_length() const {
        if (is_ready)
            return route_geo_length;
        
        compute_length_and_curvature();
        return route_geo_length;
    }
    double get_real_length() const {
        if (is_ready)
            return route_real_length;
        
        compute_length_and_curvature();
        return route_real_length;
    }
    double get_curvature() const {
        if (is_ready)
            return curvature;
        
        compute_length_and_curvature();
        return curvature;
    }
};

