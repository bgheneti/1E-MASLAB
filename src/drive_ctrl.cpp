// This file contains functions to control the drive train of the
// robot.

#include "../include/drive_ctrl.h"
#include <sys/time.h>
#include <chrono>
#include <iostream>
#define SPEED 0.25
#define P .037
#define I 0.0000024
#define D -2.3
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
        leftEncoder.resetCount();
        rightEncoder.resetCount();
        gyro.zeroAngle();
	std::cout << "angle zeroed" << std::endl;
    
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
	int correctAngleCount = 0;
        while(std::abs(leftEncoder.getCount()) < std::abs(distance) ||
              (distance < .0001 && correctAngleCount < 5 )) {
	    std::cout<<"distance="<<distance<<std::endl;
	    std::cout << (distance < .0001) << "no distance" << std::endl;
	    if(std::abs(heading - gyro.getAngle()) < 1.0) {
		correctAngleCount++;
		std::cout << correctAngleCount << "correct angle count" << std::endl;
	    } else {
		correctAngleCount = 0;
	    }
            double diff = (heading - gyro.getAngle());
            std::cout << gyro.getAngle() << std::endl;

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
            usleep(50000);
        }
	leftMotor.setSpeed(0.0);
        rightMotor.setSpeed(0.0);
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
