#include "../include/rangefinder_firmware.h"

namespace firmware{
  Rangefinder::Rangefinder(int irPin, int usTriggerPin, int usEchoPin):ir(irPin), us(usTriggerPin, usEchoPin){
    stdDev = -1;
    estimatedDistance=-1; //the short-term distance (resistant to change)
    longTermDistance=-1; //the long-term distance (extremely resistant to change)
  }
  
  //returns an estimate of distance
  double Rangefinder::getHighestProbDistance(){
    double uvN = 5, irN = 30; //number of samples for each sensor
    
    //whether the sensors should look for things in the range of 0-20 cm or 20-? cm
    bool close = (estimatedDistance != -1) && (estimatedDistance < 20);
    
    //get uv info
    double uvDist = uv.getHighestProbDistance(uvN);
    double uvS = uv.getStdDev(uvN, uvDist);
    
    //get ir info
    double irReading = ir.getReading(irN);
    double irDist = ir.getHighestProbDistance(close, irReading);
    double irS = ir.getStdDev(close, irN, irReading);
    
    double currDistance = -1; //the distance from this set of readings only
    
    //find an estimate of currDistance
    if (uvDist == -1){//if no uv reading was obtained
      currDistance = irDist; //use only the ir one
      stdDev = irS;
    }
    else{ //if there was a uv reading
      if (uvDist < irHigher && uvDist > irLower){ //if uv seems reasonable
        currDistance = (uvDist+irDist)/2; //average ir and uv
        stdDev = (uvS + irS)/2;
      }
      else { //if uv is not reasonable
        //try the other side of the curve for the ir sensor
        irDist = ir.getHighestProbDistance(!close, irReading);
        irLower = ir.getLowerBound(!close, irN, irReading);
        irHigher = ir.getHigherBound(!close, irN, irReading);
        
        //if uv seems reasonable now
        if (uvDist < irHigher && uvDist > irLower){
          currDistance = (uvDist + irDist)/2; //average the two
          stdDev = (uvS + irS)/2;
        }
        else{ //if uv is still unreasonable
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
        estimatedDistance = estimatedDistance*0.9+uvDist*0.1; //factor it in
        longTermDistance = longTermDistance*0.99+uvDist*0.01;
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
