#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

#include "point.h"
#include <vector>
#include <iostream>
#include <string>

namespace map {
    enum Element {
             EMPTY, // In playing field but nothing special
             PASSABLE,
             HOMEBASE, // In home base polygon
             OUT_OF_BOUNDS, // Not in playing field
             WALL, // The boundary of the playing field
             NO_MAN_ZONE, // The boundary between the field and home base
             STACK_0R, // Stack of 3G and 0R
             STACK_1R, // Stack of 2G and 1R
             STACK_2R, // Stack of 1G and 2R
             STACK_3R, // Stack of 0G and 3R
             STACK_US // Stack our bot dropped
        };

    enum Zone {FIELD, HOME};
    
    struct DrivingInstruction {
        double heading;
        double distance;
        DrivingInstruction(double heading, double distance) : heading(heading), distance(distance) {}
        DrivingInstruction(): heading(0),distance(-1) {}

    };

    class Map {
        private:
            std::vector<std::vector<Element> > grid;
            std::vector<std::vector<utils::Point> > stacks;
            utils::Point botLocation;
	    double botHeading;
            std::vector<utils::Point> walls; // even start point and odd end point
            std::vector<utils::Point> homebase; // vertices in the polygon
            std::vector<utils::Point> passableElements;
            utils::AxisAlignedBoundingBox fieldBB;

            void parseMapFile(std::string filename);
            void initializeGrid();
            void fillHomeBase();
            void fillObjects();
            void updatePassable();
            void markPassable();
            bool isGround(Element elt);
            bool isStackToPickUp(Element elt);

        public:
            Map(std::string filename);
            std::vector<std::vector<Element> > getGrid();
            void putDownStack(double x, double y);
            void putDownStack(int x, int y);
            utils::Point getLocation();
            void setLocation(double x, double y);
            void setLocation(int x, int y);
            void setLocationRelative(double deltaX, double deltaY);
            void setLocationRelative(int deltaX, int deltaY);
            void setLocationStraight(double actualDistance);
            void setHeading(double h);
            void setHeadingRelative(double dh);
            double getHeading();
            utils::Point whereToDropStack(Zone zone);
            utils::Point getNearestStack();
            std::vector<utils::Point> getPathTo(utils::Point goal);
            std::vector<DrivingInstruction> getDrivingInstructions(std::vector<utils::Point>);
            void print();

    };

}

#endif
