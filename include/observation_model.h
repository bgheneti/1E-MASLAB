#ifndef INCLUDE_OBSERVATION_MODEL_H_
#define INCLUDE_OBSERVATION_MODEL_H_

#include <vector>
#include "map.h"
#include "mraa.hpp"

namespace map{
  
  struct Particle{
    double x, y, th, weight;
  };
  
  class ObservationModel{ //a class to handle finding the probabilities of observations
    private:
      std::vector<std::vector<double>> probMap; //contains the probabiblities of a hit in each area
      double startX, startY;
    public:
      //observation model will be constructed from a map
      ObservationModel(Map m);
      
      double getProb(double x, double y);
      double setLoc(double x, double y);
      //get probability of a particle with observations l1, l2, and l3 from rangefinders
      double getPObsGivenPos(Particle p, double l1, double l2, double l3);
      //probability of getting distance given other information about sensor
      double getPDistance(double slope, double intercept, double distance);
  };
}

#endif
