#include <gtest/gtest.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cctype>
#include <stdexcept>

#include "osm_routing.hpp"
#include "geojson_generator.hpp"
#include "utils.hpp"

TEST(GeoJsonGeneratorTests, PaloAltoTest){
    Node a;
    Node b;
    Node c;

    a.lon = -122.15238595412501;
    a.lat = 37.42398098920238;

    b.lon = -122.14844847129115;
    b.lat = 37.42481598531876;

    c.lon = -122.14606666968574;
    c.lat = 37.42576173497942;

    std::vector<Node> node_vec;
    node_vec.push_back(a);
    node_vec.push_back(b);
    node_vec.push_back(c);

    GeojsonGenerator g;
    std::string json_text = g.GenerateGeojsonText(node_vec);
    std::string expected_json = "{\"type\": \"FeatureCollection\",\"features\": [ { \"type\": \"Feature\", \"properties\": {}, \"geometry\": {\"coordinates\": [-122.152386, 37.423981], \"type\": \"Point\"}},{ \"type\": \"Feature\", \"properties\": {}, \"geometry\": {\"coordinates\": [-122.146067, 37.425762], \"type\": \"Point\"}},{ \"type\": \"Feature\", \"properties\": {}, \"geometry\": {\"coordinates\": [[-122.152386, 37.423981], [-122.148448, 37.424816]], \"type\": \"LineString\"}},{ \"type\": \"Feature\", \"properties\": {}, \"geometry\": {\"coordinates\": [[-122.148448, 37.424816], [-122.146067, 37.425762]], \"type\": \"LineString\"}}]}";
    
    // removing whitespace from both strings to allow for comparison
    json_text.erase(remove_if(json_text.begin(), json_text.end(), isspace), json_text.end());
    expected_json.erase(remove_if(expected_json.begin(), expected_json.end(), isspace), expected_json.end());

    g.CreateFileFromText("geo", json_text);
    
    ASSERT_STREQ(json_text.c_str(), expected_json.c_str());

}

TEST(OsmRoutingTests, InvalidSettingOfCostMap){
    OsmRouting osm_router(false); // testing mode disabled
    bool exception_trigged = false;

    std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> empty_costmap;
    try{
        osm_router.SetEdgeCostMap(empty_costmap);
    }catch(std::invalid_argument & e){
        exception_trigged = true;
    }

    ASSERT_EQ(exception_trigged, true);
}

TEST(OsmRoutingTests, RoutingTest1) {

    OsmRouting osm_router(true);

    // creating test nodes:
    Node a,b,c,d;
    a.node_id = "a";
    b.node_id = "b";
    c.node_id = "c";
    d.node_id = "d";
    std::vector<Node> node_vec = {a,b,c,d};
    std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> edge_costmap;

    /* test map: (acd is minimum path since 7 < 9)
           b
        2 /\ 7
       a |  |d
         3\/4
          c
    */
    edge_costmap = {{"a", {{"b", 2}, {"c", 3}} }, 
                    {"b", {{"d", 7}}},
                    {"c", {{"d", 4}}}};
    osm_router.SetEdgeCostMap(edge_costmap);
    osm_router.node_vec = node_vec;
    
    std::vector<Node> result_route = osm_router.route(a, d);
    ASSERT_EQ(result_route.size(), 3);
    std::string expected_first_node = "a";
    std::string expected_second_node = "c";
    std::string expected_third_node = "d";

    ASSERT_STREQ(result_route.at(0).node_id.c_str(), expected_first_node.c_str());
    ASSERT_STREQ(result_route.at(1).node_id.c_str(), expected_second_node.c_str());
    ASSERT_STREQ(result_route.at(2).node_id.c_str(), expected_third_node.c_str());

}
