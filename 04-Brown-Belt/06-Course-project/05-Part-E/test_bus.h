#pragma once

#include "bus.h"
#include "test_runner.h"

namespace TestBus {

void TestPack() {
    {
        Stop x("Kutuzov", 1.0, 2.0);
        vector<Stop*> route{&x};
        Bus b("11", ROUTE_TYPE::CIRCLE, route);
        
        ASSERT_EQUAL(b.get_name(), "11");
        ASSERT(b.is_circle());
        ASSERT(!b.is_two_way());
        ASSERT_EQUAL(b.get_stops_count(), 1);
        ASSERT_EQUAL(b.get_unique_stops_count(), 1);
        ASSERT_EQUAL(b.get_curvature(), 1.);
        ASSERT_EQUAL(b.get_geo_length(), 0.);
        ASSERT_EQUAL(b.get_real_length(), 0.);
    }
    {
        Stop x("Kutuzov", 1.0, 2.0);
        Stop y("Suvorov", 0.0, 0.0);
        Stop z("Potemkin", 4.0, 5.0);
        vector<Stop*> route{&x, &y, &z};
        Bus b("11", ROUTE_TYPE::CIRCLE, route);
        
        ASSERT_EQUAL(b.get_name(), "11");
        ASSERT(b.is_circle());
        ASSERT(!b.is_two_way());
        ASSERT_EQUAL(b.get_stops_count(), 3);
        ASSERT_EQUAL(b.get_unique_stops_count(), 3);
        ASSERT_EQUAL(b.get_curvature(), 1.);
    }
    {
        Stop x("Kutuzov", 1.0, 2.0);
        Stop y("Suvorov", 0.0, 0.0);
        Stop z("Potemkin", 4.0, 5.0);
        vector<Stop*> route{&x, &y, &z};
        Bus b("11", ROUTE_TYPE::TWO_WAY, route);
        
        ASSERT_EQUAL(b.get_name(), "11");
        ASSERT(!b.is_circle());
        ASSERT(b.is_two_way());
        ASSERT_EQUAL(b.get_stops_count(), 5);
        ASSERT_EQUAL(b.get_unique_stops_count(), 3);
        ASSERT_EQUAL(b.get_curvature(), 1.);
    }
    {
        Stop x("Kutuzov", 1.0, 2.0);
        Stop y("Suvorov", 0.0, 0.0);
        Stop z("Kutuzov", 4.0, 5.0);
        vector<Stop*> route{&x, &y, &z};
        Bus b("11", ROUTE_TYPE::CIRCLE, route);
        
        ASSERT_EQUAL(b.get_name(), "11");
        ASSERT(b.is_circle());
        ASSERT(!b.is_two_way());
        ASSERT_EQUAL(b.get_stops_count(), 3);
        ASSERT_EQUAL(b.get_unique_stops_count(), 2);
        ASSERT_EQUAL(b.get_curvature(), 1.);
    }
    {
        Stop stop1("Tolstopaltsevo", 55.611087, 37.20829);
        Stop stop2("Marushkino", 55.595884, 37.209755);
        Stop stop3("Rasskazovka", 55.632761, 37.333324);
        Stop stop4("Biryulyovo Zapadnoye", 55.574371, 37.6517);
        Stop stop5("Biryusinka", 55.581065, 37.64839);
        Stop stop6("Universam", 55.587655, 37.645687);
        Stop stop7("Biryulyovo Tovarnaya", 55.592028, 37.653656);
        Stop stop8("Biryulyovo Passazhirskaya", 55.580999, 37.659164);
        Stop stop9("Rossoshanskaya ulitsa", 55.595579, 37.605757);
        Stop stop10("Prazhskaya", 55.611678, 37.603831);
        
        stop1.add_distance_to_stop(stop2, 3900);
        stop2.assume_distance_to_stop(stop1, 3900);
        
        stop2.add_distance_to_stop(stop3, 9900);
        stop3.assume_distance_to_stop(stop2, 9900);
        
        stop4.add_distance_to_stop(stop9, 7500);
        stop9.assume_distance_to_stop(stop4, 7500);
        stop4.add_distance_to_stop(stop5, 1800);
        stop5.assume_distance_to_stop(stop4, 1800);
        stop4.add_distance_to_stop(stop6, 2400);
        stop6.assume_distance_to_stop(stop4, 2400);
        
        stop5.add_distance_to_stop(stop6, 750);
        stop6.assume_distance_to_stop(stop5, 750);
        
        stop6.add_distance_to_stop(stop9, 5600);
        stop9.assume_distance_to_stop(stop6, 5600);
        stop6.add_distance_to_stop(stop7, 900);
        stop7.assume_distance_to_stop(stop6, 900);
        
        stop7.add_distance_to_stop(stop8, 1300);
        stop8.assume_distance_to_stop(stop7, 1300);
        
        stop8.add_distance_to_stop(stop4, 1200);
        stop4.assume_distance_to_stop(stop8, 1200);
        {
            Bus bus("256", ROUTE_TYPE::CIRCLE, {&stop4, &stop5, &stop6, &stop7, &stop8, &stop4});
            ASSERT_EQUAL(bus.get_real_length(), 5950);
        }
        {
            Bus bus("750", ROUTE_TYPE::TWO_WAY, {&stop1, &stop2, &stop3});
            ASSERT_EQUAL(bus.get_real_length(), 27600);
        }
    }
    
}

};
