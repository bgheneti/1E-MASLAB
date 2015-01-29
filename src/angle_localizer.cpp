#include "../include/angle_localizer.h"
#include <vector>
#include <math.h>
#include <iostream>
#include <queue>
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
        std::vector<double> b;
        probMap.push_back(b);
        for (int c = 0; c < g[0].size(); c++){
          e = g[r][c];
          if (e == WALL){probMap[r].push_back(1);}
          else{probMap[r].push_back(2);}
          
        }
        
      }
      
      //find a point that is known to be in the playing field
      utils::Point p = m.getNearestStack();
      int startX = p.x;
      int startY = p.y;
      
      //queue in which to store x and y pairs that are known to be in the playing field
      std::queue<int> xs, ys;
      xs.push(startX);
      ys.push(startY);
      int currX, currY;
      while(xs.size() != 0){ //while still looking
        currX = xs.front(); //pull off first two values
        xs.pop();
        currY = ys.front();
        ys.pop();
        //make sure that point is marked as clear
        probMap[currY][currX] = 0;
        
        //look for adjacent grid boxes that are known to be in the playing field
        if (currX + 1 < probMap[0].size() && probMap[currY][currX+1]==2){
          xs.push(currX+1); //add them to the queue
          ys.push(currY);
          probMap[currY][currX+1] = 0;
        } //etc
        if (currX - 1 >= 0 && probMap[currY][currX-1] == 2){
          xs.push(currX-1);
          ys.push(currY);
          probMap[currY][currX-1] = 0;
        }
        if (currY + 1 < probMap.size() && probMap[currY+1][currX] == 2){
          xs.push(currX);
          ys.push(currY+1);
          probMap[currY+1][currX] = 0;
        }
        if (currY - 1 >= 0 && probMap[currY-1][currX] == 2){
          xs.push(currX);
          ys.push(currY-1);
          probMap[currY-1][currX] = 0;
        }
      }
      
      //for now, print out probMap
      /*for (int r = 0; r < probMap.size(); r++){
        for (int c = 0; c < probMap[r].size(); c++){
          std::cout << probMap[r][c];
        }
        std::cout << std::endl;
      }*/
  }
  
  void AngleLocalizer::setLocation(int xl, int yl){
    xLoc = xl;
    yLoc = yl;
    if (probMap[yLoc][xLoc] != 0){
      if (probMap[yLoc+1][xLoc] == 0){yLoc += 1;}
      else if (probMap[yLoc - 1][xLoc] == 0){yLoc -= 1;}
      else if (probMap[yLoc][xLoc+1] == 0){xLoc += 1;}
      else if (probMap[yLoc][xLoc-1] == 0){xLoc -= 1;}
      else{std::cout << "issue with starting location" << std::endl;}
    }
    double currX = xLoc;
    double currY = yLoc;
    double stepX, stepY;
    std::cout << "setLocation" << xLoc << ", " << yLoc << std::endl;
    //for each angle
    for(int th = 0; th < 360; th++){
      //for this angle, where th=0 is directly upwards, the change in x and y
      stepX = sin(th*PI/180.0)/2;
      stepY = cos(th*PI/180.0)/2;
      //std::cout << stepX << ", " << stepY << std::endl;
      //step until a wall is found.
      while ((((int)xLoc == (int)currX) && ((int)yLoc == (int)currY)) || probMap[(int)currY][(int)currX] == 0){
        currX += stepX;
        currY += stepY;
        //std::cout << "a" << std::endl;
      }
      
      //store expected distances
      expected[th] = sqrt((currX-(double)xLoc)*(currX-(double)xLoc) + (currY-(double)yLoc)*(currY-(double)yLoc));
      expected[th+360] = expected[th];
      //std::cout << th << ": " <<  expected[th] << std::endl;
      //reset
      currX = xLoc;
      currY = yLoc;
    }
    std::cout << "location set" << std::endl;
  }
  
  int AngleLocalizer::getMax(std::vector<double> sta, std::vector<double> dyn, int lowerBound, int upperBound){
    std::cout << "getMax" << std::endl;
    std::vector<double> res(360, 0);
    double max = 1000000;
    int argmax = lowerBound;
    std::cout << "getMax starting loop" << std::endl;
    for (int n = lowerBound; n < upperBound; n++){
      for (int m = 0; m < 360; m++){
        if (dyn[m] != 0){
          res[n] += (dyn[m]-sta[m+n])*(dyn[m]-sta[m+n]);
          //std::cout << dyn[m] << ", " << sta[m+n] << std::endl;
        }
        
      }
      if (res[n] < max){
        max = res[n];
        argmax = n;
      }
    }
    std::cout << "max gotten" << std::endl;
    return argmax;
  }
  
  int AngleLocalizer::getAngle(int numReadings){
    return getAngle(numReadings, 0, 360);
  }
  
  int AngleLocalizer::getAngle(int numReadings, int lowerBound, int upperBound){
    std::cout << "getAngle" << std::endl;
    std::vector<double> readings(360, 0);
    
    
    /*std::vector<double> testReadings(360, 0);
    testReadings[0] = 3;
    testReadings[36] = 16;
    testReadings[72] = 12;
    testReadings[108] = 5;
    testReadings[144] = 6;
    testReadings[180] = 16;
    testReadings[216] = 2;
    testReadings[252] = -1;
    testReadings[288] = 1;
    testReadings[324] = 1;
    */
    
    
    double stepTh = (double)(upperBound - lowerBound) / numReadings;
    double currTh = lowerBound;
    std::cout << "stepping " << stepTh << std::endl;
    
    while(currTh < upperBound){
      for (int i = 0; i < 5; i++){
        front.getHighestProbDistance();
        right.getHighestProbDistance();
      }
      readings[(int)currTh] = front.getHighestProbDistance()/10;
      readings[((int)currTh+90)%360] = right.getHighestProbDistance()/10;
      dt.turnForDegrees(stepTh);
      
      //readings[(int)currTh] = testReadings[(int)currTh];
      currTh += stepTh;
      std::cout << "reached " << currTh << std::endl;
    }
    
    std::cout << "done turning" << std::endl;
    return getMax(expected, readings, lowerBound, upperBound);
  }
}
