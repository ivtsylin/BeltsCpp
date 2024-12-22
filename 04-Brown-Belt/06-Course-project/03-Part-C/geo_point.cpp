#pragma once

#include "geo_point.h"
#include "test_runner.h"

std::ostream& operator<<(std::ostream& output, const GeoPoint &point) {
    output << point.lat << ", " << point.lon;
    return output;
}
