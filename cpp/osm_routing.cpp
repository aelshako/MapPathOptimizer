#include <vector>
#include <string>
#include <iostream>
#include <cctype>
#include <unordered_map>
#include <cmath>
#include <queue>
#include <limits>
#include <algorithm>
#include <stdexcept>

#include "osm_routing.hpp"
#include "geojson_generator.hpp"

    OsmRouting::OsmRouting(bool is_in_testing_mode){
        test_mode_enabled = is_in_testing_mode;
    }

    // this function is used only for unit testing
    void OsmRouting::SetEdgeCostMap(std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> map){
        // safety check to ensure this is not accidentally used outside of unit testing
        if(test_mode_enabled){
            edge_costmap = map;
        }else{
            throw std::invalid_argument("Error! - Edge Map cannot be set directly, unless in testing mode.");
        }
    }

    void OsmRouting::ComputeEdgeCostMap(){
        // build all edges
        for(auto w : way_vec){ //traverse list of ways
            for(int i = 1; i < w.nodes_vec.size(); i++){
                Node prev_node = w.nodes_vec.at(i-1);
                Node curr_node = w.nodes_vec.at(i);
                
                double dist = ComputeDist(prev_node.lat, prev_node.lon, curr_node.lat, curr_node.lon);
                
                // check if node is present in map
                auto is_found = edge_costmap.find(prev_node.node_id);
                if(is_found != edge_costmap.end()){
                    edge_costmap[prev_node.node_id].push_back(std::make_pair(curr_node.node_id, dist));
                }else{ // not present ==> create vector
                    std::vector<std::pair<std::string, double>> vec;
                    vec.push_back(std::make_pair(curr_node.node_id, dist));
                    edge_costmap[prev_node.node_id] = vec;
                }
            }
        }
            
    }

    std::vector<Node> OsmRouting::route(const Node& start, const Node& end) {
            if(!test_mode_enabled){
                ComputeEdgeCostMap();
            }
            std::priority_queue<std::pair<std::string, double>, std::vector<std::pair<std::string, double>>, Compare> pq;
            std::unordered_map<std::string, Node> nodeid_to_obj_map;
            std::unordered_map<std::string, std::pair<double, std::string>> node_dist_map; // second pair value notes the predecessor
            for(int i = 0; i < node_vec.size();i++){
                double dist_val = std::numeric_limits<double>::max();
                if(node_vec[i].node_id == start.node_id){
                    dist_val = 0;
                }
                node_dist_map[node_vec[i].node_id] = std::make_pair(dist_val, "");
                nodeid_to_obj_map[node_vec[i].node_id] = node_vec[i];
            }
            pq.push({start.node_id, 0});

            while(!pq.empty()){
                int dis = pq.top().second;
                std::string node = pq.top().first;
                pq.pop();

                for(auto neighbor_node_pair : edge_costmap[node]){ /// \todo: add a validation check here
                    double edge_cost = neighbor_node_pair.second;
                    std::string neighbor_id = neighbor_node_pair.first;

                    if(dis + edge_cost < node_dist_map[neighbor_id].first){
                        node_dist_map[neighbor_id].first = dis + edge_cost;
                        node_dist_map[neighbor_id].second = node;   // setting the prev node
                        pq.push(std::make_pair(neighbor_id, dis + edge_cost));
                    }
                }
            }

            std::string temp = end.node_id;
            std::string prev = node_dist_map[end.node_id].second;
            std::vector<Node> result_vec;
            result_vec.push_back(nodeid_to_obj_map[end.node_id]);

            while(prev != ""){
                temp += " " + prev;
                result_vec.push_back(nodeid_to_obj_map[prev]);
                prev = node_dist_map[prev].second;
            }

            std::reverse(result_vec.begin(), result_vec.end());
            std::cout << "Shortest Route Nodes in order: ";
            for(int i = 0; i < result_vec.size(); i++){
                std::cout << result_vec[i].node_id + " ";
            }


            std::cout << "\nShortest Route Distance from start to end = " << node_dist_map[end.node_id].first << " meters" << std::endl;     

            return result_vec;
        }

