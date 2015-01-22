#ifndef INCLUDE_INFRARED_FIRMWARE_H_
#define INCLUDE_INFRARED_FIRMWARE_H_

#include "mraa.hpp"

namespace firmware{
  class InfraredSensor{
    private:
      mraa::Aio aio;
      //regression for long range is reading = a * (distance) ^ b
      double longRangeBias1; //A number to add to a in a particular sensor
      double longRangeBias2; //A number to add to b (...)
    public:
      //creates a new IR sensor object at the given pin
      InfraredSensor(int aioPin);
      
      //sets biases
      void setBiases(double lrb1, double lrb2);
      
      //Returns the current reading by the sensor (roughly in the range 200-600)
      //takes n samples
      double getReading(int n);
      
      //Returns the short or long distance of maximum probability that the
      // sensor is reading
      double getHighestProbDistance(bool close, double reading);
      
      //Returns a reasonable range of distances
      double getStdDev(bool close, int n, double reading);
  };
}
#endif
