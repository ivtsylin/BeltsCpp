#pragma once

#include "requests.h"

void publicate_to_stream(const BusRequest& bus_request, std::ostream& sout);
void publicate_to_stream(const StopRequest& stop_request, std::ostream& sout);
