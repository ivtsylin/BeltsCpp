#pragma once

#include "publisher_to_json.h"
#include "test_runner.h"

namespace TestPublisherToJson {

void TestPack() {
    {
        BusRequest bus_request;
        bus_request.bus_stats = {6, 5, 4371.02, 1.0};
        
        json::Node node;
        publicate_to_json (bus_request, node);
        
        ASSERT(node.IsMap());
        ASSERT_EQUAL(node.AsMap().at("stop_count").AsInt(), 6);
        ASSERT_EQUAL(node.AsMap().at("unique_stop_count").AsInt(), 5);
        ASSERT_EQUAL(node.AsMap().at("route_length").AsDouble(), 4371.02);
        ASSERT_EQUAL(node.AsMap().at("curvature").AsDouble(), 1.0);
    }
    {
        BusRequest bus_request;
        
        json::Node node;
        publicate_to_json (bus_request, node);
        
        ASSERT(node.IsMap());
        ASSERT_EQUAL(node.AsMap().at("error_message").AsString(), "not found");
    }
    {
        StopRequest stop_request;
        
        json::Node node;
        publicate_to_json (stop_request, node);
        
        ASSERT(node.IsMap());
        ASSERT_EQUAL(node.AsMap().at("error_message").AsString(), "not found");
    }
    {
        Stop s("Prazhskaya");
        StopRequest stop_request{s.get_buses()};
        
        json::Node node;
        publicate_to_json (stop_request, node);
        
        ASSERT(node.IsMap());
        ASSERT(node.AsMap().at("buses").AsArray().empty());
    }
    {
        Stop s("Biryulyovo Zapadnoye");
        Bus b1("828", ROUTE_TYPE::TWO_WAY, {});
        Bus b2("256", ROUTE_TYPE::TWO_WAY, {});
        s.add_bus(b1);
        s.add_bus(b2);
        
        StopRequest stop_request{s.get_buses()};
        
        json::Node node;
        publicate_to_json (stop_request, node);
        
        ASSERT(node.IsMap());
        ASSERT_EQUAL(node.AsMap().at("buses").AsArray()[0], "256");
        ASSERT_EQUAL(node.AsMap().at("buses").AsArray()[1], "828");
    }
}
}

