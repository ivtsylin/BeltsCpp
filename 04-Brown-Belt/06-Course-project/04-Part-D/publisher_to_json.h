#pragma once

#include "requests.h"
#include "json_node.h"

void publicate_to_json(const BusRequest& bus_request, json::Node& node, int req_id = 0);
void publicate_to_json(const StopRequest& stop_request, json::Node& node, int req_id = 0);
