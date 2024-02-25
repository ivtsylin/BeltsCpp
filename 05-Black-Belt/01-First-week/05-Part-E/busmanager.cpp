#include "busmanager.h"
#include "test_runner.h"

using namespace std;

ostream& operator<< (ostream& sout, const Graph::Edge<double>& edge) {
    sout << "{from: " << edge.from << ", to = " << edge.to << ", weight = " << edge.weight << "}";
    return sout;
}

void BusManager::init_graph() {
    
    p_edges = make_unique<Graph::DirectedWeightedGraph<double>>(stops.get_data().size() * 2);
    
    for (const auto& [stop, _] : stops.get_data()) {
        uint32_t i = vertex_indeces.size();
        vertex_indeces.push_back(stop);
        terminal_vertex_indeces[stop] = i;
        vertex_indeces.push_back(stop);
        auto edge_id = p_edges->AddEdge({i, i + 1, bus_wait_time});
        edge_info[edge_id] = {EXCHANGE_TYPE::WAIT};
    }
    
    //cout << vertex_indeces << endl;
    //cout << terminal_vertex_indeces << endl;
    
    for (const auto& [bus_name, bus_info] : buses.get_data()) {
        const auto& stop_name_vec = bus_info.get_data();
        for (int i = 0; i + 1 < stop_name_vec.size(); ++i) {
            double weight = 0.;
            for (int j = i + 1; j < stop_name_vec.size(); ++j) {
                weight += stops.find_real_distance(stop_name_vec[j - 1], stop_name_vec[j]) / 1000. / bus_velocity * 60;
                auto edge_id = p_edges->AddEdge({
                    terminal_vertex_indeces[stop_name_vec[i]] + 1,
                    terminal_vertex_indeces[stop_name_vec[j]],
                    weight
                });
                edge_info[edge_id] = {EXCHANGE_TYPE::BUS, bus_name, j - i};
            }
        }
    }
    /*
    cout << "GRAPH" << endl;
    for (const auto& item : p_edges.get()->edges_) {
        cout << "{from: " << vertex_indeces[item.from].stop_name << "+" << vertex_indeces[item.from].bus_name << "" << ", to = " << vertex_indeces[item.to].stop_name << "+" << vertex_indeces[item.to].bus_name << ", weight = " << item.weight << endl;
    }
    cout << endl;
     */
}

void BusManager::read_bus_catalog(const Json::Document& doc) {
    const auto& root = doc.GetRoot().AsMap();
    
    if (root.find("routing_settings") == root.end()) return;
    const auto& settings = root.at("routing_settings").AsMap();
    bus_wait_time = settings.at("bus_wait_time").AsDouble();
    bus_velocity = settings.at("bus_velocity").AsDouble();
    
    if (root.find("base_requests") == root.end()) return;
    const auto& requests = root.at("base_requests").AsArray();
    for (const auto& request : requests) {
        if (request.AsMap().at("type").AsString() == "Stop") {
            stops.read_stop_and_coordinates(request);
        } else if (request.AsMap().at("type").AsString() == "Bus") {
            buses.read_bus_and_bus_route(request, stops);
        } else {
            std::stringstream error;
            error << "string *" << request.AsMap().at("type").AsString() << "* can not be recognized\n";
            throw std::invalid_argument(error.str());
        }
    }
    //cout << buses << endl;
    //cout << stops << endl;
    init_graph();
}

Json::Document BusManager::process_and_print_requests(const Json::Document& doc) {
    const auto& root_map = doc.GetRoot().AsMap();
    vector<Json::Node> output;
    if (root_map.find("stat_requests") == root_map.end()) return Json::Document(Json::Node(std::move(output)));
    
    const auto& stat_requests = root_map.at("stat_requests").AsArray();
    
    Graph::Router router(*p_edges.get());
    
    
    for (const auto& stat_req : stat_requests) {
        
        const auto& stat_req_map = stat_req.AsMap();
        map<string, Json::Node> res;
        if (stat_req_map.at("type").AsString() == "Bus") {
            auto bus_stats = buses.get_bus_stats(stat_req_map.at("name").AsString(), stops);
            res["request_id"] = stat_req_map.at("id").AsInt();
            if(bus_stats) {
                res["route_length"] = bus_stats.value().route_real_length;
                res["curvature"] = bus_stats.value().curvature;
                res["stop_count"] = bus_stats.value().stops_count;
                res["unique_stop_count"] = bus_stats.value().unique_stops_count;
            } else {
                res["error_message"] = string("not found");
            }
        } else if (stat_req_map.at("type").AsString() == "Stop") {
            auto stop_request_result = stops.find_stop_and_get_buses(stat_req_map.at("name").AsString());
            res["request_id"] = stat_req_map.at("id").AsInt();
            if(stop_request_result) {
                vector<Json::Node> buses_output;
                for (const auto& bus_output : *(stop_request_result.value())) {
                    buses_output.push_back(bus_output);
                }
                res["buses"] = move(buses_output);
            } else {
                res["error_message"] = string("not found");
            }
        } else if (stat_req_map.at("type").AsString() == "Route") {
            res["request_id"] = stat_req_map.at("id").AsInt();
            const string& stop_from = stat_req_map.at("from").AsString();
            const string& stop_to = stat_req_map.at("to").AsString();
            
            auto result = router.BuildRoute(terminal_vertex_indeces[stop_from], terminal_vertex_indeces[stop_to]);
            
            if (result) {
                vector<Json::Node> res_items;
                const auto& result_struct = result.value();
                res["total_time"] = result_struct.weight;
            
                for (size_t i = 0; i < result_struct.edge_count; ++i) {
                    const auto& edge_id = router.GetRouteEdge(result_struct.id, i);
                    const auto& edge = p_edges->GetEdge(edge_id);
                    map<string, Json::Node> res_item;
                    res_item["time"] = edge.weight;
                    if (edge_info[edge_id].e_type == EXCHANGE_TYPE::WAIT) {
                        res_item["stop_name"] = string(vertex_indeces[edge.from]);
                        res_item["type"] = string("Wait");
                    } else {
                        res_item["type"] = string("Bus");
                        res_item["span_count"] = edge_info[edge_id].span_count;
                        res_item["bus"] = string(edge_info[edge_id].bus_name);
                    }
                    res_items.push_back(move(res_item));
                }
                res["items"] = move(res_items);
                router.ReleaseRoute(result_struct.id);
            } else {
                res["error_message"] = string("not found");
            }
        } else {
            string err = "Invalid type of stat request/n";
            throw invalid_argument(err);
        }
        output.emplace_back(move(res));
    }
    return Json::Document(Json::Node(std::move(output)));
}

ostream& operator<< (ostream& out, const BusManager& bus_man) {
    out << bus_man.stops << endl;
    out << bus_man.buses << endl;
    return out;
}

