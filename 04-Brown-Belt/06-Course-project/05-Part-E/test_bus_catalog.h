#pragma once

#include <unordered_map>
#include <string>
#include <utility>

#include "bus_catalog.h"
#include "test_runner.h"

namespace TestBusCatalog {

void TestPack() {
    {
        BusCatalog bus_catalog;
        auto& stop = bus_catalog.get_or_insert_stop(string_view("Kutuzov"));
        ASSERT_EQUAL(stop.get_name(), "Kutuzov");
        ASSERT(stop.get_point() == GeoPoint({0., 0.}));
    }
    {
        BusCatalog bus_catalog;
        std::string_view name = "Kutuzov";
        auto& stop = bus_catalog.get_or_insert_stop(name);
        ASSERT_EQUAL(name, "Kutuzov");
        ASSERT_EQUAL(stop.get_name(), "Kutuzov");
        ASSERT(stop.get_point() == GeoPoint({0., 0.}));
    }
    {
        BusCatalog bus_catalog;
        std::string_view name = "Kutuzov";
        auto& stop = bus_catalog.get_or_insert_stop(name);
        ASSERT_EQUAL(stop.get_name(), "Kutuzov");
        ASSERT(stop.get_point() == GeoPoint({0., 0.}));
        auto& x = bus_catalog.get_or_insert_stop(string_view("Kutuzov"));
        ASSERT_EQUAL(x.get_name(), "Kutuzov");
        ASSERT(x.get_point() == GeoPoint({0., 0.}));
        
    }
    {
        BusCatalog bus_catalog;
        std::string name = "Kutuzov";
        auto& x = bus_catalog.get_or_insert_stop(name);
        
        name = x.get_name();
        auto& y = bus_catalog.get_or_insert_stop(name);
        
        ASSERT_EQUAL(x.get_name(), "Kutuzov");
        ASSERT(x.get_point() == GeoPoint({0., 0.}));
        ASSERT_EQUAL(y.get_name(), "Kutuzov");
        ASSERT(y.get_point() == GeoPoint({0., 0.}));
    }
    {
        BusCatalog bus_catalog;
        std::string name = "Kutuzov";
        auto& x = bus_catalog.get_or_insert_stop(std::string_view(name));
        
        name = x.get_name();
        auto& y = bus_catalog.add_stop(std::move(name), 1.0, 2.0);
        
        ASSERT_EQUAL(x.get_name(), "Kutuzov");
        ASSERT(x.get_point() == GeoPoint({1., 2.}));
        ASSERT_EQUAL(y.get_name(), "Kutuzov");
        ASSERT(y.get_point() == GeoPoint({1., 2.}));
    }
    {
        BusCatalog bus_catalog;
        std::string name = "Kutuzov";
        auto& x = bus_catalog.add_stop(std::move(name), 1.0, 2.0);
        
        name = x.get_name();
        auto& y = bus_catalog.get_or_insert_stop(std::string_view(name));
        
        ASSERT_EQUAL(x.get_name(), "Kutuzov");
        ASSERT(x.get_point() == GeoPoint({1., 2.}));
        ASSERT_EQUAL(y.get_name(), "Kutuzov");
        ASSERT(y.get_point() == GeoPoint({1., 2.}));
    }
    {
        BusCatalog bus_catalog;
        std::vector<Stop*> route;
        
        auto &x = bus_catalog.add_stop(std::move("Kutuzov"), 1.0, 2.0);
        route.push_back(&x);
        
        auto &y = bus_catalog.add_stop(std::move("Suvorov"), 2.0, 3.0);
        route.push_back(&y);
        
        auto &z = bus_catalog.add_stop(std::move("Ushakov"), 3.0, 4.0);
        route.push_back(&z);
        
        ASSERT_EQUAL(route[0]->get_name(), "Kutuzov");
        ASSERT_EQUAL(route[1]->get_name(), "Suvorov");
        ASSERT_EQUAL(route[2]->get_name(), "Ushakov");
        
        auto& bus = bus_catalog.add_bus(std::move("114"), ROUTE_TYPE::TWO_WAY, std::move(route));
        
        ASSERT_EQUAL(bus.get_name(), "114");
        ASSERT_EQUAL(bus.get_route()[0]->get_name(), "Kutuzov");
        ASSERT_EQUAL(bus.get_route()[1]->get_name(), "Suvorov");
        ASSERT_EQUAL(bus.get_route()[2]->get_name(), "Ushakov");
    }
    {
        double dist = acos(sin(55.574371 * GRAD) * sin(55.581065 * GRAD) +
                    cos(55.574371 * GRAD) * cos(55.581065 * GRAD) *
                    cos(abs(37.6517 * GRAD - 37.64839 * GRAD))
                   ) * 6'371'000;
        ASSERT_EQUAL(round(dist * 100) / 100, 772.87);
    }
    {
        GeoPoint p1{55.574371, 37.6517};
        GeoPoint p2{55.581065, 37.64839};

        ASSERT_EQUAL(round(p1.compute_distance(p2) * 100) / 100, 772.87);
    }
    {
        Stop stop1("Biryulyovo Zapadnoye", 55.574371, 37.6517);
        Stop stop2("Biryusinka", 55.581065, 37.64839);
        ASSERT_EQUAL(round(stop1.get_real_distance(stop2) * 100) / 100, 772.87);
    }
    {
        BusCatalog bus_catalog;
        auto& stop1 = bus_catalog.add_stop("Tolstopaltsevo", 55.611087, 37.20829);
        auto& stop2 = bus_catalog.add_stop("Marushkino", 55.595884, 37.209755);
        auto& stop3 = bus_catalog.add_stop("Rasskazovka", 55.632761, 37.333324);
        auto& stop4 = bus_catalog.add_stop("Biryulyovo Zapadnoye", 55.574371, 37.6517);
        auto& stop5 = bus_catalog.add_stop("Biryusinka", 55.581065, 37.64839);
        auto& stop6 = bus_catalog.add_stop("Universam", 55.587655, 37.645687);
        auto& stop7 = bus_catalog.add_stop("Biryulyovo Tovarnaya", 55.592028, 37.653656);
        auto& stop8 = bus_catalog.add_stop("Biryulyovo Passazhirskaya", 55.580999, 37.659164);
        auto& bus = bus_catalog.add_bus("256",
                            ROUTE_TYPE::CIRCLE,
                            {&stop4, &stop5, &stop6, &stop7, &stop8, &stop4}
                            );
    
        ASSERT_EQUAL(bus.get_stops_count(), 6);
        ASSERT_EQUAL(bus.get_unique_stops_count(), 5);
        
        ASSERT_EQUAL(round(stop4.get_real_distance(stop5) * 100) / 100, 772.87);
        
        ASSERT_EQUAL(round(bus.get_route()[0]->get_real_distance(*bus.get_route()[1]) * 100) / 100, 772.87);
        ASSERT_EQUAL(round(bus.get_route()[1]->get_real_distance(*bus.get_route()[2]) * 100) / 100, 752.21);
        ASSERT_EQUAL(round(bus.get_route()[2]->get_real_distance(*bus.get_route()[3]) * 100) / 100, 698);
        ASSERT_EQUAL(round(bus.get_route()[3]->get_real_distance(*bus.get_route()[4]) * 100) / 100, 1274.28);
        ASSERT_EQUAL(round(bus.get_route()[4]->get_real_distance(*bus.get_route()[5]) * 100) / 100, 873.66);
        
        ASSERT_EQUAL(round(bus.get_real_length() * 100) / 100, 4371.02);
    }
}
}
