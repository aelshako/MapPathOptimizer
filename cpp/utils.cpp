#include "utils.hpp"
#include <string>
#include <cctype>
#include <cmath>

double Parsenumfromstr(std::string & str){
    double result = 0.0;
    std::string num_str = "";
    for(char c : str){
        if(std::isdigit(c) || (c == '-') || (c == '.')){
            num_str += c;
        }
    }
    return std::stod(num_str);
}

std::string FilterNumericalString(std::string & str){
    // add validation here
    std::string num_str = "";
    for(char c : str){
        if(std::isdigit(c)){
            num_str += c;
        }
    }
    return num_str;
}

double Degtorad(double deg){
    return ((deg * M_PI) /180.0);
}

// computes dist between points(expressed in lat/long)
double ComputeDist(double lat1, double lon1, double lat2, double lon2){
    double  lat_new = Degtorad(lat1);
    double  lat_old = Degtorad(lat2);
    double  lat_diff = Degtorad(lat2-lat1);
    double  lng_diff = Degtorad(lon2-lon1);

    double  a = std::sin(lat_diff/2) * std::sin(lat_diff/2) +
                std::cos(lat_new) * std::cos(lat_old) *
                std::sin(lng_diff/2) * std::sin(lng_diff/2);
    double  c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));

    double  distance = 6372797.56085 * c;

    if(distance < 0){
        /// \todo: Throw exception?
    }
    
    return distance;
}