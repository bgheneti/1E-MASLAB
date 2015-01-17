#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

#include <vector>

namespace map {
    // Represents an element on the map.
    enum Element { EMPTY, // No object could be in this spot
                   OUTSIDE, // Outside the walls of region
                   WALL, // A wall or platform sensable by IR
                   STACK_R, // A stack wtih 2R and 1G blocks
                   STACK_G, // A stack with 2G and 1R blocks 
                   STACK_US // A stack of any color deposited by bot
    };
    struct Point {
        int x;
        int y;
        Point(int x, int y) : x(x), y(y) {}
    };
    // The Map class contains a vector array representing the map
    // as well as functions to determine which zone of the map we are
    // in.
    class Map {
        private:
            std::vector<Element> map;
            std::vector<Point> homebase; // A vector containing the vertices
                                         // of the home base polygon
            std::vector<Point> walls;
            std::vector<Point> stacksR;
            std::vector<Point> stacksG;
            std::vector<Point> stacksUs;
        public:
            // Create a map from a map file
            Map(std::string filename);

            // Get a copy of the current map
            std::vector<Element> getMap();

            // Add a STACK_US to the map at position (x, y)
            void addStack(double x, double y);

            // Determines whether a point is in home base
            bool inHomeBase(double x, double y);

            // Gets a point inside home base that is far from any other dropped
            // stacks. Returns (-7, -7) if this is not possible within the
            // specified clearance. Tries to do this in a way that provides
            // efficient packing of stacks.
            Point getPointForStackInHomeBase(double clearance);

            // Gets a point not in home base for dropping a stack where it is
            // unlikely to be run over by the bot. Returns (-7, -7) if not
            // possible in given clearance.
            Point getPointForStackNotInHomeBase(double clearance);
    };       
}


#endif
