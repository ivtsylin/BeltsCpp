#pragma once

#include <memory>
#include <array>
#include "buses.h"
#include "graph.h"
#include "router.h"
#include "svg.h"

enum class EXCHANGE_TYPE {
    BUS,
    WAIT
};

class BusManager {
private:
    Buses buses;
    Stops stops;
    double bus_wait_time;
    double bus_velocity;
    const std::string empty_bus = "";
    
    struct RenderSettings {
        double width;
        double height;
        double padding;
        double stop_radius;
        double line_width;
        int stop_label_font_size;
        Svg::Point stop_label_offset;
        Svg::Color underlayer_color;
        double underlayer_width;
        std::vector<Svg::Color> color_palette;
        int bus_label_font_size;
        Svg::Point bus_label_offset;
        std::vector<std::string> layers;
    } render_setting;
    
    struct EdgeInfo {
        EXCHANGE_TYPE e_type = EXCHANGE_TYPE::WAIT;
        std::string_view bus_name;
        int span_count = 0;
    };
    
    using VertexId = size_t;
    std::vector<std::string_view> vertex_indeces;
    std::unordered_map<std::string_view, VertexId> terminal_vertex_indeces;
    std::unordered_map<Graph::EdgeId, EdgeInfo> edge_info;
    
    std::unique_ptr<Graph::DirectedWeightedGraph<double>> p_edges;
    
    void init_graph();
    
    template <class T>
    static Svg::Color read_color(const T& color_array);
    
public:
    friend std::ostream& operator<< (std::ostream& os, const BusManager& bm);
    void read_bus_catalog(const Json::Document& doc);
    Json::Document process_and_print_requests(const Json::Document& doc);
};

std::ostream& operator<< (std::ostream& out, const BusManager& bus_man);
