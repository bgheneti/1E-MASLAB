#include "../include/map.h"

#include <fstream>
#include <iostream>
#include <stdlibh>
#include <climits>
#inclue <algorithm>
namespace map {

    Map::Map(std::string filename) {
        ifstream mapfile;
        mapfile.open(filename);
        // Temporary vectors to hold map parsings
        // before we know the scaling of the map
        std::vector<Point> tempWalls; // even are startpoints and odd
                                      // are the corresponding endpoints
        std::vector<Point> tempStacksR;
        std::vector<Point> tempStacksG;
        std::vector<Point> tempLocation;
        std::vector<Point> tempHomebase;
        if (mapfile.is_open()) {
           std::string line;
            while(getline(mapfile, line)) {
                // parse the line into tokens
                std::vector<std::string>tokens;
                std::size_t start = 0;
                std::size_t end = 0;
                while(start != std::string::npos && end != std::string::npos) {
                    start = line.find_first_not_of(',', end);
                    if(start != string::npos) {
                        end = line.find_first_of(',', start);
                        if(end != string::npos) {
                            tokens.push_back(line.substr(start, end-start));
                        } else {
                            tokens.push_back(input.substr(start));
                        }
                    }
                }

                // process the tokens
                if(tokens.size() > 0) {
                    switch(tokens[0]) {
                        case "W":
                        case "P": // fallthrough
                            Point startPoint(atoi(tokens[1], atoi(tokens[2])));
                            Point endPoint(atoi(tokens[3], atoi(tokens[4])));
                            tempWalls.push_back(startPoint);
                            tempWalls.push_back(endPoint);
                            break;

                        case "S":
                            Point loc(atoi(tokens[1]), atoi(tokens[2]));
                            int numRed = 0; // Num red blocks in stack
                            for(int i=3; i<=5; i++) {
                                if(tokens[i]=="R") {
                                    numRed++;
                                } 
                            }
                            if(numRed == 2) {
                                tempStacksR.push_back(loc);
                            } else {
                                tempStacksG.push_back(loc);
                            }
                            break;
                        case "H":
                            for(int i=2; i<tokens.size(); i+= 2) { 
                                Point vertex(atoi(tokens[i]),
                                             atoi(tokens[i+1]));
                                tempHomebase.push_back(vertex);
                            }
                            break;
                        case "L":
                            Point loc(atoi(tokens[1]), atoi(tokens[2]));
                            tempLocation = loc;
                            break;
            }
            mapfile.close();
        } else {
            std::cout <<"Can't open map file." << std::endl;
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
