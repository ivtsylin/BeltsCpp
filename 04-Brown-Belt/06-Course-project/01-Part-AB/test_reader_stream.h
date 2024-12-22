#pragma once

#include <sstream>

#include "reader_stream.h"
#include "test_runner.h"

namespace TestStreamReader {

void TestPack() {
    {
        BusCatalog bus_catalog;
        stringstream s;
        s << "1" << endl;
        s << "Stop Tolstopaltsevo: 55.611087, 37.20829" << endl;
        read_bus_catalog_from_stream(bus_catalog, s);
        const auto& stops = bus_catalog.get_stops();
        const auto& buses = bus_catalog.get_buses();
        ASSERT_EQUAL(stops.begin()->get_name(), "Tolstopaltsevo");
        ASSERT(stops.begin()->get_point() == GeoPoint({55.611087, 37.20829}));
        ASSERT(buses.empty());
    }
    {
        BusCatalog bus_catalog;
        stringstream s;
        s << "1" << endl;
        s << "Stop Tolstopaltsevo: -55.611087, -37.20829" << endl;
        read_bus_catalog_from_stream(bus_catalog, s);
        const auto& stops = bus_catalog.get_stops();
        const auto& buses = bus_catalog.get_buses();
        ASSERT_EQUAL(stops.begin()->get_name(), "Tolstopaltsevo");
        ASSERT(stops.begin()->get_point() == GeoPoint({-55.611087, -37.20829}));
        ASSERT(buses.empty());
    }
    {
        BusCatalog bus_catalog;
        stringstream s;
        s << "1" << endl;
        s << "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye" << endl;
        read_bus_catalog_from_stream(bus_catalog, s);
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
        stringstream s;
        s << "1" << endl;
        s << "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka" << endl;
        read_bus_catalog_from_stream(bus_catalog, s);
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
