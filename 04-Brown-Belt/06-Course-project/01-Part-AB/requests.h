#pragma once

#include <optional>

#include "bus_catalog.h"

struct BusStats {
    int stops_count;
    int unique_stops_count;
    double route_length;
};

struct BusRequest {
    std::string_view bus_name;
    std::optional<BusStats> bus_stats;
};


template<class IT>
class BusNameIterator {
private:
    IT iter;
public:
    BusNameIterator(IT t) {
        iter = t;
    };
    bool operator==(const BusNameIterator& lhs) const {
        return iter == lhs.iter;
    }
    bool operator!=(const BusNameIterator& lhs) const {
        return iter != lhs.iter;
    }
    std::string_view operator*() const {
        return iter->first;
    }
    BusNameIterator& operator++() {
        ++iter;
        return *this;
    }
};

class BusNameSet {
private:
    const std::map<std::string_view, const Bus*>& data;
public:
    BusNameSet(const std::map<std::string_view, const Bus*>& _data) : data(_data) {};
    
    const BusNameIterator<std::map<std::string_view, const Bus*>::const_iterator> begin() const {
        return data.cbegin();
    }
    const BusNameIterator<std::map<std::string_view, const Bus*>::const_iterator> end() const {
        return data.cend();
    }
};

struct StopRequest {
    std::string_view stop_name;
    std::optional<BusNameSet> bus_name_set;
};

BusRequest get_bus_stats(const BusCatalog &bus_catalog, std::string_view bus_name);
StopRequest get_stop_stats(const BusCatalog &bus_catalog, std::string_view stop_name);
