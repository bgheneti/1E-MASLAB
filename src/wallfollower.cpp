#include "../include/wallfollower.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <thread>

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
    double front = -1;//frontRF.getHighestProbDistance();
    double left = leftRF.getHighestProbDistance();
    double right = rightRF.getHighestProbDistance();
    
    std::cout << front << "\t" << left << "\t" << right << std::endl;
    double distanceFromWall = right;
    double otherDistance = left;
    if (dir == -1){
      distanceFromWall = left;
      otherDistance = right;
    }
    
    bool wd = (distanceFromWall != -1 && distanceFromWall < std::max(30.0, dist + 10));
    bool od = (otherDistance != -1 && distanceFromWall < std::max(30.0, dist + 10));
    bool f = (front != -1 && distanceFromWall < std::max(30.0, dist + 10));
    
    if (!wd && !od && !f){
      std::cout << "all clear" << std::endl;
      driveTrain.straightForDistance(std::min(dist/400, 0.05));
      std::cout << "done" << std::endl;
    }
    else if (wd && !od && !f){
      std::cout << "next to wall only" << std::endl;
      if (distanceFromWall - dist > std::min(dist/800, 0.025)){ //if far from the wall
        driveTrain.turnForDegrees(10*dir);
      }
      else if (dist - distanceFromWall > std::min(dist/800, 0.025)){ //if close to the wall
        driveTrain.turnForDegrees(-10*dir);
      }
      driveTrain.straightForDistance(std::min(dist/400, 0.05));
      std::cout << "done" << std::endl;
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
      driveTrain.straightForDistance(std::min(dist/400, 0.05));
    }
    else if (!wd && od && f){
      driveTrain.turnForDegrees(-10*dir);
    }
    else {
      driveTrain.turnForDegrees(-10*dir);
    }
    
    
    std::cout << "DONE" << std::endl;
    
  }
  
  void Wallfollower::runner(){
    while (running){
      step();
      //usleep(50000);
      std::cout << "running" << std::endl;
    }
  }
  
  void Wallfollower::start(){
    running = 1;
    r = std::thread(&Wallfollower::runner, this);
    std::cout << "started running " << std::endl;
  }
  
  void Wallfollower::stop(){
    running = 0;
    r.~thread();
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
