#ifndef INCLUDE_ULTRASONIC_FIRMWARE_H_
#define INCLUDE_ULTRASONIC_FIRMWARE_H_

#include "mraa.hpp"

namespace firmware{
  class UltrasonicSensor{
    private:
      mraa::Gpio trigger; //the gpio through which to send signals
      mraa::Gpio echo; //the gpio from which to read responses
    public:
      //initialize ultrasonic sensor object
      UltrasonicSensor(int triggerPin, int echoPin);
      
      //returns the distance in cm corresponding to the length of the echo
      double getDistance();
      
      //returns the highest probability distance based on multiple distance readings
      double getHighestProbDistance(int n);
      
      //return reasonable ranges of distances based on highest prob distance
      double getStdDev(int n, double hpDist);
  };
}
#endif
