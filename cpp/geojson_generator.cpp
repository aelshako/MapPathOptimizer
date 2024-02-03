#include <fstream>
#include <iomanip>
#include <iostream>
#include "geojson_generator.hpp"
#include "utils.hpp"


// This test prints 3 points in Palo Alto (near Stanford)
std::string GeojsonGenerator::GenerateGeojsonText(std::vector<Node> points){
    std::string json_text = "";
    if(points.size() >= 2){
        
        // start feature collection
        json_text += "{\"type\": \"FeatureCollection\",\"features\": [ ";

        // add start point
        json_text += "{ \"type\": \"Feature\", \"properties\": {}, \"geometry\": {\"coordinates\": [";
        json_text += std::to_string(points.at(0).lon) + ", " + std::to_string(points.at(0).lat) + "], \"type\": \"Point\"}},";

        // add end point
        json_text += "{ \"type\": \"Feature\", \"properties\": {}, \"geometry\": {\"coordinates\": [";
        json_text += std::to_string(points.back().lon) + ", " + std::to_string(points.back().lat) + "], \"type\": \"Point\"}},";

        // add ways (path) iteratively
        for(int i = 1; i < points.size(); i++){
            json_text += "{ \"type\": \"Feature\", \"properties\": {}, \"geometry\": {\"coordinates\": [[";
            json_text += std::to_string(points.at(i - 1).lon) + ", " + std::to_string(points.at(i - 1).lat) + "], [" + std::to_string(points.at(i).lon) + ", " + std::to_string(points.at(i).lat);
            json_text +=  "]], \"type\": \"LineString\"}}";
            if(i != points.size() - 1){
                json_text += ",";
            }
        }

        // close feature collection
        json_text += "]}";
    }
    return json_text;
}

void GeojsonGenerator::CreateFileFromText(const std::string & file_out_name, std::string data){
    std::ofstream geojson_file(file_out_name + ".geojson");
    geojson_file << data;
    geojson_file.close();
}