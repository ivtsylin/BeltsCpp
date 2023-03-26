#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main() {
    int q;
    cin >> q;
    map<string, vector<string>> buses, stops;
    string cmd;
    for (int i = 0; i < q; ++i) {
        cin >> cmd;
        if (cmd == "NEW_BUS"){
            string bus;
            int stop_count;
            cin >> bus >> stop_count;
            string stop;
            for (int j = 0; j < stop_count; ++j) {
                cin >> stop;
                buses[bus].push_back(stop);
                stops[stop].push_back(bus);
            }
        } else if (cmd == "BUSES_FOR_STOP") {
            string stop;
            cin >> stop;
            if (stops.count(stop)) {
                for (const auto& bus : stops[stop]) {
                    cout << bus << " ";
                }
                cout << endl;
            } else {
                cout << "No stop" << endl;
            }
        } else if (cmd == "STOPS_FOR_BUS") {
            string bus;
            cin >> bus;
            if (buses.count(bus)) {
                for (const auto& stop: buses[bus]) {
                    cout << "Stop " << stop << ": ";
                    if (stops[stop].size() == 1) {
                        cout << "no interchange" << endl;
                    } else {
                        for (const auto& bus_item: stops[stop]) {
                            if (bus_item != bus) {
                                cout << bus_item << " ";
                            }
                        }
                        cout << endl;
                    }
                }
            } else {
                cout << "No bus" << endl;
            }
        } else { // cmd == ALL_BUSES
            if (buses.size()) {
                for (const auto& [key, value] : buses){
                    cout << "Bus " << key << ":";
                    for (const auto& stop : value) {
                        cout << " " << stop;
                    }
                    cout << endl;
                }
            } else {
                cout << "No buses" << endl;;
            }
        }
    }
    return 0;
}
