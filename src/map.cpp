#include "../include/map.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <climits>
#include <algorithm>
#include <cmath>
namespace map {

    Map::Map(std::string filename) {
        std::ifstream mapfile;
        mapfile.open(filename.c_str());
        // Temporary vectors to hold map parsings
        // before we know the scaling of the map
        FormatPoint tempLocation(0, 0 );
        std::vector<FormatPoint> tempWalls; // even are startpoints and odd
                                            // are the corresponding endpoints
        // tempStacks[k] contains locations of stacks with k red blocks
        std::vector<std::vector<FormatPoint> > tempStacks(4, 
                                                   std::vector<FormatPoint>());
        std::vector<FormatPoint> tempHomebase;
        if(mapfile.is_open()) {
           std::string line;
            while(getline(mapfile, line)) {
                // parse the line into tokens
                std::vector<std::string>tokens;
                std::size_t start = 0;
                std::size_t end = 0;
                while(start != std::string::npos && end != std::string::npos) {
                    start = line.find_first_not_of(',', end);
                    if(start != std::string::npos) {
                        end = line.find_first_of(',', start);
                        if(end != std::string::npos) {
                            tokens.push_back(line.substr(start, end-start));
                        } else {
                            tokens.push_back(line.substr(start));
                        }
                    }
                }

                // process the tokens
                if(tokens.size() > 0) {
                    switch(tokens[0][0]) {
                        case 'P':
                        case 'W': {// fallthrough
                            FormatPoint startPoint(atoi(tokens[1].c_str()), 
                                             atoi(tokens[2].c_str()));
                            FormatPoint endPoint(atoi(tokens[3].c_str()), 
                                           atoi(tokens[4].c_str()));
                            tempWalls.push_back(startPoint);
                            tempWalls.push_back(endPoint);
                            break;
                        }
                        case 'S': {
                            FormatPoint stackLoc(atoi(tokens[1].c_str()), 
                                           atoi(tokens[2].c_str()));
                            int numRed = 0; // Num red blocks in stack
                            for(int i=3; i<=5; i++) {
                                if(tokens[i]=="R") {
                                    numRed++;
                                } 
                            }
                            tempStacks[numRed].push_back(stackLoc);
                            break;
                        }
                        case 'H': {
                            for(int i=2; i<tokens.size(); i+= 2) { 
                                FormatPoint vertex(atoi(tokens[i].c_str()),
                                             atoi(tokens[i+1].c_str()));
                                tempHomebase.push_back(vertex);
                            }
                            break;
                        }
                        case 'L': {
                            tempLocation.x = atoi(tokens[1].c_str());
                            tempLocation.y = atoi(tokens[2].c_str());
                            break;
                        }
                    }
                }
            }
            mapfile.close();

        } else {
            std::cout <<"Can't open map file." << std::endl;
        }        
        
        // Compute axis aligned bounding box
        int minX = INT_MAX, maxX = INT_MIN, minY = INT_MAX, maxY = INT_MIN;
        for(int i=0; i<tempWalls.size(); i++) {
            FormatPoint pt = tempWalls[i];
            minX = std::min(minX, pt.x);
            minY = std::min(minY, pt.y);
            maxX = std::max(maxX, pt.x);
            maxY = std::max(maxY, pt.y); 
        }

        // Compute grid dimensions and populate an empty grid
        int deltaX = maxX - minX;
        int deltaY = maxY - minY;
        int xDim = std::ceil(float(deltaX) * 6.096); // 6.096 dm per 2 ft
        int yDim = std::ceil(float(deltaY) * 6.096);
        
        for(int y=0; y<yDim; y++) {
            std::vector<Element> row;
            for(int x=0; x<xDim; x++) {
                row.push_back(EMPTY);
            }
            map.push_back(row);
        }
    
        // Mark the stacks on the grid and put them in the map's stack vector
        for(int numRed=0; numRed<4; numRed++) {
            for(int i=0; i<tempStacks[numRed].size(); i++) {
                Point stackLoc = tempStacks[numRed][i].convert(minX, minY);
                stacks.push_back(Stack(stackLoc.x, stackLoc.y, 
                                       numRed, 3-numRed, false));
                Element elt;
                switch(numRed) {
                    case 0:
                        elt=STACK_0R;
                        break;
                    case 1:
                        elt=STACK_1R;
                        break;
                    case 2:
                        elt=STACK_2R;
                        break;
                    case 3:
                        elt=STACK_3R;
                        break;
                }
                map[round(stackLoc.y)][round(stackLoc.x)] = elt;
            }
        } 

        // Convert our current location to new coordinates and store
        Point loc = tempLocation.convert(minX, minY);
        botLocation(loc.x, loc.y);

        // Convert home base to new coordinates and store


        // Convert walls to new coordinates and mark on map

        
        // Mark areas as inside/outside the playing field region
    }

    void Map::print() {
        int yDim = map.size();
        int xDim = map[0].size();
        for(int y=0; y < yDim; y++) {
            for(int x=0; x < xDim; x++) {
                char charToPrint;
                switch(map[y][x]) {
                    case(EMPTY):
                        charToPrint = '.';
                        break;
                    case(OUTSIDE):
                        charToPrint = ' ';
                        break;
                    case(WALL):
                        charToPrint = '/';
                        break;
                    case(STACK_0R):
                        charToPrint = '0';
                        break;
                    case(STACK_1R):
                        charToPrint = '1';
                        break;
                    case(STACK_2R):
                        charToPrint = '2';
                        break;
                    case(STACK_3R):
                        charToPrint = '3';
                        break;
                    case(STACK_US):
                        charToPrint = 'x';
                        break;
                }
                std::cout << charToPrint << " ";
            }
            std::cout << std::endl;
        }

        std::cout << "CURRENT LOCATION: (" << botLocation.x << ", "
                  << botLocation.y << ")" << std::endl;

        std::cout << "STACKS: " << std::endl;
        for(int i=0; i < stacks.size(); i++) {
            std::cout << "(" << stacks[i].loc.x << ", " << stacks[i].loc.y <<
                         ") " << stacks[i].numRed << "R and " <<
                         stacks[i].numGreen << "G; dropped by us: " <<
                         stacks[i].ours << std::endl; 
        }
    }

    std::vector<Element> Map::getMap() {

    }

    void Map::addStack(double x, double y) {
    
    }

    bool inHomeBase(double x, double y) {

    }

    Point getPointForStackInHomeBase(double clearance) {

    }

    Point getPointForStackNotInHomeBase(double clearance) {

    }

}
