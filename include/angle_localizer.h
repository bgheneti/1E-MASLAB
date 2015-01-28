#ifndef INCLUDE_ANGLE_LOCALIZER_H_
#define INCLUDE_ANGLE_LOCALIZER_H_

#include "rangefinder_firmware.h"
#include "drive_ctrl.h"
#include "mraa.hpp"
#include "map.h"
#include <vector>
#include <math.h>
#include <iostream>

namespace map{
  //a class to get the highest probability angle given a x and y position
  
  class AngleLocalizer{
    private:
      firmware::Rangefinder &front;
      firmware::Rangefinder &right;
      drive::DriveTrain &dt;
      int xLoc;
      int yLoc;
      std::vector<double> expected;
      std::vector<std::vector<double>> probMap;
    public:
      AngleLocalizer(firmware::Rangefinder &front,
        firmware::Rangefinder &right,
        drive::DriveTrain &dt,
        Map m);
      //must be called so that the localizer can calculate expected distances
      void setLocation(int xl, int yl);
      
      //returns the shift of indices that most likely yields static from dynamic
      int getMax(std::vector<double> sta, std::vector<double> dyn, int lowerBound, int upperBound);
      
      //get most probable heading
      //degrees
      int getAngle(int numReadings);
      int getAngle(int numReadings, int lowerBound, int upperBound);
  };
}

#endif
