#include <cmath>
#include <cassert>
#include <math.h>
#include "../include/infrared_firmware.h"

namespace firmware{
  //This class communicates with the infrared sensors (range finders).
  InfraredSensor::InfraredSensor(int aioPin):aio(aioPin){
    longRangeBias1 = 0;
    longRangeBias2 = 0;
  }
  
  //reads and returns the value of the aio
  double InfraredSensor::getReading(int n){
    double reading = 0;
    for (int i = 0; i < n; i++){
      reading += aio.read();
    }
    return reading/n;
  }
  
  void InfraredSensor::setBiases(double lrb1, double lrb2){
    longRangeBias1 = lrb1;
    longRangeBias2 = lrb2;
  }
  
  
  //returns the estimated distance in two ways: a highest-probability estimate
  //and a range of reasonable values
  //returns {highest probability value, low reasonable, high reasonable}
  double InfraredSensor::getHighestProbDistance(bool close, double reading){
    double highestProbDistance=0;
    if (close){
      highestProbDistance = (reading-206.55)/32.81;
    }
    else{
      highestProbDistance = pow(double(reading)/(9609.5+longRangeBias1), -1/(0.965+longRangeBias2));
      if (highestProbDistance < 19){
        highestProbDistance = (713.64-reading)/10.866;
      }
    }
    return highestProbDistance;
  }
  
  //returns the lower reasonable bound of possible distances
  double InfraredSensor::getLowerBound(bool close, double reading){
    double allowance = 5;
    if (close){
      return InfraredSensor::getHighestProbDistance(close, reading - allowance);
    }
    else{
      return InfraredSensor::getHighestProbDistance(close, reading + allowance);
    }
  }

  //returns the upper reasonable bound of possible distnaces
  double InfraredSensor::getHigherBound(bool close, double reading){
    double allowance = 5;
    if (close){
      return InfraredSensor::getHighestProbDistance(close, reading + allowance);
    }
    else{
      return InfraredSensor::getHighestProbDistance(close, reading - allowance);
    }
  }
}
