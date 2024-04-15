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

void Stops::prepeare_for_render (double height, double width, double padding) {
    if (is_prepeared)
        return;
    
    is_prepeared = true;
    
    if (data.size() == 0)
        return;
    
    if (data.size() == 1) {
        data.begin()->second.set_normalized_coordinates_lon(padding);
        data.begin()->second.set_normalized_coordinates_lat(height - padding);
        return;
    }
    
    vector<pair<double, string_view>> temp;
    for (const auto& [stop_name, stop_info] : data) {
        temp.emplace_back(
                          stop_info.get_coordinates().lon,
                          stop_name
                          );
    }
    
    sort(temp.begin(), temp.end(), [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; } );
    
    double step = (width - 2 * padding) / (data.size() - 1);
    
    int i = 0;
    for (const auto& [_, stop_name] : temp) {
        data.find(stop_name)->second.set_normalized_coordinates_lon(i * step + padding);
        ++i;
    }
    
    
    
    
    i = 0;
    for (const auto& [stop_name, stop_info] : data) {
        temp[i] = {stop_info.get_coordinates().lat, stop_name};
        ++i;
    }
    
    sort(temp.begin(), temp.end(), [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; } );
    
    step = (height - 2 * padding) / (data.size() - 1);
    
    i = 0;
    for (const auto& [_, stop_name] : temp) {
        data.find(stop_name)->second.set_normalized_coordinates_lat(height - padding - i * step);
        ++i;
    }
    
}

