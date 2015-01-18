#include <iostream>
#include "../include/rangefinder_firmware.h"
#include <algorithm>

namespace firmware{
  Rangefinder::Rangefinder(int irPin, int usTriggerPin, int usEchoPin) : ir(irPin), us(usTriggerPin, usEchoPin){
    stdDev = -1;
    estimatedDistance=-1; //the short-term distance (resistant to change)
    longTermDistance=-1; //the long-term distance (extremely resistant to change)
  }
  
  //returns an estimate of distance
  double Rangefinder::getHighestProbDistance(){
    double usN = 5, irN = 30; //number of samples for each sensor
    
    //whether the sensors should look for things in the range of 0-20 cm or 20-? cm
    bool close = (estimatedDistance != -1) && (estimatedDistance < 20);
    
    //get us info
    double usDist = us.getHighestProbDistance(usN);
    double usS = us.getStdDev(usN, usDist);
    //std::cout << usDist << "\t" << usS << std::endl;
    //get ir info
    double irReading = ir.getReading(irN);
    double irDist = ir.getHighestProbDistance(close, irReading);
    double irS = ir.getStdDev(close, irN, irReading);
    //std::cout << irDist << "\t" << irS << std::endl;
    double currDistance = -1; //the distance from this set of readings only
    
    //find an estimate of currDistance
    if (usDist == -1){//if no us reading was obtained
      currDistance = irDist; //use only the ir one
      stdDev = irS;
    }
    else{ //if there was a us reading
      if (abs(usDist - irDist) < irS){ //if us seems reasonable
        currDistance = (usDist+irDist)/2; //average ir and uv
        stdDev = (usS + irS)/2;
      }
      else { //if us is not reasonable
        //try the other side of the curve for the ir sensor
        irDist = ir.getHighestProbDistance(!close, irReading);
        irS = ir.getStdDev(!close, irN, irReading);
        
        //if us seems reasonable now
        if (abs(usDist-irDist)<irS){
          currDistance = (usDist + irDist)/2; //average the two
          stdDev = (usS + irS)/2;
        }
        else{ //if us is still unreasonable
          currDistance = -1; //this entire reading should be taken as invalid
        }
      }
    }
    
    //updating longer-term estimates based on current information
    if (estimatedDistance==-1){ //if no past information
      if (currDistance == -1){ //if no current information
        //nothing to be done about that
      }
      else{ //if there is current information
        estimatedDistance = currDistance; //store the current information
        longTermDistance = currDistance;
      }
    }
    else{ //if there is past information
      if (currDistance == -1){ //if there is no current information
        //leave past estimates as they are
      }
      else{ //if there is current information
        estimatedDistance = estimatedDistance*0.9+currDistance*0.1; //factor it in
        longTermDistance = longTermDistance*0.99+currDistance*0.01;
      }
    }
    
    return currDistance;
  }
  
  //returns a reasonable lower bound
  double Rangefinder::getStdDev(double hpDist){
    return stdDev;
  }
  
  double Rangefinder::getProbOfBlock(){
    if (abs(estimatedDistance-longTermDistance)<stdDev){
      return 0;
    }
    else{
      return 1;
    }
  }
}
