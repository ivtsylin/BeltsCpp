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
}
}
