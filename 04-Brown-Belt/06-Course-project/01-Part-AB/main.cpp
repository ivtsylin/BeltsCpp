#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <iterator>

#include "stop.h"
#include "bus.h"
#include "bus_catalog.h"
#include "reader_stream.h"
#include "requests.h"
#include "publisher_stream.h"
#include "process_stream.h"

#include "test_runner.h"
#include "test_stop.h"
#include "test_bus.h"
#include "test_bus_catalog.h"
#include "test_reader_stream.h"
#include "test_requests.h"
#include "test_publisher_stream.h"

using namespace std;

int main() {
    
    TestRunner tr;
    RUN_TEST(tr, TestStop::TestPack);
    RUN_TEST(tr, TestBus::TestPack);
    RUN_TEST(tr, TestBusCatalog::TestPack);
    RUN_TEST(tr, TestStreamReader::TestPack);
    RUN_TEST(tr, TestRequests::TestPack);
    RUN_TEST(tr, TestStreamPublisher::TestPack);
    
    BusCatalog bus_catalog;
    read_bus_catalog_from_stream(bus_catalog, cin);
    process_stream(bus_catalog, cin, cout);
    
    //cout << bus_catalog << endl;
    
    return 0;
}
