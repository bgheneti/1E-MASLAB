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

    // Return requestedSpeed if it doesn't change the current speed by more
    // than .2. Otherwise change the speed by .2 in the correct direction.
    void DriveTrain::trySetMotorSpeed(double requestedSpeed, firmware::Motor motor) {
	double currentSpeed = motor.getSpeed();
        double delta = requestedSpeed - motor.getSpeed();
    
        if(std::abs(delta) < .2 && std::abs(requestedSpeed) < 1.0) {
            motor.setSpeed(requestedSpeed);
        } else {
            if(delta > 0 && currentSpeed <= .8) {
                motor.setSpeed(currentSpeed + .2);
            } else if(delta < 0 && currentSpeed >= -.8) {
                motor.setSpeed(currentSpeed - .2);
            }
        }
	std::cout << "currentSpeed: " << currentSpeed << "new speed: " << motor.getSpeed() << std::endl;
    }

    // Maintain the bias and the diff in the left and right motors.
    // Don't let the speeds change abruptly.
    void DriveTrain::maintainSpeeds() {
        while(driving) {
            speedLock.lock();

            double requestedLeftSpeed = bias + diff;
            double requestedRightSpeed = bias - diff;
            std::cout << "leftSpeed: " << leftMotor->getSpeed() << std::endl;
            DriveTrain::trySetMotorSpeed(requestedLeftSpeed, *leftMotor);
            DriveTrain::trySetMotorSpeed(requestedRightSpeed, *rightMotor);
            std::cout << "leftSpeed: " << leftMotor->getSpeed() << std::endl;
            speedLock.unlock();
            std::chrono::milliseconds sleep_time(2000);
            std::this_thread::sleep_for(sleep_time);
 
        }
    }

    void DriveTrain::resetSensors() {
        leftEncoder->resetNumTicks();
        rightEncoder->resetNumTicks();
        gyro->zeroAngle();
    
    }

    // Get the robot to start moving straight. Forward if direction > 0,
    // otherwise backward
    void DriveTrain::straight(int direction) {
        resetSensors();
        if(direction > 0) {
            bias = SPEED;
        } else {
            bias = -SPEED;
        }
        driving = true;
        std::thread maintainSpeedThread(&drive::DriveTrain::maintainSpeeds, this);

        struct timeval currentTime;
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
    }

    // Get the robot to turn. If direction is negative, turn left;
    // if positive turn right.
    void DriveTrain::turn(int direction) {

    }

    // Have the robot move straight until some point. If distance > 0,
    // go forward, otherwise go backward
    void DriveTrain::straightForDistance(double distance) {
        std::thread straightThread(&drive::DriveTrain::straight, this, distance);
        std::chrono::milliseconds sleep_time(5000);
        std::this_thread::sleep_for(sleep_time);
	diff=0;
	bias=0;
        driving = false;
        straightThread.join(); 
    }
            
    // Have the robot turn for some number of degrees. If degrees
    // is negative, turn left; if positive turn right.
    void DriveTrain::turnForDegrees(double degrees) {

    }

}
