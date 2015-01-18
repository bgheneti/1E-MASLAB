#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

#include <vector>
#include <string>

namespace map {
    // Represents an element on the map.
    enum Element { EMPTY, // No object could be in this spot
                   OUTSIDE, // Outside the walls of region
                   WALL, // A wall or platform sensable by IR
                   STACK_0R, // A stack wtih 3G blocks
                   STACK_1R, // A stack with 2G and 1R blocks 
                   STACK_2R, // A stack with 1G and 2R blocks
                   STACK_3R, // A stack with 3R blocks
                   STACK_US // A stack of any color deposited by bot
    };

    // Represents a point on the bot's internal map representation
    struct Point {
        double x;
        double y;
        Point(double x, double y) : x(x), y(y) {}
    };


    // Represents a point on the staff's map format 
    struct FormatPoint {
        int x;
        int y;
        FormatPoint(int x, int y) : x(x), y(y) {}

        // Converts FormatPoint to internal Point
        Point convert(int minX, int minY) {
            Point pt(double(x-minX)*6.096, double(y-minY)*6.096);
            return pt;
        }
    };

    struct Stack {
        Point loc;
        int numRed;
        int numGreen;
        bool ours; // If true, bot dropped this stack
        Stack(double x, double y, int numRed, int numGreen, bool ours) : 
                                                            loc(x, y), 
                                                            numRed(numRed),
                                                            numGreen(numGreen),
                                                            ours(ours) {}
    };

    struct Wall {
        Point startPoint;
        Point endPoint;
        Wall(double startX, double startY, 
             double endX, double endY) : startPoint(startX, startY),
                                         endPoint(endX, endY) {}
    };
        
    // The Map class contains a vector array representing the map
    // as well as functions to determine which zone of the map we are
    // in.
    class Map {
        private:
            std::vector<std::vector<Element> > map;
            std::vector<Point> homebase; // A vector containing the vertices
                                         // of the home base polygon
            std::vector<Wall> walls;
            std::vector<Stack> stacks;
            Point botLocation;
        public:
            // Create a map from a map file
            Map(std::string filename);

            // Print the map grid (for debugging purposes)
            void print();

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
