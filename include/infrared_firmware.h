#ifndef INCLUDE_INFRARED_FIRMWARE_H_
#define INCLUDE_INFRARED_FIRMWARE_H_

#include "mraa.hpp"

namespace firmware{
  class InfraredSensor{
    private:
      mraa::Aio aio;
    public:
      //creates a new IR sensor object at the given pin
      InfraredSensor(int aioPin);
      
      //Returns the current reading by the sensor (roughly in the range 200-600)
      int getReading();
  };
}
#endif
