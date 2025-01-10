#pragma once

#include <optional>

#include "bus_catalog.h"

struct RequestStatBus {
    
    struct BusStats {
        int stops_count;
        int unique_stops_count;
        double route_length;
        double curvature;
    };
    
    std::optional<BusStats> bus_stats;
};



struct RequestStatStop {
    
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
    
    std::optional<BusNameSet> bus_name_set;
};

class RequestStat {
private:
    const BusCatalog& bus_catalog;
public:
    RequestStat (const BusCatalog& bus_catalog_) : bus_catalog(bus_catalog_) {};
    RequestStatBus get_bus_stats(std::string_view bus_name);
    RequestStatStop get_stop_stats(std::string_view stop_name);
};


