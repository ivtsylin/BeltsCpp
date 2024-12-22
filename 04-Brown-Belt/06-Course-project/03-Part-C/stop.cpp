#pragma once

#include "stop.h"
#include "test_runner.h"

std::ostream& operator<<(std::ostream& output, const Stop &stop) {
    output << stop.get_name() << ": <{" << stop.get_point() << "}, {" << stop.get_distance_data() << "}>";
    return output;
}
