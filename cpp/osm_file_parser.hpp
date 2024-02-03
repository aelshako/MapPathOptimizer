#ifndef OSM_FILE_PARSER_HPP
#define OSM_FILE_PARSER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "utils.hpp"

class OsmFileParser {
private:
    std::string file_path;
    std::unordered_map<std::string, int> node_num_links_map; 
    std::vector<Node> node_vec;
    std::vector<Way> way_vec;
    bool reading_way_in_prog = false;
    Node ProcessNode(std::stringstream & iss);
    std::string ReadWayId(std::stringstream & iss);
    void ProcessNodeInWay(std::stringstream & iss, Way & way);
    void CleanNodesVec();
    void CleanWayVec();
    bool DoesFileExist(const std::string & file_path);
public:
    std::vector<Node> getNodes();
    std::vector<Way> getWays();
    bool ParseFile(const std::string & file_path, int min_way_size = 2, int min_num_links_per_node = 1);
};

#endif