#ifndef INCLUDE_WALLFOLLOWER_H_
#define INCLUDE_WALLFOLLOWER_H_

#include "mraa.hpp"
#include "drive_ctrl.h"
#include "rangefinder_firmware.h"

namespace control{
  // a class for making the robot follow walls
  // only 'start', 'stop', 'getDir', and 'setDir' should be called externally
  class Wallfollower{
    private:
      drive::DriveTrain &driveTrain;
      firmware::Rangefinder &frontRF;
      firmware::Rangefinder &leftRF;
      firmware::Rangefinder &rightRF;
      bool running;
      int dir;
      double dist;
    public:
      //constructor
      Wallfollower(drive::DriveTrain &driveTrain,
        firmware::Rangefinder &frontRF,
        firmware::Rangefinder &leftRF,
        firmware::Rangefinder &rightRF);
      
      //what to do each 'step'
      void step();
      
      //a runner function
      void runner();
      
      //begin running
      void start();
      
      //stop running
      void stop();
      
      //set distance from which to follow wall
      void setDist(int d);
      
      //get distance
      double getDist();
      
      //set direction
      //-1 means that the wall is on the left
      void setDir(int d);
      
      //get direction
      int getDir();
  };
}

#endif
