#include "requests.h"

using namespace std;

BusRequest get_bus_stats(const BusCatalog &bus_catalog, std::string_view bus_name) {
    const auto bus_opt = bus_catalog.get_bus(bus_name);
    if (!bus_opt.has_value())
        return {std::nullopt};
    const auto bus_ptr = bus_opt.value();
    
    return {BusStats{
                .stops_count = bus_ptr->get_stops_count(),
                .unique_stops_count = bus_ptr->get_unique_stops_count(),
                .route_length = bus_ptr->get_real_length(),
                .curvature = bus_ptr->get_curvature()
            }
    };
}

StopRequest get_stop_stats(const BusCatalog &bus_catalog, std::string_view stop_name) {
    
    const auto stop_opt = bus_catalog.get_stop(stop_name);
    if (!stop_opt.has_value())
        return {std::nullopt};
    const auto stop_ptr = stop_opt.value();
    
    return {stop_ptr->get_buses()};
}
