// This file contains functions to control the drive train of the
// robot.

#include "../include/drive_ctrl.h"
#include <sys/time.h>
#include <chrono>
#include <iostream>
namespace drive {

    DriveTrain::DriveTrain(firmware::Motor *leftMotor, 
                           firmware::Motor *rightMotor,
                           firmware::Encoder *leftEncoder,
                           firmware::Encoder *rightEncoder,
                           firmware::Gyro *gyro) : leftMotor(leftMotor),
                                                   rightMotor(rightMotor),
                                                   leftEncoder(leftEncoder),
                                                   rightEncoder(rightEncoder),
                                                   gyro(gyro),
                                                   bias(0),
                                                   power(0),
                                                   driving(false) {}

    void DriveTrain::resetSensors() {
        leftEncoder->resetNumTicks();
        rightEncoder->resetNumTicks();
        gyro->zeroAngle();
    
    }


    // Get the robot to start moving straight. Forward if direction > 0,
    // otherwise backward
    void DriveTrain::straightForDistance(double distance) {
        struct timeval currentTime;
        gettimeofday(&currentTime, NULL);
        double integral = 0;
        resetSensors();
        if(distance > 0) {
            bias = SPEED;
        } else {
            bias = -SPEED;
        }
        leftMotor->setSpeed(bias);
        rightMotor->setSpeed(bias);
        
        while(leftEncoder->getDistance() < std::abs(distance)) {
            double diff = -gyro->getAngle();
            double dT = ((double)currentTime.tv_sec)*1000.0 +
                        ((double)currentTime.tv_usec)/1000.0;
            integral += diff * dT;
            double derivative = gyro->getAngularV();
            power = P*diff + I*integral + D*derivative;
            leftMotor->setSpeed(bias + power);
            rightMotor->setSpeed(bias - power);
            usleep(100000);
        }
     	leftMotor->setSpeed(0.0);
   	    rightMotor->setSpeed(0.0);
    }

            
    // Have the robot turn for some number of degrees. If degrees
    // is negative, turn left; if positive turn right.
    void DriveTrain::turnForDegrees(double degrees) {

    }

}
