#pragma once

#include <sstream>

#include "reader_from_json.h"
#include "test_runner.h"

namespace TestReaderFromJson {

void TestPack() {
    using namespace json;
    {
        BusCatalog bus_catalog;
        
        Array node_array({
                      Map {
                          {"type", "Stop"},
                          {"name", "Tolstopaltsevo"},
                          {"longitude", 37.20829},
                          {"latitude", 55.611087}
                      }
        });
        
        read_bus_catalog_from_json(bus_catalog, node_array);
        const auto& stops = bus_catalog.get_stops();
        const auto& buses = bus_catalog.get_buses();
        ASSERT_EQUAL(stops.begin()->get_name(), "Tolstopaltsevo");
        ASSERT(stops.begin()->get_point() == GeoPoint({55.611087, 37.20829}));
        ASSERT(stops.begin()->get_distance_data().empty());
        ASSERT(buses.empty());
    }
    {
        BusCatalog bus_catalog;
        
        Array node_array({
                      Map {
                          {"type", "Stop"},
                          {"name", "Tolstopaltsevo"},
                          {"longitude", 37.20829},
                          {"latitude", 55.611087},
                          {"road_distances", Map{}},
                      }
        });
        
        read_bus_catalog_from_json(bus_catalog, node_array);
        const auto& stops = bus_catalog.get_stops();
        const auto& buses = bus_catalog.get_buses();
        ASSERT_EQUAL(stops.begin()->get_name(), "Tolstopaltsevo");
        ASSERT(stops.begin()->get_point() == GeoPoint({55.611087, 37.20829}));
        ASSERT(stops.begin()->get_distance_data().empty());
        ASSERT(buses.empty());
    }
    {
        BusCatalog bus_catalog;
        
        Array node_array({
                      Map {
                          {"type", "Stop"},
                          {"name", "Tolstopaltsevo"},
                          {"longitude", 37.20829},
                          {"latitude", 55.611087},
                          {"road_distances", Map{
                              {"Marushkino", 3900}
                          }},
                      }
        });
        
        read_bus_catalog_from_json(bus_catalog, node_array);
        const auto& stops = bus_catalog.get_stops();
        const auto& buses = bus_catalog.get_buses();
        ASSERT_EQUAL(stops.begin()->get_name(), "Tolstopaltsevo");
        ASSERT(stops.begin()->get_point() == GeoPoint({55.611087, 37.20829}));
        ASSERT_EQUAL(stops.begin()->get_distance_data().at("Marushkino"), 3900);
        ASSERT(buses.empty());
    }
    {
        BusCatalog bus_catalog;
        
        Array node_array({
                      Map {
                          {"type", "Bus"},
                          {"name", "256"},
                          {"is_roundtrip", true},
                          {"stops", Array{
                              "Biryulyovo Zapadnoye",
                              "Biryusinka",
                              "Universam",
                              "Biryulyovo Tovarnaya",
                              "Biryulyovo Passazhirskaya",
                              "Biryulyovo Zapadnoye"
                          }},
                      }
        });
        read_bus_catalog_from_json(bus_catalog, node_array);
        const auto& buses = bus_catalog.get_buses();
        ASSERT_EQUAL(buses.back().get_name(), "256");
        ASSERT(buses.back().is_circle());
        ASSERT(!buses.back().is_two_way());
        ASSERT_EQUAL(buses.back().get_route().size(), 6);
        ASSERT_EQUAL(buses.back().get_route()[0]->get_name(), "Biryulyovo Zapadnoye");
        ASSERT_EQUAL(buses.back().get_route()[1]->get_name(), "Biryusinka");
        ASSERT_EQUAL(buses.back().get_route()[2]->get_name(), "Universam");
        ASSERT_EQUAL(buses.back().get_route()[3]->get_name(), "Biryulyovo Tovarnaya");
        ASSERT_EQUAL(buses.back().get_route()[4]->get_name(), "Biryulyovo Passazhirskaya");
        ASSERT_EQUAL(buses.back().get_route()[5]->get_name(), "Biryulyovo Zapadnoye");
    }
    {
        BusCatalog bus_catalog;
        
        Array node_array({
                      Map {
                          {"type", "Bus"},
                          {"name", "750"},
                          {"is_roundtrip", false},
                          {"stops", Array{
                              "Tolstopaltsevo",
                              "Marushkino",
                              "Rasskazovka"
                          }},
                      }
        });
        read_bus_catalog_from_json(bus_catalog, node_array);
        const auto& buses = bus_catalog.get_buses();
        ASSERT_EQUAL(buses.back().get_name(), "750");
        ASSERT(!buses.back().is_circle());
        ASSERT(buses.back().is_two_way());
        ASSERT_EQUAL(buses.back().get_route().size(), 5);
        ASSERT_EQUAL(buses.back().get_route()[0]->get_name(), "Tolstopaltsevo");
        ASSERT_EQUAL(buses.back().get_route()[1]->get_name(), "Marushkino");
        ASSERT_EQUAL(buses.back().get_route()[2]->get_name(), "Rasskazovka");
        ASSERT_EQUAL(buses.back().get_route()[3]->get_name(), "Marushkino");
        ASSERT_EQUAL(buses.back().get_route()[4]->get_name(), "Tolstopaltsevo");
    }
}
}
