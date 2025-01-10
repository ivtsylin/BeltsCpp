#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <iterator>

#include "stop.h"
#include "bus.h"
#include "bus_catalog.h"
#include "reader_from_json.h"
#include "request_stat.h"
#include "request_route.h"
#include "publisher_to_json.h"
#include "process_to_json.h"
#include "json_node.h"
#include "json_node_writer.h"
#include "json_node_reader.h"

#include "test_runner.h"
#include "test_stop.h"
#include "test_bus.h"
#include "test_bus_catalog.h"
#include "test_reader_from_json.h"
#include "test_request_stat.h"
#include "test_request_route.h"
#include "test_publisher_to_json.h"
#include "test_json_node.h"
#include "test_json_node_writer.h"
#include "test_json_node_reader.h"


using namespace std;
using namespace json;

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestStop::TestPack);
    RUN_TEST(tr, TestBus::TestPack);
    RUN_TEST(tr, TestBusCatalog::TestPack);
    RUN_TEST(tr, TestRequestStat::TestPack);
    
    RUN_TEST(tr, TestJsonNode::TestPack);
    RUN_TEST(tr, TestJsonNodeWriter::TestPack);
    RUN_TEST(tr, TestJsonNodeReader::TestPackCorrect);
    RUN_TEST(tr, TestJsonNodeReader::TestPackIncorrect);
    
    RUN_TEST(tr, TestReaderFromJson::TestPack);
    RUN_TEST(tr, TestPublisherToJson::TestPack);
    
    BusCatalog bus_catalog;
    json::Node document;
    json::Node responses;
    
    cin >> document;
    read_bus_catalog_from_json(
                               bus_catalog,
                               document.AsMap().at("base_requests").AsArray()
                               );
    
    process_to_json(
                    bus_catalog,
                    document,
                    responses);
    
    cout << fixed << setprecision(7);
    
    cout << responses;
    
    return 0;
}
