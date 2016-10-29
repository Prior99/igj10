#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <limits>
#include <iostream>

int compareDoubles (double a, double b){
    if (abs(a - b) < std::numeric_limits<double>::epsilon()){
        return 0;
    }
    else if (a < b){
        return -1;
    }
    else{
        return 1;
    }
}

#endif
