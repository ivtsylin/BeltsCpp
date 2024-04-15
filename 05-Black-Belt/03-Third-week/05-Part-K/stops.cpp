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



void Stops::prepeare_for_render (double height, double width, double padding, const std::map<std::string_view, std::set<std::string_view>>& adjacent) {
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
    
    {
    
        vector<set<string_view>> norm_coordinates;
        
        norm_coordinates.push_back({});
        norm_coordinates.back().insert(temp.front().second);
        
        for (int i = 1; i < temp.size(); ++i) {
            const string_view stop_name = temp[i].second;
            
            auto it = adjacent.find(stop_name);
            if (it == adjacent.end()) {
                norm_coordinates.back().insert(stop_name);
                continue;
            }
            
            bool is_found = false;
            for (const string_view cur_stops : norm_coordinates.back()) {
                if (it->second.find(cur_stops) != it->second.end()) {
                    norm_coordinates.push_back({});
                    norm_coordinates.back().insert(stop_name);
                    is_found = true;
                    break;
                }
            }
            
            if (is_found == false) {
                norm_coordinates.back().insert(stop_name);
            }
        }
        
        if (norm_coordinates.size() == 1) {
            for (auto& [_, stop_info] : data) {
                stop_info.set_normalized_coordinates_lon(padding);
            }
        } else {
            double step = (width - 2 * padding) / (norm_coordinates.size() - 1);
            
            int i = 0;
            for (const auto& set_stops : norm_coordinates) {
                for (const auto& stop_name : set_stops) {
                    data.find(stop_name)->second.set_normalized_coordinates_lon(i * step + padding);
                }
                ++i;
            }
        }
    }
    
    
    
    
    int i = 0;
    for (const auto& [stop_name, stop_info] : data) {
        temp[i] = {stop_info.get_coordinates().lat, stop_name};
        ++i;
    }
    
    sort(temp.begin(), temp.end(), [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; } );
    
    {
    
        vector<set<string_view>> norm_coordinates;
        
        norm_coordinates.push_back({});
        norm_coordinates.back().insert(temp.front().second);
        
        for (int i = 1; i < temp.size(); ++i) {
            const string_view stop_name = temp[i].second;
            
            auto it = adjacent.find(stop_name);
            if (it == adjacent.end()) {
                norm_coordinates.back().insert(stop_name);
                continue;
            }
            
            bool is_found = false;
            for (const string_view cur_stops : norm_coordinates.back()) {
                if (it->second.find(cur_stops) != it->second.end()) {
                    norm_coordinates.push_back({});
                    norm_coordinates.back().insert(stop_name);
                    is_found = true;
                    break;
                }
            }
            
            if (is_found == false) {
                norm_coordinates.back().insert(stop_name);
            }
        }
        
        if (norm_coordinates.size() == 1) {
            for (auto& [_, stop_info] : data) {
                stop_info.set_normalized_coordinates_lat(padding);
            }
        } else {
            double step = (height - 2 * padding) / (norm_coordinates.size() - 1);
            
            int i = 0;
            for (const auto& set_stops : norm_coordinates) {
                for (const auto& stop_name : set_stops) {
                    data.find(stop_name)->second.set_normalized_coordinates_lat(height - padding - i * step);
                }
                ++i;
            }
        }
    }
    
}

