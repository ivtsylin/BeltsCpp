#include "publisher_stream.h"

using namespace std;

void publicate_to_stream(const BusRequest& bus_request, std::ostream& sout) {
    sout << "Bus " << bus_request.bus_name << ": ";
    if (bus_request.bus_stats.has_value()) {
        const auto& bus_stats = bus_request.bus_stats.value();
        sout
            << bus_stats.stops_count << " stops on route, "
            << bus_stats.unique_stops_count << " unique stops, "
            << bus_stats.route_length << " route length, "
            << bus_stats.curvature << " curvature";
    } else {
        sout << "not found";
    }
    sout << '\n';
}

void publicate_to_stream(const StopRequest& stop_request, std::ostream& sout) {
    sout << "Stop " << stop_request.stop_name << ": ";
    if (stop_request.bus_name_set.has_value()) {
        const auto& bus_name_set = stop_request.bus_name_set.value();
        if (bus_name_set.begin() != bus_name_set.end()) {
            sout << "buses";
            for (const auto bus_name : bus_name_set) {
                sout << " " << bus_name;
            }
        } else {
            sout << "no buses";
        }
    } else {
        sout << "not found";
    }
    sout << '\n';
}
