#pragma once

#include <optional>
#include <cmath>

#include "request_route.h"
#include "test_runner.h"

namespace TestRequestRoute {

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

        }
    }
}
}
