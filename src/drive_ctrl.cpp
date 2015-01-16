// This file contains functions to control the drive train of the
// robot.

#include "../include/drive_ctrl.h"
#include <sys/time.h>
#include <chrono>
#include <iostream>
namespace drive {

    DriveTrain::DriveTrain(firmware::Motor &leftMotor, 
                           firmware::Motor &rightMotor,
                           firmware::Encoder &leftEncoder,
                           firmware::Encoder &rightEncoder,
                           firmware::Gyro &gyro) : leftMotor(leftMotor),
                                                   rightMotor(rightMotor),
                                                   leftEncoder(leftEncoder),
                                                   rightEncoder(rightEncoder),
                                                   gyro(gyro),
                                                   bias(0),
                                                   power(0),
                                                   driving(false) {}

    void DriveTrain::resetSensors() {
        leftEncoder.resetNumTicks();
        rightEncoder.resetNumTicks();
        gyro.zeroAngle();
    
    }


    // Control the robot's motion.
    void DriveTrain::straightForDistance(double distance, double heading) {
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
        leftMotor.setSpeed(bias);
        rightMotor.setSpeed(bias);
        while(leftEncoder.getDistance() < std::abs(distance) ||
              (distance < .0001 && std::abs(heading - gyro.getAngle) < 2.0 )) { 
            double diff = heading - gyro.getAngle();
            gettimeofday(&currentTime, NULL);
            double newCurrentMS = ((double)currentTime.tv_sec)*1000.0 +
                                  ((double)currentTime.tv_usec)/1000.0;
            double dT = newCurrentMS - currentMS;
            currentMS = newCurrentMS;
            integral += diff * dT;
            double derivative = gyro.getAngularV();
            power = P*diff + I*integral + D*derivative;
            double leftMotorSpeed = bias - power;
            double rightMotorSpeed = bias + power;
            if(std::abs(leftMotorSpeed) > .3) {
                leftMotorSpeed = leftMotorSpeed < 0 ? -.3 : .3;
            }
            if(std::abs(rightMotorSpeed) > .3) {
                rightMotorSpeed = rightMotorSpeed < 0 ? -.3: .3;
            }
            leftMotor.setSpeed(leftMotorSpeed);
            rightMotor.setSpeed(rightMotorSpeed);
            usleep(50000);
        }
     	leftMotor.setSpeed(0.0);
   	    rightMotor.setSpeed(0.0);
    }
            
    // Have the robot turn for some number of degrees. If degrees
    // is negative, turn left; if positive turn right.
    void DriveTrain::turnForDegrees(double degrees) {
    }

}
