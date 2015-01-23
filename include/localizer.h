#ifndef INCLUDE_LOCALIZER_H_
#define INCLUDE_LOCALIZER_H

#include "mraa.hpp"
#include "observation_model.h"
#include "rangefinder_firmware.h"
#include "map.h"

namespace map{
  class Localizer{
    private:
      std::vector<Particle> particles;
      int sampleSize;
      int xMax, yMax, xMin, yMin;
      ObservationModel obsModel;
      firmware::Rangefinder r1, r2, r3;
    public:
      //constructor, takes three 
      Localizer(Map m, int irR1, int usTR1, int usER1, int irR2, int usTR2, int usER2, int irR3, int usTR3, int usER3);
      
      //uses bayesian reasoning to update particle weights given distances from sensors
      void updateWeights(double l1, double l2, double l3);
      
      //given external information from odometry, update
      void updatePos(double dx, double dy, double dth);
      
      //generate new particles based on updated weights
      void step();
  };
}

#endif
