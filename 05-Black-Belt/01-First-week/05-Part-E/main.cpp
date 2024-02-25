#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include "test_runner.h"
#include "json.h"
#include "busmanager.h"

using namespace std;

string problem_str;

void UnitTest() {
}

int main() {
    TestRunner tr;
    BusManager bus_manager;
    //RUN_TEST(tr, UnitTest);
    try {
        cout << fixed << setprecision(7);
        //cout << "START" << endl;
        Json::Document doc = Json::Load(cin);
        //cout << "LOAD JSON FINISHED" << endl;
        //cout << doc << endl;
        problem_str = "Read Json";
        bus_manager.read_bus_catalog(doc);
        problem_str = "Read Bus Catalog";
        //cout << "BUS_MAMAGER_CATALOG HAS BUILT" << endl;
        //cout << bus_manager << endl;
        cout << bus_manager.process_and_print_requests(doc) << endl;
        
    } catch (...) {
        throw invalid_argument(problem_str);
    }
    return 0;
}
