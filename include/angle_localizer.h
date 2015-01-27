#ifndef INCLUDE_ANGLE_LOCALIZER_H_
#define INCLUDE_ANGLE_LOCALIZER_H_

#include "rangefinder_firmware.h"
#include "drive_ctrl.h"
#include "mraa.hpp"
#include <vector>
#include <math.h>
#include <iostream>

namespace map{
  //a class to get the highest probability angle given a x and y position
  
  class AngleLocalizer{
    private:
      firmware::Rangefinder &rf;
      drive::DriveTrain &dt;
      int xLoc;
      int yLoc;
      std::vector<double> expected;
      std::vector<std::vector<double>> map;
    public:
      AngleLocalizer(firmware::Rangefinder &rf,
        drive::DriveTrain &dt,
        Map &probMap);
      //must be called so that the localizer can calculate expected distances
      void setLocation(int xl, int yl);
      
      //returns the shift of indices that most likely yields static from dynamic
      int getMax(std::vector<double> sta, std::vector<double> dyn);
      
      //get most probable heading
      //degrees
      int getAngle(int numReadings);
      
  };
}

#endif
