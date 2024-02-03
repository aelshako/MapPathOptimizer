#ifndef OSM_ROUTING_HPP
#define OSM_ROUTING_HPP

#include <string>
#include <unordered_map>
#include <limits>
#include "utils.hpp"

// Performs comparison between pair of <std::string node, double cost>
// used as comparator for priority queue
struct Compare{
    constexpr bool operator()(
            std::pair<std::string, double> const& a, 
            std::pair<std::string, double> const &b)
            const noexcept{
        return (a.second > b.second);
    }
};

class OsmRouting {
    public:
        OsmRouting(bool is_in_testing_mode);
        std::vector<Node> node_vec;
        std::vector<Way> way_vec;
        std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> edge_costmap;
        std::vector<Node> route(const Node& start, const Node& end);
        void ComputeEdgeCostMap();
        void SetEdgeCostMap(std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> edge_costmap); // used for unit testing only
private:
    bool test_mode_enabled = false; 
};

#endif
