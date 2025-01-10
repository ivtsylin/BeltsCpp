#pragma once

#include "request_stat.h"
#include "request_route.h"
#include "json_node.h"

void publicate_to_json(const RequestStatBus& bus_request, json::Node& node, int req_id = 0);
void publicate_to_json(const RequestStatStop& stop_request, json::Node& node, int req_id = 0);
void publicate_to_json(const std::optional<RequestRouteResult>& route_request, json::Node& node, int req_id = 0);
