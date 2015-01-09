#include <cmath>
#include <cassert>

#include "../include/infrared_firmware.h"

namespace firmware{
  //This class communicates with the infrared sensors (range finders).
  InfraredSensor::InfraredSensor(int aioPin):aio(aioPin){}
  
  //reads and returns the value of the aio
  int InfraredSensor::getReading(){
    return aio.read();
  }
}
