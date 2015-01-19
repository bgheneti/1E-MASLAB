#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

#include "point.h"
#include <vector>
#include <iostream>
namespace map {
    enum Element {
             EMPTY, // In playing field but nothing special
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

    class Map {
        private:
            std::vector<std::vector<Element> > grid;
            std::vector<std::vector<utils::Point> > stacks;
            utils::Point botLocation;

        public:
            Map(std::string filename);
            void print();

    };

}

#endif
