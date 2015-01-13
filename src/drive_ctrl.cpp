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
                                                   diff(0),
                                                   driving(false) {}

    void DriveTrain::resetSensors() {
        leftEncoder->resetNumTicks();
        rightEncoder->resetNumTicks();
        gyro->zeroAngle();
    
    }

    void DriveTrain::correctHeading() {
    
    }

    // Get the robot to start moving straight. Forward if direction > 0,
    // otherwise backward
    void DriveTrain::straightForDistance(double direction) {

        resetSensors();
        if(direction > 0) {
            bias = SPEED;
        } else {
            bias = -SPEED;
        }
	leftMotor->setSpeed(bias);
	rightMotor->setSpeed(bias);

        usleep(2000000); 
	leftMotor->setSpeed(0.0);
	rightMotor->setSpeed(0.0);
        /*struct timeval currentTime;
        gettimeofday(&currentTime, NULL);
        double integral = 0;
	
        while(driving) {
            speedLock.lock();
            // PID Control
            double delta = -gyro->getAngle();
            gettimeofday(&currentTime, NULL);
            double dT = ((double) currentTime.tv_sec)*1000.0 + 
			((double) currentTime.tv_usec)/1000.0;
            integral += delta * dT;
            double derivative = gyro->getAngularV();
            diff = P*delta + I*integral + D*derivative;
            speedLock.unlock();
        }
        maintainSpeedThread.join();
        */
    }

            
    // Have the robot turn for some number of degrees. If degrees
    // is negative, turn left; if positive turn right.
    void DriveTrain::turnForDegrees(double degrees) {

    }

}
