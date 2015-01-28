#ifndef INCLUDE_POINT_H_
#define INCLUDE_POINT_H_

#include <string>
#include <cmath>
#include <sstream>
#include <climits>
#include <vector>

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
        
        bool equals(utils::Point otherPoint) {
            return x==otherPoint.x && y==otherPoint.y;
        }

        // Return the string representation of the point
        std::string toString() {
            std::ostringstream stringStream;
            stringStream << "(" << x << ", " << y << ")";
            return stringStream.str();
        }

        double slope(Point otherPoint) {
            return (double(otherPoint.y - y))/(double(otherPoint.x - x));
        }

        double distanceTo(Point otherPoint) {
            double dY = double(otherPoint.y - y);
            double dX = double(otherPoint.x - x); 
            return sqrt(dY*dY + dX*dX);
        }
    };

    struct AxisAlignedBoundingBox {
        int minX;
        int maxX;
        int minY;
        int maxY;
        AxisAlignedBoundingBox(std::vector<Point> object) {
            minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
            for(int i=0; i<object.size(); i++) {
                minX = std::min(minX, object[i].x);
                minY = std::min(minY, object[i].y);
                maxX = std::max(maxX, object[i].x);
                maxY = std::max(maxY, object[i].y);
            }
        }
        AxisAlignedBoundingBox() : minX(INT_MAX), minY(INT_MAX), 
                                   maxX(INT_MIN), maxY(INT_MIN) {}
    };
       
}

#endif 
