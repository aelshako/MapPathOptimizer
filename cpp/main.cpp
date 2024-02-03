#include <vector>
#include <string>
#include <iostream>

#include "osm_file_parser.hpp"
#include "osm_routing.hpp"
#include "geojson_generator.hpp"


int main(){
    // prompt the user for input (they should input the map path and start/end node)
    std::string osm_file_path;
    std::string start_node_id;
    std::string end_node_id;

    // \todo Add input validation support
    std::cout << "Enter the osm file name(including \".osm\"): ";
    std::cin >> osm_file_path;

    std::cout << "Enter the start node's id (it must exist in map)): ";
    std::cin >> start_node_id;

    std::cout << "Enter the end node's id (it must exist in map)): ";
    std::cin >> end_node_id;

    OsmFileParser parser_obj;
    OsmRouting osm_router(false);

    // Parse the osm File
    parser_obj.ParseFile(osm_file_path, 2, 1);

    osm_router.node_vec = parser_obj.getNodes();

    osm_router.way_vec = parser_obj.getWays();
    
    Node start;
    Node end;

    for(auto n : osm_router.node_vec){
         if(n.node_id == start_node_id){
             start = n;
         }
         if(n.node_id == end_node_id){
             end = n;
     }
    }

    auto result_path_vec = osm_router.route(start, end);

    GeojsonGenerator g;
    std::string json_text = g.GenerateGeojsonText(result_path_vec);
    g.CreateFileFromText("geo" , json_text); // outputs to a "geo.geojson" file in working dir
    
    return 0;
    }
