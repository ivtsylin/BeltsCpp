#pragma once

#include "json_node.h"
#include "bus_catalog.h"

void read_bus_catalog_from_json(BusCatalog &bus_catalog, const json::Array& node_array);
