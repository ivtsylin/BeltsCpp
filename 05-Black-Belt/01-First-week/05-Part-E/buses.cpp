#include "buses.h"
#include "test_runner.h"

using namespace std;

ostream& operator<<(ostream& s_out, const BusRoute& bus_route) {
    s_out << "<" << bus_route.get_data() << ">";
    return s_out;
}

ostream& operator<<(ostream& s_out, const Buses& buses) {
    s_out << buses.get_data();
    return s_out;
}
