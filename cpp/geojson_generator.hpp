#ifndef GEOJSON_GENERATOR_HPP
#define GEOJSON_GENERATOR_HPP

#include "utils.hpp"

class GeojsonGenerator {
    public:
        std::string GenerateGeojsonText(std::vector<Node> points);
        void CreateFileFromText(const std::string & file_out_name, std::string data);
};

#endif