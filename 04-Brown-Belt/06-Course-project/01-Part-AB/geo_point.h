#pragma once

#include <cmath>
#include <ostream>

constexpr double PI = 3.1415926535;
constexpr double GRAD = PI / 180.;

inline bool isZero(double x) {
    return std::fabs(x) < 1e-6;
}

struct GeoPoint {
    double lat;
    double lon;
    
    double compute_distance(const GeoPoint &rhs) const {
        return acos(sin(lat * GRAD) * sin(rhs.lat * GRAD) +
                    cos(lat * GRAD) * cos(rhs.lat * GRAD) *
                    cos(abs(lon * GRAD - rhs.lon * GRAD))
                   ) * 6'371'000;
    }
    
    bool operator==(const GeoPoint& rhs) const {
        return (lat == rhs.lat) && (lon == rhs.lon);
    }
    bool operator!=(const GeoPoint& rhs) const {
        return !((lat == rhs.lat) && (lon == rhs.lon));
    }
};

std::ostream& operator<<(std::ostream& output, const GeoPoint &point);
