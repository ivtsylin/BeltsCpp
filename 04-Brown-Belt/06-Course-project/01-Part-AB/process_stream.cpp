#include <sstream>
#include <iomanip>

#include "process_stream.h"
#include "common_stream.h"

using namespace std;

void process_stream(const BusCatalog &bus_catalog, istream& sin, ostream& sout) {
    
    sout << fixed << setprecision(6);
    
    string line;
    getline(sin, line);
    string_view subline, token;
    int count;
    subline = line;
    count = ConvertToInt(ReadToken(subline));
    
    for (int i = 0; i < count; ++i) {
        getline(cin, line);
        subline = line;
        token = ReadToken(subline);
        if (token == "Bus") {
            publicate_to_stream(
                                get_bus_stats(bus_catalog, subline),
                                sout
                                );
        } else if (token == "Stop") {
            publicate_to_stream(
                                get_stop_stats(bus_catalog, subline),
                                sout
                                );
        } else {
            stringstream error;
            error << "string *" << line << "* can not be recognized\n";
            throw invalid_argument(error.str());
        }
    }
}
