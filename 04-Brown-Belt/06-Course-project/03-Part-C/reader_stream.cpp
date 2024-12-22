#include <sstream>

#include "reader_stream.h"
#include "common_stream.h"

using namespace std;

void read_stop(BusCatalog &bus_catalog, string_view str_in) {
    Stop& stop = bus_catalog.add_stop(
                         string(ReadToken(str_in, ": ")),
                         ConvertToDouble(ReadToken(str_in, ", ")),
                         ConvertToDouble(ReadToken(str_in, ", "))
                         );

    if (str_in != "") {
        string_view token = ReadToken(str_in, ", ");
        while (token != "") {
            double distance = ConvertToDouble(ReadToken(token, "m to "));
            Stop& dst_stop = bus_catalog.get_or_insert_stop(token);
            
            stop.add_distance_to_stop(dst_stop, distance);
            dst_stop.assume_distance_to_stop(stop, distance);
            token = ReadToken(str_in, ", ");
        }
    }
}

void read_bus(BusCatalog &bus_catalog, string_view str_in) {
    string_view bus_name = ReadToken(str_in, ": ");
    string_view delimiter = " - ";
    ROUTE_TYPE rt = ROUTE_TYPE::TWO_WAY;
    if (str_in.find('-') == string::npos) {
        rt = ROUTE_TYPE::CIRCLE;
        delimiter = " > ";
    }
    
    string_view stop_name = ReadToken(str_in, delimiter);
    vector<Stop*> route;
    while (stop_name != "") {
        route.push_back(&bus_catalog.get_or_insert_stop(stop_name));
        stop_name = ReadToken(str_in, delimiter);
    }
    
    bus_catalog.add_bus(string(bus_name), rt, move(route));
}

void read_bus_catalog_from_stream(BusCatalog &bus_catalog, std::istream& is) {
    
    string line;
    string_view subline, token;
    int count;
    
    getline(is, line);
    subline = line;
    count = ConvertToInt(ReadToken(subline));
    
    for (int i = 0; i < count; ++i) {
        getline(is, line);
        subline = line;
        token = ReadToken(subline);
        if (token == "Stop") {
            
            read_stop(bus_catalog, subline);
            
        } else if (token == "Bus") {
            
            read_bus(bus_catalog, subline);
            
        } else {
            stringstream error;
            error << "string *" << line << "* can not be recognized\n";
            throw invalid_argument(error.str());
        }
    }
}

