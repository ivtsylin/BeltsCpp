#pragma once

#include "stop.h"
#include "test_runner.h"

namespace TestStop {

void TestPack() {
    {
        Stop x("Kutuzov");
        ASSERT_EQUAL(x.get_name(), "Kutuzov");
        ASSERT(x.get_point() == GeoPoint({0., 0.}));
    }
    {
        Stop x("Kutuzov", 1.0, 2.0);
        ASSERT_EQUAL(x.get_name(), "Kutuzov");
        ASSERT(x.get_point() == GeoPoint({1.0, 2.0}));
    }
    /*
    {
        Stop x("Kutuzov", 1.0, 2.0);
        Stop y = x;
        ASSERT_EQUAL(x.get_name(), "Kutuzov");
        ASSERT(x.get_point() == GeoPoint({1.0, 2.0}));
        ASSERT_EQUAL(y.get_name(), "Kutuzov");
        ASSERT(y.get_point() == GeoPoint({1.0, 2.0}));
    }
     */
    {
        Stop x("Kutuzov", 1.0, 2.0);
        Stop y = std::move(x);
        ASSERT_EQUAL(x.get_name(), "");
        ASSERT_EQUAL(y.get_name(), "Kutuzov");
        ASSERT(y.get_point() == GeoPoint({1.0, 2.0}));
    }
    /*
    {
        Stop x("Kutuzov", 1.0, 2.0);
        Stop y(x);
        ASSERT_EQUAL(x.get_name(), "Kutuzov");
        ASSERT(x.get_point() == GeoPoint({1.0, 2.0}));
        ASSERT_EQUAL(y.get_name(), "Kutuzov");
        ASSERT(y.get_point() == GeoPoint({1.0, 2.0}));
    }
     */
    {
        Stop x("Kutuzov", 1.0, 2.0);
        Stop y(std::move(x));
        ASSERT_EQUAL(x.get_name(), "");
        ASSERT_EQUAL(y.get_name(), "Kutuzov");
        ASSERT(y.get_point() == GeoPoint({1.0, 2.0}));
    }
    {
        Stop x("Kutuzov", 1.0, 2.0);
        Stop y("Suvorov", 0.0, 0.0);
        x.assume_distance_to_stop(y, 2.5);
        y.assume_distance_to_stop(x, 3.5);
        
        ASSERT_EQUAL(x.get_real_distance(y), 2.5);
        ASSERT_EQUAL(y.get_real_distance(x), 3.5);
    }
    {
        Stop x("Kutuzov", 1.0, 2.0);
        Stop y("Suvorov", 0.0, 0.0);
        x.assume_distance_to_stop(y, 2.5);
        x.add_distance_to_stop(y, 3.5);
        
        ASSERT_EQUAL(x.get_real_distance(y), 3.5);
    }
    {
        Stop x("Kutuzov", 1.0, 2.0);
        Stop y("Suvorov", 0.0, 0.0);
        x.add_distance_to_stop(y, 3.5);
        x.assume_distance_to_stop(y, 2.5);
        
        ASSERT_EQUAL(x.get_real_distance(y), 3.5);
    }
    {
        Stop x("Kutuzov", 1.0, 2.0);
        Stop y("Suvorov", 0.0, 0.0);
        x.add_distance_to_stop(y, 3.5);
        x.assume_distance_to_stop(y, 2.5);
        
        ASSERT_EQUAL(x.get_real_distance(x), 0.);
        ASSERT_EQUAL(x.get_geo_distance(x), 0.);
    }
};

}
