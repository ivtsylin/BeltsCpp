#pragma once

#include <sstream>

#include "publisher_stream.h"
#include "test_runner.h"

namespace TestStreamPublisher {

void TestPack() {
    {
        BusRequest bus_request;
        bus_request.bus_name = "256";
        bus_request.bus_stats = {6, 5, 4371.02, 1.0};
        
        stringstream sout;
        publicate_to_stream (bus_request, sout);
        
        ASSERT_EQUAL(sout.str(), "Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length, 1 curvature\n");
    }
    {
        BusRequest bus_request;
        bus_request.bus_name = "751";
        
        stringstream sout;
        publicate_to_stream (bus_request, sout);
        
        ASSERT_EQUAL(sout.str(), "Bus 751: not found\n");
    }
    {
        StopRequest stop_request;
        stop_request.stop_name = "Samara";
        
        stringstream sout;
        publicate_to_stream (stop_request, sout);
        
        ASSERT_EQUAL(sout.str(), "Stop Samara: not found\n");
    }
    {
        Stop s("Prazhskaya");
        StopRequest stop_request{s.get_name(), s.get_buses()};
        
        stringstream sout;
        publicate_to_stream (stop_request, sout);
        
        ASSERT_EQUAL(sout.str(), "Stop Prazhskaya: no buses\n");
    }
    {
        Stop s("Biryulyovo Zapadnoye");
        Bus b1("828", ROUTE_TYPE::TWO_WAY, {});
        Bus b2("256", ROUTE_TYPE::TWO_WAY, {});
        s.add_bus(b1);
        s.add_bus(b2);
        
        StopRequest stop_request{s.get_name(), s.get_buses()};
        
        stringstream sout;
        publicate_to_stream (stop_request, sout);
        
        ASSERT_EQUAL(sout.str(), "Stop Biryulyovo Zapadnoye: buses 256 828\n");
    }

}
}


