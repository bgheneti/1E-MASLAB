#include "../include/wallfollower.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <thread>

namespace control{
  
  Wallfollower::Wallfollower(drive::DriveTrain &driveTrain,
    firmware::Rangefinder &frontRF,
    firmware::Rangefinder &rightRF):driveTrain(driveTrain),
                                    frontRF(frontRF),
                                    rightRF(rightRF){
    running = 0;
    dir = 1;
    dist = 25;
  }
  
  void Wallfollower::turn(double degrees, double tolerance, double timeout){
    double res = driveTrain.turnForDegrees(degrees, tolerance, timeout);
    
    if (std::abs(res-degrees)>tolerance*2){
      std::cout << "timeout of turn " << std::endl;
      res = driveTrain.straightForDistance(-0.1, 0.0, 1);
      if (std::abs(res+0.1)>0.05){
        driveTrain.straightForDistance(0.1, 0.0, 1);
      }
      driveTrain.turnForDegrees(-degrees, tolerance, timeout);
    }
    
    
  }
  
  void Wallfollower::step(){
    //get distances from rangefinders
    double fAvg = 0;
    double rAvg = 0;
    int numRdgs = 5;
    for (int i = 0; i < numRdgs; i++){
      fAvg += frontRF.getHighestProbDistance();
      rAvg += rightRF.getHighestProbDistance();
    }
    
    
    
    double front = fAvg / numRdgs;
    double right = rAvg / numRdgs;
    
    std::cout << front << "\t"  << "\t" << right << std::endl;
    double distanceFromWall = right;
    double timeout = 5;
    double tolerance = 5;
    
    bool wd = (distanceFromWall != -1 && distanceFromWall < 30.0);
    bool f = (front != -1 && front < 20.0);
   
    if (!wd && !f){
      std::cout << "all clear" << std::endl;
      turn(20, tolerance, timeout);
      if (std::abs(driveTrain.straightForDistance(0.15, 0.1, timeout)-0.15)>0.05){
        driveTrain.straightForDistance(-0.15, 0.1, timeout);
        turn(-90, tolerance, timeout);
      }
      //turn(-20, tolerance, timeout);
      std::cout << "done" << std::endl;
    }
    else if (wd && !f){
      std::cout << "next to wall only" << std::endl;
      if (distanceFromWall - dist > std::min(dist/800, 0.025)){ //if far from the wall
        turn(20*dir, tolerance, timeout);
        if (std::abs(driveTrain.straightForDistance(0.15, 0.1, timeout)-0.15) > 0.05){
          driveTrain.straightForDistance(-0.15, 0.1, timeout);
          turn(-70, tolerance, timeout);
        }
        turn(-20*dir, tolerance, timeout);
      }
      else if (dist - distanceFromWall > std::min(dist/800, 0.025)){ //if close to the wall
        turn(-20*dir, tolerance, timeout);
          
        if (std::abs(driveTrain.straightForDistance(0.15, 0.1, timeout)-0.15)>0.05){
          turn(-55, tolerance, timeout);
          turn(-55, tolerance, timeout);
        }
        turn(20*dir, tolerance, timeout);
      }
      
      std::cout << "done" << std::endl;
    }
    
    else if (!wd && f){
      std::cout << "front only" << std::endl;
      turn(-90*dir, tolerance, timeout);
      if (std::abs(driveTrain.straightForDistance(0.2, 0.1, timeout)-0.2)>0.05){
        driveTrain.straightForDistance(-0.15, 0.1, timeout);
        turn(-90, tolerance, timeout);
      }
      
    }
    else if (wd && f){
      std::cout << "both" << std::endl;
      turn(-90*dir, tolerance, timeout);
      if (std::abs(driveTrain.straightForDistance(0.2, 0.1, timeout)-0.2)>0.05){
        driveTrain.straightForDistance(-0.15, 0.1, timeout);
        turn(-90, 2, timeout);
      }
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
    for (int i = 0; i < 10; i++){
      frontRF.getHighestProbDistance();
      rightRF.getHighestProbDistance();
    }

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
