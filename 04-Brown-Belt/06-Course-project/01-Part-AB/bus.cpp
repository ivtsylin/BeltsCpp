#pragma once

#include "bus.h"
#include "test_runner.h"

std::ostream& operator<<(std::ostream& output, const Bus &bus) {
    output << bus.get_name() << ": " << (bus.is_circle() ? "ROUND" : "TWO_WAY") << " {";
    for (const auto& stop : bus.get_route()) {
        output << stop->get_name() << ", ";
    }
    output << "}";
    return output;
}
