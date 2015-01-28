#include "../include/angle_localizer.h"
#include <vector>
#include <math.h>
#include <iostream>
#define PI 3.14159
namespace map{
  AngleLocalizer::AngleLocalizer(firmware::Rangefinder &front,
    firmware::Rangefinder &right, drive::DriveTrain &dt, Map m):
    front(front), right(right), dt(dt){
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
    std::cout << "setLocation" << xLoc << ", " << yLoc << std::endl;
    //for each angle
    for(int th = 0; th < 360; th++){
      //for this angle, where th=0 is directly upwards, the change in x and y
      stepX = sin(th*PI/180);
      stepY = cos(th*PI/180);
      
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
    std::cout << "location set" << std::endl;
  }
  
  int AngleLocalizer::getMax(std::vector<double> sta, std::vector<double> dyn, int lowerBound, int upperBound){
    std::cout << "getMax" << std::endl;
    std::vector<double> res(360, 0);
    double max = 0;
    int argmax = lowerBound;
    std::cout << "getMax starting loop" << std::endl;
    for (int n = lowerBound; n < upperBound; n++){
      for (int m = 0; m < 360; m++){
        res[n] += dyn[m]*sta[m+n];
      }
      
      if (res[n] > max){
        max = res[n];
        argmax = n;
      }
    }
    std::cout << "max gotten" << std::endl;
    return argmax;
  }
  
  int AngleLocalizer::getAngle(int numReadings){
    return getAngle(numReaedings, 0, 360);
  }
  
  int AngleLocalizer::getAngle(int numReadings, int lowerBound, int upperBound){
    std::cout << "getAngle" << std::endl;
    std::vector<double> readings(360, 0);
    double stepTh = (double)(upperBound - lowerBound) / numReadings;
    double currTh = lowerBound;
    std::cout << "stepping " << stepTh << std::endl;
    
    while(currTh < upperBound){
      for (int i = 0; i < 10; i++){
        front.getHighestProbDistance();
        right.getHighestProbDistance();
      }
      readings[(int)currTh] = front.getHighestProbDistance();
      readings[((int)currTh+90)%360] = right.getHighestProbDistance();
      dt.turnForDegrees(stepTh);
      currTh += stepTh;
      std::cout << "reached " << currTh << std::endl;;
    }
    std::cout << "done turning" << std::endl;
    return getMax(expected, readings, lowerBound, upperBound);
  }
}
