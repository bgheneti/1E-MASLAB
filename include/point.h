#ifndef INCLUDE_POINT_H_
#define INCLUDE_POINT_H_

#include <string>
#include <cmath>
#include <sstream>

namespace utils {
    // This class represents point on a grid
    struct Point {
        int x;
        int y;
        Point() : x(-7) , y(-7) {}
        Point(int x, int y) : x(x), y(y) {}
        Point(double x, double y) : x(round(x)), y(round(y)) {};
        
        // Convert a point from map format to internal format
        void convertToInternal(int minX, int minY) {
            x = (x-minX)*6.096;
            y = (y-minY)*6.096;
        }
        
        // Return the string representation of the point
        std::string toString() {
            std::ostringstream stringStream;
            stringStream << "(" << x << ", " << y << ")";
            return stringStream.str();
        }
    };
}

#endif
