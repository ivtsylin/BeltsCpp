#include <iomanip>
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
}

template <class T>
Svg::Color BusManager::read_color(const T& color_array) {
    if (color_array.IsString()) {
        return color_array.AsString();
    }
    
    const auto& arr = color_array.AsArray();
    if (arr.size() == 4) {
        return Svg::Rgba({arr[0].AsInt(), arr[1].AsInt(), arr[2].AsInt(), arr[3].AsDouble()});
    }
    if (arr.size() == 3) {
        return Svg::Rgb({arr[0].AsInt(), arr[1].AsInt(), arr[2].AsInt()});
    }
    return Svg::NoneColor;
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
    
    if (root.find("render_settings") == root.end()) return;
    const auto& rsettings = root.at("render_settings").AsMap();
    render_setting.width = rsettings.at("width").AsDouble();
    render_setting.height = rsettings.at("height").AsDouble();
    render_setting.padding = rsettings.at("padding").AsDouble();
    render_setting.stop_radius = rsettings.at("stop_radius").AsDouble();
    render_setting.line_width = rsettings.at("line_width").AsDouble();
    render_setting.stop_label_font_size = rsettings.at("stop_label_font_size").AsInt();
    render_setting.stop_label_offset = Svg::Point{
                                            rsettings.at("stop_label_offset").AsArray()[0].AsDouble(),
                                            rsettings.at("stop_label_offset").AsArray()[1].AsDouble()
                                        };
    
    render_setting.underlayer_color = read_color(rsettings.at("underlayer_color"));
    render_setting.underlayer_width = rsettings.at("underlayer_width").AsDouble();
    
    for (const auto& col_pal : rsettings.at("color_palette").AsArray()) {
        render_setting.color_palette.push_back(read_color(col_pal));
    }
    
    render_setting.bus_label_font_size = rsettings.at("bus_label_font_size").AsInt();
    render_setting.bus_label_offset = Svg::Point{
                                            rsettings.at("bus_label_offset").AsArray()[0].AsDouble(),
                                            rsettings.at("bus_label_offset").AsArray()[1].AsDouble()
                                        };

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
        } else if (stat_req_map.at("type").AsString() == "Map") {
            res["request_id"] = stat_req_map.at("id").AsInt();
            stringstream ss;
            
            ss << fixed << setprecision(7);
            
            ss << "<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" ?>"
                << "<svg xmlns=\\\"http://www.w3.org/2000/svg\\\" version=\\\"1.1\\\">";
            
            double min_lon = stops.get_min_lon();
            double max_lon = stops.get_max_lon();
            double min_lat = stops.get_min_lat();
            double max_lat = stops.get_max_lat();
            
            double zoom_coef;
            
            if (isZero(max_lon - min_lon) && isZero(max_lat - min_lat)) {
                zoom_coef = 0.;
            } else if (isZero(max_lon - min_lon)) {
                zoom_coef = (render_setting.height - 2 * render_setting.padding) / (max_lat - min_lat);
            } else if (isZero(max_lat - min_lat)) {
                zoom_coef = (render_setting.width - 2 * render_setting.padding) / (max_lon - min_lon);
            } else {
                zoom_coef = min((render_setting.width - 2 * render_setting.padding) / (max_lon - min_lon), (render_setting.height - 2 * render_setting.padding) / (max_lat - min_lat));
            }
            
            double &padding = render_setting.padding;
            auto lon_to_x = [&min_lon, &zoom_coef, &padding](double lon){return (lon - min_lon) * zoom_coef + padding;};
            auto lat_to_y = [&max_lat, &zoom_coef, &padding](double lat){return (max_lat - lat) * zoom_coef + padding;};
            
            int color_palette_size = render_setting.color_palette.size();
            int cur_color_num = 0;
            
            // Линии (ломаные) автобусных маршрутов
            
            for (const auto& [_, bus_route] : buses.get_data()) {
                
                auto poly = Svg::Polyline()
                    .SetStrokeColor(render_setting.color_palette[cur_color_num % color_palette_size])
                    .SetStrokeWidth(render_setting.line_width)
                    .SetStrokeLineCap("round")
                    .SetStrokeLineJoin("round");
                
                cur_color_num++;
                
                for (const auto& stop_name : bus_route.get_data()) {
                    poly.AddPoint({
                        lon_to_x(stops.get_data().find(stop_name)->second.get_coordinates().lon),
                        lat_to_y(stops.get_data().find(stop_name)->second.get_coordinates().lat)
                    });
                }
                
                poly.Render(ss);
                
            }
            
            // Названия автобусов.
            
            cur_color_num = 0;
            
            for (const auto& [bus_name, bus_route] : buses.get_data()) {
                
                const string& first_stop = bus_route.get_data().front();
                
                // Подложка
                
                Svg::Text()
                    .SetPoint({
                        lon_to_x(stops.get_data().find(first_stop)->second.get_coordinates().lon),
                        lat_to_y(stops.get_data().find(first_stop)->second.get_coordinates().lat)
                    })
                    .SetOffset(render_setting.bus_label_offset)
                    .SetFontSize(render_setting.bus_label_font_size)
                    .SetFontFamily("Verdana")
                    .SetFontWeight("bold")
                    .SetData(bus_name)
                    .SetFillColor(render_setting.underlayer_color)
                    .SetStrokeColor(render_setting.underlayer_color)
                    .SetStrokeWidth(render_setting.underlayer_width)
                    .SetStrokeLineCap("round")
                    .SetStrokeLineJoin("round")
                    .Render(ss);
                
                // Надпись
                
                Svg::Text()
                    .SetPoint({
                        lon_to_x(stops.get_data().find(first_stop)->second.get_coordinates().lon),
                        lat_to_y(stops.get_data().find(first_stop)->second.get_coordinates().lat)
                    })
                    .SetOffset(render_setting.bus_label_offset)
                    .SetFontSize(render_setting.bus_label_font_size)
                    .SetFontFamily("Verdana")
                    .SetFontWeight("bold")
                    .SetData(bus_name)
                    .SetFillColor(render_setting.color_palette[cur_color_num % color_palette_size])
                    .Render(ss);
                
                if (const string& last_stop = bus_route.get_data()[bus_route.get_last_stop_index()];
                    last_stop != first_stop ) {
                    
                    Svg::Text()
                        .SetPoint({
                            lon_to_x(stops.get_data().find(last_stop)->second.get_coordinates().lon),
                            lat_to_y(stops.get_data().find(last_stop)->second.get_coordinates().lat)
                        })
                        .SetOffset(render_setting.bus_label_offset)
                        .SetFontSize(render_setting.bus_label_font_size)
                        .SetFontFamily("Verdana")
                        .SetFontWeight("bold")
                        .SetData(bus_name)
                        .SetFillColor(render_setting.underlayer_color)
                        .SetStrokeColor(render_setting.underlayer_color)
                        .SetStrokeWidth(render_setting.underlayer_width)
                        .SetStrokeLineCap("round")
                        .SetStrokeLineJoin("round")
                        .Render(ss);
                    
                    // Надпись
                    
                    Svg::Text()
                        .SetPoint({
                            lon_to_x(stops.get_data().find(last_stop)->second.get_coordinates().lon),
                            lat_to_y(stops.get_data().find(last_stop)->second.get_coordinates().lat)
                        })
                        .SetOffset(render_setting.bus_label_offset)
                        .SetFontSize(render_setting.bus_label_font_size)
                        .SetFontFamily("Verdana")
                        .SetFontWeight("bold")
                        .SetData(bus_name)
                        .SetFillColor(render_setting.color_palette[cur_color_num % color_palette_size])
                        .Render(ss);
                    
                }
                
                cur_color_num++;
                
            }
            
            // Круги, обозначающие остановки.
            
            for (const auto& [_, stop_info] : stops.get_data()) {
                Svg::Circle()
                    .SetCenter({
                        lon_to_x(stop_info.get_coordinates().lon),
                        lat_to_y(stop_info.get_coordinates().lat)
                    })
                    .SetRadius(render_setting.stop_radius)
                    .SetFillColor("white")
                    .Render(ss);
            }
            
            // Названия остановок
            
            for (const auto& [stop_name, stop_info] : stops.get_data()) {
                Svg::Text()
                    .SetPoint({
                        lon_to_x(stop_info.get_coordinates().lon),
                        lat_to_y(stop_info.get_coordinates().lat)
                    })
                    .SetOffset(render_setting.stop_label_offset)
                    .SetFontSize(render_setting.stop_label_font_size)
                    .SetFontFamily("Verdana")
                    .SetData(stop_name)
                    .SetFillColor(render_setting.underlayer_color)
                    .SetStrokeColor(render_setting.underlayer_color)
                    .SetStrokeWidth(render_setting.underlayer_width)
                    .SetStrokeLineCap("round")
                    .SetStrokeLineJoin("round")
                    .Render(ss);
                
                Svg::Text()
                    .SetPoint({
                        lon_to_x(stop_info.get_coordinates().lon),
                        lat_to_y(stop_info.get_coordinates().lat)
                    })
                    .SetOffset(render_setting.stop_label_offset)
                    .SetFontSize(render_setting.stop_label_font_size)
                    .SetFontFamily("Verdana")
                    .SetData(stop_name)
                    .SetFillColor("black")
                    .Render(ss);
            }
            
            ss << "</svg>";
            
            res["map"] = ss.str();
            
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

