#include "../include/map.h"
#include <iostream>
#include <algorithm>

namespace map{
  //generates a 
  ObservationModel::ObservationModel(Map m){
    std::vector<std::vector<Element>> g = m.getGrid();
    Element e;
    for (int r = 0; r < g.size(); r++){
      
      for (int c = 0; c < g[0].size(); c++){
        e=[r][c];
        if (e == EMPTY || e == HOMEBASE || e == NO_MAN_ZONE){probMap[r][c] = 0.01;}
        else if (e==OUT_OF_BOUNDS){probMap[r][c] = 0.001;}
        else if (e==WALL){probMap[r][c] = 0.99;}
        else if (e==STACK_0R || e==STACK_1R || e==STACK_2R || e == STACK_3R || e == STACK_US){probMap[r][c] = 0.3;}
        else{std::cout << e << std::endl;}
      }
    }
    startX = c/2;
    startY = r/2;
  }
  
  //returns the probability of a sensor hit in this area
  //x and y in tens of centimeters
  double getProb(double x, double y){
    return probMap[int(y)][int(x)];
  }
  double setLoc(double x, double y){
    startX = int(x*10)/10.0;
    startY = int(y*10)/10.0;
  }
  //returns the probability of getting l1, l2, l3 given p
  //l1, l2, l3 are in centimeters
  double ObservationModel::getPObsGivenPos(Particle p, double l1, double l2, double l3){
    double pL1 = getPDistance();
    double pL2 = getPDistance();
    double pL3 = getPDistance();
    
    return pL1 * pL2 * pL3;
  }
  
  //probability of getting distance given the other stuff.  Slope and intercept describe the direction of the sensors
  //distance in centimeters
  double ObservationModel::getPDistance(double slope, double intercept, double distance){
    double p = 1;
    double stepX = std::min(1, std::abs(1/slope));
    double stepY = slope * stepX;
    double currX = startX;
    double currY = startY;
    
    while (std::abs(int(currX*10) - int(startX*10))<distance){
      currX += stepX;
      currY += stepY;
      
      p *= (1-getProb(currX, currY));
      
      
    }
    p *= getProb(currX, currY);
    return p;
  }
}
