#include "osm_file_parser.hpp"
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <stdexcept>


bool OsmFileParser::DoesFileExist(const std::string & file_path){
    return true;    // \todo Add logic to use boost fs
}

bool OsmFileParser::ParseFile(const std::string & file_path, int min_way_size /*= 2*/, int min_num_links_per_node /*= 1*/){
    bool is_parse_success = true;
    std::cout << "Map Name: " << file_path << std::endl;

    // open the file
    std::ifstream infile(file_path);
    std::string line;

    Way temp_way;
    reading_way_in_prog = false;

    while (std::getline(infile, line)){
        std::stringstream iss(line);
        std::string temp;
        iss >> temp;
        
        if(temp == "<node"){    // Node flag detected
            Node temp_node = ProcessNode(iss);
            node_vec.push_back(temp_node);
            node_num_links_map[temp_node.node_id] = 0;
            
        }else if(temp == "<way"){   // way flag detected
            reading_way_in_prog = true;
            temp_way.way_id = ReadWayId(iss);

        }else if(reading_way_in_prog && (temp == "<nd")){
            ProcessNodeInWay(iss, temp_way);

        }else if(reading_way_in_prog && (temp == "</way>")){ // end of way detected
            if(temp_way.nodes_vec.size() >= min_way_size){ // todo: add additional validation checks here
                way_vec.push_back(temp_way);
                reading_way_in_prog = false;

                // clear temp way
                temp_way.way_id = "";
                temp_way.nodes_vec.clear();
            }
        }
    }
        infile.close();

       CleanNodesVec();

    return is_parse_success;

}

Node OsmFileParser::ProcessNode(std::stringstream & iss){
    // keep reading until all required fields are obtained(id, lat, long) or end is reached(exception case)
    bool id_found = false;
    std::string curr_node_id = "";
    bool lat_found = false;
    std::string curr_node_lat = "";
    bool lon_found = false;
    std::string curr_node_lon = "";
    std::string temp = "";

    Node new_node;

    while(iss >> temp){ // reading node line
        if(temp.rfind("id=", 0) == 0){
            id_found = true;
            curr_node_id = temp;
        }else if(temp.rfind("lat=", 0) == 0){
            lat_found = true;
            curr_node_lat = temp;
        }else if(temp.rfind("lon=", 0) == 0){
            lon_found = true;
            curr_node_lon = temp;
        }
    }

    if(id_found && lat_found && lon_found){ // valid point found
        new_node.node_id = FilterNumericalString(curr_node_id);
        new_node.lat = Parsenumfromstr(curr_node_lat);
        new_node.lon = Parsenumfromstr(curr_node_lon);
        new_node.link_counter = 0;

    }else{
        throw std::runtime_error("Error! - Detected Incomplete Node in input OSM file.");
    }

    return new_node;
            
}

std::string OsmFileParser::ReadWayId(std::stringstream & iss){
    std::string temp_str = "";
    std::string way_id = "";
    bool successful_read = false;
    iss >> temp_str;
    if(temp_str.rfind("id=", 0) == 0){
        successful_read = true;
        way_id = FilterNumericalString(temp_str);
    }

    if(!successful_read){
        throw std::runtime_error("Error! - Detected Incomplete Way (missing ID) in input OSM file.");
    }
    return way_id;
}

void OsmFileParser::ProcessNodeInWay(std::stringstream & iss, Way & way){
    std::string node_id = "";
    std::string temp = "";
    bool node_exists = false;
    bool node_id_found = false;
    iss >> temp;

    if(temp.rfind("ref=", 0) == 0){
        node_id = FilterNumericalString(temp);
        node_id_found = true;
    }
    Node temp_node;
    for(Node & curr : node_vec){ // traverse the list to find the node, change this to hashmap later
        if(curr.node_id == node_id){
            node_exists = true;
            temp_node.lat = curr.lat;
            temp_node.lon = curr.lon;
            temp_node.node_id = node_id;
            curr.link_counter++;
            temp_node.link_counter = curr.link_counter;
            node_num_links_map[node_id]++;
            break;
        }
    }

    way.nodes_vec.push_back(temp_node);

    if(!node_id_found){
        std::string err_msg = "Error! - Node with undefined id is referenced in Way with id = " + way.way_id;
        throw std::runtime_error(err_msg);
    }

    if(!node_exists){
        std::string err_msg = "Error! - Node with id = " + node_id + " is referenced in Way with id = " + way.way_id + ", but doesn't exist in input file.";
        throw std::runtime_error(err_msg);
    }
    
}

void OsmFileParser::CleanNodesVec(){
    std::vector<Node> temp_cleaned_vec;

    for(int i = 0; i < node_vec.size(); i++){
        if(node_vec.at(i).link_counter >= 1){
            temp_cleaned_vec.push_back(node_vec.at(i));
        }
    }
    node_vec = temp_cleaned_vec;
}

void OsmFileParser::CleanWayVec(){ // addresses parallel paths
    // for every way:
    for(int j = 0; j < way_vec.size(); j++){
        for(int i = 0; i < way_vec.at(j).nodes_vec.size(); i++){
            if((i != 0) && (i != way_vec.at(j).nodes_vec.size() - 1)){
                auto curr_node = way_vec.at(j).nodes_vec.at(i);
                if(node_num_links_map[curr_node.node_id] > 1){
                    // Way splitting case
                    // Example: way 1(n0, n1, n2, n3), way2(n7,n2,n3) ==> {way1 becomes way1a(n0, n1, n2), way1b(n2, n3)} and {way2a(n7,n2),way2b(n2,n3)}
                    std::vector<Node>::const_iterator first = way_vec.at(j).nodes_vec.begin() + i;
                    std::vector<Node>::const_iterator last = way_vec.at(j).nodes_vec.end();
                    std::vector<Node> node_vec_second_half(first, last);
                    Way second_way_half;
                    second_way_half.nodes_vec = node_vec_second_half;
                    way_vec.at(j).nodes_vec.erase(way_vec.at(j).nodes_vec.begin() + i + 1, way_vec.at(j).nodes_vec.end());
                    way_vec.insert(way_vec.begin() + j + 1, second_way_half);
                }
            }
        }
    }
}

std::vector<Node> OsmFileParser::getNodes(){
    return node_vec;
}

std::vector<Way> OsmFileParser::getWays(){
    return way_vec;
}
