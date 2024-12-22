#pragma once

#include "bus_catalog.h"
#include "test_runner.h"

std::ostream& operator<<(std::ostream& output, const BusCatalog &bus_catalog) {
    output << "STOPS " << bus_catalog.get_stops() << endl;
    output << "BUSES " << bus_catalog.get_buses() << endl;
    return output;
}
