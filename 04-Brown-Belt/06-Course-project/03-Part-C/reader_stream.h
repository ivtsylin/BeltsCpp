#pragma once

#include <istream>

#include "bus_catalog.h"

void read_bus_catalog_from_stream(BusCatalog &bus_catalog, std::istream& is);
