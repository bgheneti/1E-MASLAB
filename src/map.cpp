#include "../include/map.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <climits>
#include <math.h>

#define PI 3.14159265

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

    Map::Map(std::string filename) : botHeading(0.0) {
        // Initialize temp structures for the map
        stacks.resize(5);

        // Now parse the file and populate structures using map format coords
        parseMapFile(filename);           

        // Initialize the map grid
        initializeGrid();

        // Fill in the homebase on the map
        fillHomeBase();

        // Put walls, stacks, and empty space on the map
        fillObjects();

        // Mark passable points (the robot could fit at them)
        markPassable();
    }

    void Map::markPassable() {
        for(int i=0; i < passableElements.size(); i++) {
            if(grid[passableElements[i].y][passableElements[i].x] == PASSABLE) {
                grid[passableElements[i].y][passableElements[i].x] = EMPTY; 
            }
        }
        updatePassable();
        for(int i=0; i < passableElements.size(); i++) {
            grid[passableElements[i].y][passableElements[i].x] = PASSABLE;
        }
    }

    void Map::updatePassable() {
        passableElements.clear();
        for(int y=0; y<grid.size(); y++) {
            for(int x=0; x<grid[0].size(); x++) {
                if(!isGround(grid[y][x])) continue;
                bool passable = true;
                for(int deltaY=-2; deltaY<=2; deltaY++) {
                    for(int deltaX=-2; deltaX<=2; deltaX++) {
                        if(y+deltaY<0 || y+deltaY>=grid.size() ||
                                x+deltaX<0 || x+deltaX>=grid[0].size() ||
                                !(isStackToPickUp(grid[y+deltaY][x+deltaX]) || 
                                  isGround(grid[y+deltaY][x+deltaX]))) {
                            passable=false;
                            break;
                        }
                    }
                }
                if(passable) passableElements.push_back(utils::Point(x, y));
            }
        }

    }

    void Map::parseMapFile(std::string filename) {
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

        } else {
            std::cout << "Unable to open file " << filename << std::endl;
        }


    }

    void Map::initializeGrid() {
        // Compute the dimensions of the grid and initialize it to those
        // dimensions
        fieldBB = utils::AxisAlignedBoundingBox(walls);

        utils::Point lowerRight(fieldBB.maxX, fieldBB.maxY);
        lowerRight.convertToInternal(fieldBB.minX, fieldBB.minY);
        for(int y=0; y<=lowerRight.y; y++) {
            std::vector<Element> row;
            for(int x=0; x<=lowerRight.x; x++) {
                row.push_back(OUT_OF_BOUNDS);
            }
            grid.push_back(row);
        }

        // Convert all points 
        for(int i=0; i<walls.size(); i+=1) {
            walls[i].convertToInternal(fieldBB.minX, fieldBB.minY);
        }
        for(int i=0; i<stacks.size(); i++) {
            for(int j=0; j<stacks[i].size(); j++) {
                stacks[i][j].convertToInternal(fieldBB.minX, fieldBB.minY);
            }
        }
        for(int i=0; i<homebase.size(); i++) {
            homebase[i].convertToInternal(fieldBB.minX, fieldBB.minY);
        }
        botLocation.convertToInternal(fieldBB.minX, fieldBB.minY);
    }
    
    void Map::fillHomeBase() {
        // Add no-man-zone to the map
        int startIndex = homebase.size() - 1;
        for(int endIndex=0; endIndex<homebase.size(); endIndex++) {
            utils::Point startPoint = homebase[startIndex];
            utils::Point endPoint = homebase[endIndex];
            double slope = startPoint.slope(endPoint);
            utils::Point minXPoint = startPoint.x < endPoint.x ? 
                startPoint : endPoint;
            utils::Point maxXPoint = startPoint.x < endPoint.x ? 
                endPoint : startPoint;
            for(double xCoord=minXPoint.x, yCoord=minXPoint.y;
                    xCoord <= maxXPoint.x; xCoord+=.5, yCoord+=slope*.5) {
                grid[round(yCoord)][round(xCoord)] = NO_MAN_ZONE;
            }
            if(startPoint.x == endPoint.x) {
                int minY = std::min(startPoint.y, endPoint.y);
                int maxY = std::max(startPoint.y, endPoint.y);
                for(int xCoord = startPoint.x, yCoord = minY;
                        yCoord < maxY; yCoord++) {
                    grid[yCoord][xCoord] = NO_MAN_ZONE;
                }
            }

            startIndex = endIndex;
        }

        // Fill in home base
        utils::AxisAlignedBoundingBox homebaseBB(homebase);
        for(int y=homebaseBB.minY; y < homebaseBB.maxY; y++) {
            for(int x=homebaseBB.minX; x < homebaseBB.maxX; x++) {
                if(grid[y][x]==NO_MAN_ZONE) continue;

                // Iterate through each side and count intersections
                int count = 0;
                int endIndex = homebase.size()-1;
                for(int startIndex=0; startIndex<homebase.size();
                        startIndex++) {
                    utils::Point startPoint = homebase[startIndex];
                    utils::Point endPoint = homebase[endIndex];
                    double slope = startPoint.slope(endPoint);
                    if(std::min(startPoint.y, endPoint.y) <= y &&
                            std::max(startPoint.y, endPoint.y) >= y &&
                            double(y-startPoint.y)/slope + startPoint.x >= x) {
                        count++;
                    }
                    if(count%2==1) grid[y][x] = HOMEBASE;
                    endIndex = startIndex;
                }
            }
        }
    }

    void Map::fillObjects() {
        // Put walls on map
        for(int i=0; i<walls.size(); i+=2) {
            utils::Point point1 = walls[i];
            utils::Point point2 = walls[i+1];
            if(point1.x==point2.x) {     
                for(int y=std::min(point1.y, point2.y);
                        y<=std::max(point1.y, point2.y); y++) {
                    grid[y][point1.x] = WALL;
                }
            } else {
                for(int x=std::min(point1.x, point2.x); 
                        x<=std::max(point1.x, point2.x); x++) {
                    double slope = point1.slope(point2);
                    int y = slope * double(x-point1.x) + point1.y;
                    grid[y][x] = WALL;
                }

            }
        }

        // Fill in empty spaces using BFS from botLocation

        // If botLocation is in home base, explore outward until point
        // not in home base is found without crossing a wall
        std::vector<utils::Point> queue;
        queue.push_back(botLocation);
        while(queue.size() > 0) {
            utils::Point pt = queue.back();
            queue.pop_back();
            if(grid[pt.y][pt.x]==OUT_OF_BOUNDS) {
                queue.clear();
                queue.push_back(pt);
                break;
            } else if(grid[pt.y][pt.x]==HOMEBASE ||
                    grid[pt.y][pt.x]==NO_MAN_ZONE) {
                queue.insert(queue.begin(), utils::Point(pt.x, pt.y+1));
                queue.insert(queue.begin(), utils::Point(pt.x, pt.y-1));
                queue.insert(queue.begin(), utils::Point(pt.x+1, pt.y));
                queue.insert(queue.begin(), utils::Point(pt.x-1, pt.y));
            }
        } 

        // Now search outward like normal
        while(queue.size() > 0) {
            utils::Point pt = queue.back();
            queue.pop_back();
            if(grid[pt.y][pt.x]==OUT_OF_BOUNDS) {
                grid[pt.y][pt.x] = EMPTY;
                queue.insert(queue.begin(), utils::Point(pt.x, pt.y+1));
                queue.insert(queue.begin(), utils::Point(pt.x, pt.y-1));
                queue.insert(queue.begin(), utils::Point(pt.x+1, pt.y));
                queue.insert(queue.begin(), utils::Point(pt.x-1, pt.y));
            }
        }

        // Populate stacks
        for(int numRed=0; numRed < 4; numRed++) {
            Element elements[] = {STACK_0R, STACK_1R, 
                STACK_2R, STACK_3R};
            for(int i=0; i<stacks[numRed].size(); i++) {
                grid[stacks[numRed][i].y][stacks[numRed][i].x] = 
                    elements[numRed];
            }
        }

    }

    bool Map::isGround(Element elt) {
        return elt==EMPTY || elt==HOMEBASE || elt==NO_MAN_ZONE || elt==PASSABLE;
    }

    bool Map::isStackToPickUp(Element elt) {
        return elt==STACK_0R || elt==STACK_1R || elt==STACK_2R || elt==STACK_3R;
    }

    std::vector<std::vector<Element> > Map::getGrid() {
        return grid;
    }

    void Map::putDownStack(double x, double y) {
        utils::Point stackPoint(x, y);
        stacks[4].push_back(stackPoint);
        grid[stackPoint.y][stackPoint.x] = STACK_US;
        markPassable();
    }

    void Map::putDownStack(int x, int y) {
        utils::Point stackPoint(x, y);
        stacks[4].push_back(stackPoint);
        grid[y][x] = STACK_US;
        markPassable();

    }

    utils::Point Map::getLocation() {
        return botLocation;
    }

    void Map::setLocation(double x, double y) {
        botLocation = utils::Point(x, y);
    }

    void Map::setLocation(int x, int y) {
        botLocation = utils::Point(x, y);

    }

    void Map::setLocationRelative(double deltaX, double deltaY) {
        botLocation.x += round(deltaX);
        botLocation.y += round(deltaY);
    }

    void Map::setLocationRelative(int deltaX, int deltaY) {
        botLocation.x += deltaX;
        botLocation.y += deltaY;
    }
    
    void Map::setLocationStraight(double actualDistance){
      botLocation.x += sin(botHeading*PI/180.0)*actualDistance;
      botLocation.y += cos(botHeading*PI/180.0)*actualDistance;
    }
    
    void Map::setHeading(double h){botHeading=h;}
    void Map::setHeadingRelative(double dh){botHeading+=dh;}
    double Map::getHeading(){return botHeading;}
    
    utils::Point Map::getNearestStack() {
        double minDistance = 500;
        utils::Point minStack;
        for(int i=0; i<stacks.size(); i++) {
            for(int j=0; j<stacks[i].size(); j++) {
                double distance =0;
                std::vector<DrivingInstruction> instructions = getDrivingInstructions(getPathTo(stacks[i][j]));
                for(int k=0; k<instructions.size(); k++) {
                    distance += instructions[k].distance;
                }
                if(distance < minDistance) {
                    minStack = stacks[i][j];
                    minDistance = distance;
                }
            }
        }
        return minStack;
    }

    utils::Point Map::whereToDropStack(Zone zone) {


    }

    std::vector<utils::Point> Map::getPathTo(utils::Point goal) {
        double*       dist = new double      [grid.size()*grid[0].size()];
        utils::Point* prev = new utils::Point[grid.size()*grid[0].size()];
        std::vector<utils::Point> queue;
        const int MAXY = grid.size();
        
        dist[botLocation.x*MAXY + botLocation.y] = 0;
        for(int i=0; i<passableElements.size(); i++) {
            if(!botLocation.equals(passableElements[i])) {
                dist[passableElements[i].x*MAXY + passableElements[i].y] = 500; // dist will never be 500m from source
            }
            queue.push_back(passableElements[i]);
        }

        while(queue.size() > 0) {
            std::vector<utils::Point>::iterator minIt;
            double minDistance = 501;
            for(std::vector<utils::Point>::iterator it = queue.begin(); it != queue.end(); it++) {
                double newDistance = dist[(*it).x*MAXY + (*it).y];
                if(newDistance < minDistance) {
                    minDistance = newDistance;
                    minIt = it;
                }
            }
            utils::Point node((*minIt).x, (*minIt).y);
            if(node.equals(goal)) {break;}
            queue.erase(minIt);

            for(int dY=-1; dY<=1; dY++) {
                for(int dX=-1; dX<=1; dX++) {
                    if(dX==0 && dY==0) {continue;}
                    utils::Point otherPoint(node.x + dX, node.y + dY);
                    double alt = minDistance + sqrt(dX*dX + dY*dY);
                    double otherDist = dist[otherPoint.x*MAXY + otherPoint.y];
                    if(alt < otherDist) {
                        dist[otherPoint.x*MAXY + otherPoint.y] = alt;
                        prev[otherPoint.x*MAXY + otherPoint.y] = node;
                    }
                }
            }
        }

        std::vector<utils::Point> path;
        utils::Point node = goal;
        while(!node.equals(utils::Point())) {
            path.push_back(node);
            node = prev[node.x*MAXY + node.y];
        }

        // Compress the path
        std::vector<utils::Point> compressed_path;
        compressed_path.push_back(botLocation);
        for(int i=path.size()-3; i>=0; i--) {
            int dX1 = path[i+1].x - path[i].x;
            int dX2 = path[i+2].x - path[i+1].x;
            int dY1 = path[i+1].y - path[i].y;
            int dY2 = path[i+2].y - path[i+1].y;
            if(dX1!=dX2 || dY1!=dY2) {
                compressed_path.push_back(path[i+1]);
            }
        }
        compressed_path.push_back(goal);


        delete[] dist;
        delete[] prev;
        return compressed_path;    
    }

    std::vector<DrivingInstruction> Map::getDrivingInstructions(std::vector<utils::Point> path) {
        std::vector<DrivingInstruction> drivingInstructions;
        double heading = botHeading;
        for(int i=1; i<path.size(); i++) {
            double distance = path[i-1].distanceTo(path[i]) * .1;
            double dX = double(path[i].x - path[i-1].x);
            double dY = double(path[i].y - path[i-1].y);
            double newHeading = -atan2(dY, dX) * 180.0/3.14159 + 90 - heading;
            drivingInstructions.push_back(DrivingInstruction(newHeading, distance));
            heading = newHeading;
        }

        return drivingInstructions;

    }

    void Map::print() {
        for(int y=grid.size()-1; y>=0; y--) {
            for(int x=0; x<grid[y].size(); x++) {
		if(botLocation.x==x && botLocation.y==y) {
			std::cout << 'L' << ' ';
			continue;
		}
                char charToPrint;
                switch(grid[y][x]) {
                    case EMPTY:
                        charToPrint = '.';
                        break;
                    case PASSABLE:
                        charToPrint = 'o';
                        break;
                    case HOMEBASE:
                        charToPrint = 'h';
                        break;
                    case OUT_OF_BOUNDS:
                        charToPrint = ' ';
                        break;
                    case WALL:
                        charToPrint = '=';
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
                    case NO_MAN_ZONE:
                        charToPrint = 'H';
                        break;
                }

                std::cout << charToPrint << " ";
            }
            std::cout << std::endl;
        }
    }

}


