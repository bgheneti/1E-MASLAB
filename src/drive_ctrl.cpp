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
    void DriveTrain::controlPID(double distance, double heading) {
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
        while(leftEncoder.getDistance() < std::abs(distance) ||
              (distance < .0001 && std::abs(heading - gyro.getAngle()) > 1.0 )) { 
            std::cout << leftEncoder.getDistance();
            double diff = heading - gyro.getAngle();
            gettimeofday(&currentTime, NULL);
            double newCurrentMS = ((double)currentTime.tv_sec)*1000.0 +
                                  ((double)currentTime.tv_usec)/1000.0;
            double dT = newCurrentMS - currentMS;
            currentMS = newCurrentMS;
            integral += diff * dT;
            double derivative = gyro.getAngularV();
            power = P*diff + I*integral + D*derivative;
            double leftMotorSpeed = bias - .05 - power;
            double rightMotorSpeed = bias + power;
            if(std::abs(leftMotorSpeed) > .5) {
                leftMotorSpeed = leftMotorSpeed < 0 ? -.5 : .5;
            }
            if(std::abs(rightMotorSpeed) > .5) {
                rightMotorSpeed = rightMotorSpeed < 0 ? -.5: .5;
		
            }

            leftMotor.setSpeed(leftMotorSpeed);
            rightMotor.setSpeed(rightMotorSpeed);
            usleep(50000);
        }
	double MAX_ACCEL = .005;
	while(leftMotor.getSpeed() > .0001 || rightMotor.getSpeed() > .0001) {
            gettimeofday(&currentTime, NULL);
            double newCurrentMS = ((double)currentTime.tv_sec)*1000.0 +
                                  ((double)currentTime.tv_usec)/1000.0;
            double dT = newCurrentMS - currentMS;
	    currentMS = newCurrentMS;

            if(std::abs(rightMotor.getSpeed())/dT > MAX_ACCEL) {
		double sign = rightMotor.getSpeed() > 0 ? -1.0 : 1.0;
		rightMotor.setSpeed(MAX_ACCEL*dT*sign + rightMotor.getSpeed());
	    } else {
		rightMotor.setSpeed(0.0);
	    }
	    if(std::abs(leftMotor.getSpeed())/dT > MAX_ACCEL) {
		double sign = leftMotor.getSpeed() > 0 ? -1.0 : 1.0;
		leftMotor.setSpeed(MAX_ACCEL*dT*sign + leftMotor.getSpeed());
	    } else {
		leftMotor.setSpeed(0.0);
	    }
	    usleep(1000000);
	}
    }

    void DriveTrain::straightForDistance(double distance) {
        controlPID(distance, 0.0);
    }
            
    // Have the robot turn for some number of degrees. If degrees
    // is negative, turn left; if positive turn right.
    void DriveTrain::turnForDegrees(double degrees) {
        controlPID(0.0, degrees);
    }

}
