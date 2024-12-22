#pragma once

#include <optional>
#include <cmath>

#include "requests.h"
#include "test_runner.h"

namespace TestRequests {

void TestPack() {
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
        bus_catalog.add_bus("256",
                            ROUTE_TYPE::CIRCLE,
                            {&stop4, &stop5, &stop6, &stop7, &stop8, &stop4}
                            );
        bus_catalog.add_bus("750",
                            ROUTE_TYPE::TWO_WAY,
                            {&stop1, &stop2, &stop3}
                            );
        
        {
            auto result = get_bus_stats(bus_catalog, "256");
            ASSERT(result.bus_stats.has_value());
            ASSERT_EQUAL(result.bus_name, "256");
            ASSERT_EQUAL(result.bus_stats.value().stops_count, 6);
            ASSERT_EQUAL(result.bus_stats.value().unique_stops_count, 5);
            ASSERT_EQUAL(round(result.bus_stats.value().route_length * 100) / 100, 4371.02);
        }
        {
            auto result = get_bus_stats(bus_catalog, "750");
            ASSERT(result.bus_stats.has_value());
            ASSERT_EQUAL(result.bus_name, "750");
            ASSERT_EQUAL(result.bus_stats.value().stops_count, 5);
            ASSERT_EQUAL(result.bus_stats.value().unique_stops_count, 3);
            ASSERT_EQUAL(round(result.bus_stats.value().route_length * 10) / 10, 20939.5);
        }
        {
            auto result = get_bus_stats(bus_catalog, "751");
            ASSERT(!result.bus_stats.has_value());
            ASSERT_EQUAL(result.bus_name, "751");
        }
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
        auto& stop9 = bus_catalog.add_stop("Rossoshanskaya ulitsa", 55.595579, 37.605757);
        auto& stop10 = bus_catalog.add_stop("Prazhskaya", 55.611678, 37.603831);
        bus_catalog.add_bus("256",
                            ROUTE_TYPE::CIRCLE,
                            {&stop4, &stop5, &stop6, &stop7, &stop8, &stop4}
                            );
        bus_catalog.add_bus("750",
                            ROUTE_TYPE::TWO_WAY,
                            {&stop1, &stop2, &stop3}
                            );
        bus_catalog.add_bus("828",
                            ROUTE_TYPE::CIRCLE,
                            {&stop4, &stop6, &stop9, &stop4}
                            );
        {
            auto result = get_stop_stats(bus_catalog, "Samara");
            ASSERT(!result.bus_name_set.has_value());
            ASSERT_EQUAL(result.stop_name, "Samara");
        }
        {
            auto result = get_stop_stats(bus_catalog, "Prazhskaya");
            ASSERT(result.bus_name_set.has_value());
            ASSERT_EQUAL(result.stop_name, "Prazhskaya");
            ASSERT(result.bus_name_set.value().begin() == result.bus_name_set.value().end());
        }
        {
            auto result = get_stop_stats(bus_catalog, "Biryulyovo Zapadnoye");
            ASSERT(result.bus_name_set.has_value());
            ASSERT_EQUAL(result.stop_name, "Biryulyovo Zapadnoye");
            auto it = result.bus_name_set.value().begin();
            ASSERT_EQUAL(*it, "256");
            ++it;
            ASSERT_EQUAL(*it, "828");
            ++it;
            ASSERT(it == result.bus_name_set.value().end());
        }
    }
}
}
