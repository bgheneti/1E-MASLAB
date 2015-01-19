#include "../include/map.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <climits>

namespace map {

    std::vector<std::string> split(const std::string &s, char delim,     
                      std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while(std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }

    Map::Map(std::string filename) {
        // Initialize temp structures for the map
        std::vector<utils::Point> walls; // even start point and odd end point
        std::vector<utils::Point> homebase; // vertices in the polygon
        stacks.resize(4);

        // Now parse the file and populate structures using map format coords
        std::ifstream mapfile(filename.c_str());
        if(mapfile.is_open()) {
            std::string line;
            while(getline(mapfile, line)) {
                std::vector<std::string> tokens;
                split(line, ',', tokens);
                switch(tokens[0][0]) {
                    case 'P':
                    case 'W':
                        walls.push_back(utils::Point(atoi(tokens[1].c_str()),
                                                    atoi(tokens[2].c_str())));
                        walls.push_back(utils::Point(atoi(tokens[3].c_str()),
                                                    atoi(tokens[4].c_str())));
                        break;
                    case 'S': {
                        int numRed = 0;
                        for(int i=3; i<=5; i++) {
                            if(tokens[i][0]=='R') numRed++;
                        }
                        stacks[numRed].push_back(
                            utils::Point(atoi(tokens[1].c_str()),
                                         atoi(tokens[2].c_str())));
                        break;
                    }
                    case 'L':
                        botLocation.x = atoi(tokens[1].c_str());
                        botLocation.y = atoi(tokens[2].c_str());
                        break;
                    case 'H': {
                        for(int i=2; i<tokens.size(); i+=2) {
                            homebase.push_back(
                                utils::Point(atoi(tokens[i].c_str()),
                                             atoi(tokens[i+1].c_str())));
                        }
                        
                        break;
                    }
                    default:
                        std::cout << "Unknown token" << std::endl;
                        break; 
                }
            }
            mapfile.close();
            
            // Compute the dimensions of the grid
            int minX=INT_MAX, minY=INT_MAX, maxX=INT_MIN, maxY=INT_MIN;
            for(int i=0; i<walls.size(); i++) {
                minX = walls[i].x < minX ? walls[i].x : minX;
                maxX = walls[i].x > maxX ? walls[i].x : maxX;
                minY = walls[i].y < minY ? walls[i].y : minY;
                maxY = walls[i].y > maxY ? walls[i].y : maxY;
            }
            
            utils::Point lowerRight(maxX, maxY);
            lowerRight.convertToInternal(minX, minY);
            for(int y=0; y<lowerRight.y; y++) {
                std::vector<Element> row;
                for(int x=0; x<lowerRight.x; x++) {
                    row.push_back(EMPTY);
                }
                grid.push_back(row);
            }
                
        } else {
            std::cout << "Unable to open file " << filename << std::endl;
        }
        
    }

    void Map::print() {
        for(int y=0; y<grid.size(); y++) {
            for(int x=0; x<grid[y].size(); x++) {
                char charToPrint;
                switch(grid[y][x]) {
                    case EMPTY:
                        charToPrint = '.';
                        break;
                    case HOMEBASE:
                        charToPrint = 'H';
                        break;
                    case OUT_OF_BOUNDS:
                        charToPrint = ' ';
                        break;
                    case WALL:
                        charToPrint = '-';
                        break;
                    case STACK_0R:
                        charToPrint = '0';
                        break;
                    case STACK_1R:
                        charToPrint = '1';
                        break;
                    case STACK_2R:
                        charToPrint = '2';
                        break;
                    case STACK_3R:
                        charToPrint = '3';
                        break;
                    case STACK_US:
                        charToPrint = 'U';
                        break; 
                }

                std::cout << charToPrint << " ";
            }
            std::cout << std::endl;
        }
    }

}

