#include "request_route.h"

using namespace std;

const RequestRouteResult::RequestRouteIterator RequestRouteResult::RequestRouteItems::end() const {
    return RequestRouteResult::RequestRouteIterator(request_route, request_route.result.value().edge_count);
}

RequestRouteItem RequestRouteResult::RequestRouteIterator::operator*() const {
    
    const auto& result_struct = request_route.result.value();
    const auto& edge_id = request_route.p_router->GetRouteEdge(result_struct.id, i);
    const auto& edge = request_route.edges.GetEdge(edge_id);
    
    if (request_route.edge_info.at(edge_id).e_type == RequestRoute::EXCHANGE_TYPE::WAIT) {
        return RequestRouteWaitItem {
            .stop_name = request_route.vertex_indeces[edge.from]->get_name(),
            .time = edge.weight };
    }
    
    return RequestRouteBusItem {
        .bus_name = request_route.edge_info.at(edge_id).bus_name,
        .time = edge.weight,
        .span_count = request_route.edge_info.at(edge_id).span_count};
}
