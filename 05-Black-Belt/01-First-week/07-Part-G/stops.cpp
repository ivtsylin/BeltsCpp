#include "stops.h"
#include "test_runner.h"

using namespace std;

double compute_distance(SphericalCoordinates lhs, SphericalCoordinates rhs) {
    return acos(sin(lhs.lat * GRAD) * sin(rhs.lat * GRAD) +
                cos(lhs.lat * GRAD) * cos(rhs.lat * GRAD) *
                cos(abs(lhs.lon * GRAD - rhs.lon * GRAD))
               ) * 6'371'000;
}

ostream& operator<<(ostream& output, const SphericalCoordinates &SphericalCoordinates) {
    output << SphericalCoordinates.lat << ", " << SphericalCoordinates.lon;
    return output;
}


ostream& operator<<(ostream& output, const Stops::StopInfo &stop) {
    output << "[" << "{" << stop.coordinates << "}" << stop.buses_names << "map=" << stop.distance_data << "]";
    return output;
}


ostream& operator<<(ostream& s_out, const Stops& stops) {
    s_out << stops.get_data();
    return s_out;
}

