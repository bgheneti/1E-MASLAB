// This file contains functions to control the drive train of the
// robot.

#include "../include/drive_ctrl.h"
#include <sys/time.h>
#include <chrono>
#include <iostream>
#define SPEED 0.25
#define P .037
#define I 0.000001
#define D -2.3
namespace drive {

    DriveTrain::DriveTrain(firmware::Motor &leftMotor, 
                           firmware::Motor &rightMotor,
                           firmware::Encoder &leftEncoder,
                           firmware::Encoder &rightEncoder,
                           firmware::Gyro &gyro,
                           firmware::Rangefinder &front) : leftMotor(leftMotor),
                                                   rightMotor(rightMotor),
                                                   leftEncoder(leftEncoder),
                                                   rightEncoder(rightEncoder),
                                                   gyro(gyro),
                                                   bias(0),
                                                   power(0),
         					   angleOffset(0),
                                                   driving(false),
       						   front(front) {}

    void DriveTrain::resetSensors() {
        leftEncoder.resetCount();
        rightEncoder.resetCount();
        gyro.zeroAngle();
	std::cout << "angle zeroed" << std::endl;
    
    }


    // Control the robot's motion.
    double DriveTrain::controlPID(double distance, double heading, double tolerance, double wallAllowance) {
      // std::cout << "entered driveControl, tolerance="<<tolerance << std::endl;
	
        struct timeval currentTime;
        gettimeofday(&currentTime, NULL);
	double currentMS = ((double)currentTime.tv_sec)*1000.0 +
			   ((double)currentTime.tv_usec)/1000.0;
        double integral = 0;
        resetSensors();
        if(distance > 0) {
            bias = SPEED;
        } else if(distance < 0) {
            bias = -SPEED;
        } else {
            bias = 0;
        }
	int correctAngleCount = 0;
        //std::cout << "front " << front.getHighestProbDistance() << std::endl;
        while((front.getHighestProbDistance() > wallAllowance*100 || front.getHighestProbDistance() < 0) && 
              (std::abs(leftEncoder.getCount()) < std::abs(distance) ||
              (distance < .0001 && correctAngleCount < 5 ))) {
//	    std::cout<<"distance="<<distance<<std::endl;
//	    std::cout << (distance < .0001) << "no distance" << std::endl;
	  double diff = (heading - gyro.getAngle())+angleOffset;
	  //	  std::cout << diff << std::endl;

	    if(std::abs(diff) < tolerance) {
		correctAngleCount++;
		//std::cout << correctAngleCount << "correct angle count" << std::endl;
	    } else {
		correctAngleCount = 0;
	    }


            gettimeofday(&currentTime, NULL);
            double newCurrentMS = ((double)currentTime.tv_sec)*1000.0 +
                                  ((double)currentTime.tv_usec)/1000.0;
            double dT = newCurrentMS - currentMS;
            currentMS = newCurrentMS;
            integral += diff * dT;
            double derivative = gyro.getAngularV();
            power = P*diff + I*integral + D*derivative;
            double leftMotorSpeed = bias + power;
            double rightMotorSpeed = bias - power;
	   
            leftMotor.setSpeed(leftMotorSpeed);
            rightMotor.setSpeed(rightMotorSpeed);
            usleep(5000);
            
        }
        if(!(front.getHighestProbDistance() > wallAllowance*100 || front.getHighestProbDistance() < 0)){std::cout << "STOPPED BECAUSE TOO CLOSE TO WALL" << std::endl;}
	leftMotor.setSpeed(0.0);
        rightMotor.setSpeed(0.0);
        angleOffset = heading-gyro.getAngle();
        
        return leftEncoder.getCount();
    }

    double DriveTrain::straightForDistance(double distance, double wallAllowance) {
        return controlPID(distance, 0.0, 1.0, wallAllowance);
    }
            
    // Have the robot turn for some number of degrees. If degrees
    // is negative, turn left; if positive turn right.
    double DriveTrain::turnForDegrees(double degrees, double tolerance) {
        if (std::abs(degrees) > std::abs(tolerance)){
          std::cout <<"turning " <<  degrees << ", " << tolerance << std::endl;
          controlPID(0.0, degrees, tolerance, 0.0);
        }
        else{
          std::cout << "STOPPED BECAUSE ANGLE TO TURN WAS TOO SMALL " << degrees << ", " << tolerance << std::endl;
        }
        return gyro.getAngle();
    }
    

}
