#pragma once

#include "publisher_to_json.h"

void process_to_json(const BusCatalog &bus_catalog, json::Array& requests, json::Array& responses);

