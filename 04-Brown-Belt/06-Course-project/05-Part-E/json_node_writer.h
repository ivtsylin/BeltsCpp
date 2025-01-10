#pragma once

#include <ostream>
#include <string>
#include "json_node.h"

namespace json {

std::ostream &operator<<(std::ostream &out, const Node &node);

}
