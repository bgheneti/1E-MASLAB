#include "../include/map.h"
#include <algorithm>

namespace map{
  //generates a 
  ObservationModel::ObservationModel(Map m){
    
  }
  
  //returns the probability of a sensor hit in this area
  double getProb(double x, double y){
    return probMap.at(int(10*x) + int(y*10)*mapWidth);
  }
  double setLoc(double x, double y){
    startX = int(x*10)/10.0;
    startY = int(y*10)/10.0;
  }
  //returns the probability of getting l1, l2, l3 given p
  double ObservationModel::getPObsGivenPos(Particle p, double l1, double l2, double l3){
    double pL1 = getPDistance();
    double pL2 = 1;
    double pL3 = 1;
    
    return pL1 * pL2 * pL3;
  }
  
  //probability of getting distance given the other stuff.  Slope and intercept describe the direction of the sensors
  double ObservationModel::getPDistance(double startX, double startY, double slope, double intercept, double distance){
    double p = 1;
    double stepX = std::min(1, std::abs(1/slope));
    double stepY = slope * stepX;
    double currX = startX;
    double currY = startY;
    
    while (std::abs(int(currX*10) - int(startX*10))<distance*10){
      currX += stepX;
      currY += stepY;
      
      p *= (1-getProb(currX, currY));
      
      
    }
    p *= getProb(currX, currY);
    return p;
  }
}
