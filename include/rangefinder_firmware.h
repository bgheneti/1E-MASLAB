#ifndef INCLUDE_RANGEFINDER_FIRMWARE_H_
#define INCLUDE_RANGEFINDER_FIRMWARE_H_

#include "mraa.hpp"

namespace firmware{
  //a class to make more intelligent distance estimates based on an infrared and 
  //ultrasonic sensor pair pointed in the same direction
  class Rangefinder{
    private:
      InfraredSensor ir; //the infrared sensor
      UltrasonicSensor us; //the ultrasonic sensor
      double estimatedDistance;
      double longTermDistance;
      double stdDev;
    public:
      //Constructor; requires the three io pins
      Rangefinder(int irPin, int usTriggerPin, int usEchoPin);
      
      //returns an estimate of distance 
      double getHighestProbDistance();
      
      //returns a reasonable standard deviation
      double getStdDev(double hpDist);
      
      //returns the probability that a block has been passed recently
      double getProbOfBlock();
}
#endif
