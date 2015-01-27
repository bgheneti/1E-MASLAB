#include "../include/angle_localizer.h"
#include <vector>
#include <math.h>
#include <iostream>
#define PI 3.14159
namespace map{
  AngleLocalizer::AngleLocalizer(firmware::Rangefinder &rf,
    drive::DriveTrain &dt, Map &probMap):
    rf(rf), dt(dt), probMap(probMap){
      //initialize a blank copy of expected
      expected = std::vector<double>(720, 0);
      
      //copy grid from map into probMap
      std::vector<std::vector<Element>> g = m.getGrid();
      Element e;
      for (int r = 0; r < g.size(); r++){
        for (int c = 0; c < g[0].size(); c++){
          e = g[r][c];
          if (e == WALL || e == OUT_OF_BOUNDS){probMap[r][c] = 1;}
          else{probMap[r][c] = 0;}
          
        }
      }
      
  }
  
  void AngleLocalizer::setLocation(int xl, int yl){
    xLoc = xl;
    yLoc = yl;
    double currX = xLoc;
    double currY = yLoc;
    double stepX, stepY;
    
    //for each angle
    for(int th = 0; th < 360; th++){
      //for this angle, where th=0 is directly upwards, the change in x and y
      stepX = sin(th);
      stepY = cos(th);
      
      //step until a wall is found.
      while (probMap[(int)currY][(int)currX] == 0){
        currX += stepX;
        currY += stepY;
      }
      
      //store expected distances
      expected[th] = sqrt((currX-xLoc)*(currX-xLoc) + (currY-yLoc)*(currY-yLoc));
      expected[th+360] = expected[th];
      
      //reset
      currX = xLoc;
      currY = yLoc;
    }
    
  }
  
  int AngleLocalizer::getMax(std::vector<double> sta, std::vector<double> dyn){
    std::vector<double> res(360, 0);
    double max = 0;
    int argmax = 0;
    
    for (int n = 0; n < 360; n++){
      for (int m = 0; m < 360; m++){
        res[n] += dyn[m]*sta[m+n];
      }
      
      if (res[n] > max){
        max = res[n];
        argmax = n;
      }
    }
    
    return argmax;
  }
  
  int AngleLocalizer::getAngle(int numReadings){
    std::vector<double> readings(360, 0);
    double stepTh = 360.0 / numReadings;
    double currTh = 0;
    
    while(currTh < 360){
      for (int i = 0; i < 10; i++){rf.getHighestProbDistance();}
      readings[(int)currTh] = rf.getHighestProbDistance();
      dt.turnForDegrees(stepTh);
      currTh += stepTh;
      
    }
    
    return getMax(expected, readings);
  }
}
