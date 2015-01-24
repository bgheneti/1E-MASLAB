#include "../include/wallfollower.h"
#include <algorithm>
#include <cmath>

namespace control{
  
  Wallfollower::Wallfollower(drive::DriveTrain &driveTrain,
    firmware::Rangefinder &frontRF,
    firmware::Rangefinder &leftRF,
    firmware::Rangefinder &rightRF):driveTrain(driveTrain),
                                    frontRF(frontRF),
                                    leftRF(leftRF),
                                    rightRF(rightRF){
    running = 0;
    dir = 1;
    dist = 20;
  }
  
  void Wallfollower::step(){
    //get distances from rangefinders
    frontRF.getHighestProbDistance();
    leftRF.getHighestProbDistance();
    rightRF.getHighestProbDistance();
    double front = frontRF.getHighestProbDistance();
    double left = leftRF.getHighestProbDistance();
    double right = rightRF.getHighestProbDistance();
    
    double distanceFromWall = right;
    double otherDistance = left;
    if (dir == -1){
      distanceFromWall = left;
      otherDistance = right;
    }
    
    bool wd = (distanceFromWall != -1);
    bool od = (otherDistance != -1);
    bool f = (front != -1);
    
    if (!wd && !od && !f){
      driveTrain.straightForDistance(std::min(dist/4, 5.0));
    }
    else if (wd && !od && !f){
      if (distanceFromWall - dist > std::min(dist/8, 2.5)){ //if far from the wall
        driveTrain.turnForDegrees(10*dir);
      }
      else if (dist - distanceFromWall > std::min(dist/8, 2.5)){ //if close to the wall
        driveTrain.turnForDegrees(-10*dir);
      }
      driveTrain.straightForDistance(std::min(dist/4, 5.0));
    }
    else if (!wd && od && !f){
      if (od > 10){
        driveTrain.turnForDegrees(-10*dir);
      }
      else{
        driveTrain.turnForDegrees(10*dir);
      }
    }
    else if (!wd && !od && f){
      if (f > 10){
        driveTrain.turnForDegrees(-10*dir);
      }
      else{
        driveTrain.straightForDistance(-10);
      }
    }
    else if (wd && !od && f){
      driveTrain.turnForDegrees(-10*dir);
    }
    else if (wd && od && !f){
      if (od - wd > 5){
        driveTrain.turnForDegrees(-10*dir);
      }
      else if (wd - od > 5){
        driveTrain.turnForDegrees(10*dir);
      }
      driveTrain.straightForDistance(std::min(dist/4, 5.0));
    }
    else if (!wd && od && f){
      driveTrain.turnForDegrees(-10*dir);
    }
    else {
      driveTrain.turnForDegrees(-10*dir);
    }
    
    
    
    
  }
  
  void Wallfollower::runner(){
    while (running){
      step();
      usleep(50000);
      
    }
  }
  
  void Wallfollower::start(){
    running = 1;
    runner();
  }
  
  void Wallfollower::stop(){
    running = 0;
  }
  
  void Wallfollower::setDist(int d){
    dist = d;
  }
  
  double Wallfollower::getDist(){
    return dist;
  }
  
  void Wallfollower::setDir(int d){
    dir = d;
  }
  
  int Wallfollower::getDir(){
    return dir;
  }

}
