#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

struct Node {
    double lat; // latitude
    double lon; // longitude
    std::string node_id; // id of the node
    //int num_related_ways = 0;
    int link_counter = 0;
};

struct Way {
    std::string way_id; // id of the way
    std::vector<Node> nodes_vec; // nodes present in the way
};

// Extracts characters used to build a decimal number(".", "-", digits)
double Parsenumfromstr(std::string & str);

// Extracts digit characters(in order) and uses them to build a string
std::string FilterNumericalString(std::string & str);

// Converts Degree to radian
double Degtorad(double deg);

// computes dist between points(expressed in lat/long)
double ComputeDist(double lat1, double lon1, double lat2, double lon2);

#endif