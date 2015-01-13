// This file contains functions to control the drive train of the
// robot.

#include "../include/drive_ctrl.h"
#include <sys/time.h>
namespace drive {

    DriveTrain::DriveTrain(firmware::Motor *leftMotor, 
                           firmware::Motor *rightMotor,
                           firmware::Encoder *leftEncoder,
                           firmware::Encoder *rightEncoder,
                           firmware::Gyro *gyro) {
	leftMotor = leftMotor;
	rightMotor = rightMotor;
	leftEncoder = leftEncoder;
	rightEncoder = rightEncoder;
	gyro = gyro;
        bias = 0;
        diff = 0;
        driving = false;
    }                    

    // Return requestedSpeed if it doesn't change the current speed by more
    // than .2. Otherwise change the speed by .2 in the correct direction.
    void DriveTrain::trySetMotorSpeed(double requestedSpeed, firmware::Motor motor) {
	double currentSpeed = motor.getSpeed();
        double diff = requestedSpeed - motor.getSpeed();
    
        if(std::abs(diff) < .2 && std::abs(requestedSpeed) < 1.0) {
            motor.setSpeed(requestedSpeed);
        } else {
            if(diff > 0 && currentSpeed <= .8) {
                motor.setSpeed(currentSpeed + .2);
            } else if(diff < 0 && currentSpeed >= -.8) {
                motor.setSpeed(currentSpeed - .2);
            }
        }
    }

    // Maintain the bias and the diff in the left and right motors.
    // Don't let the speeds change abruptly.
    void DriveTrain::maintainSpeeds() {
        while(driving) {
            speedLock.lock();

            double requestedLeftSpeed = bias + diff;
            double requestedRightSpeed = bias - diff;

            DriveTrain::trySetMotorSpeed(requestedLeftSpeed, *leftMotor);
            DriveTrain::trySetMotorSpeed(requestedRightSpeed, *rightMotor);

            speedLock.unlock();
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
        maintainSpeeds();

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
    }

    // Get the robot to turn. If direction is negative, turn left;
    // if positive turn right.
    void DriveTrain::turn(int direction) {

    }

    // Have the robot move straight until some point. If distance > 0,
    // go forward, otherwise go backward
    void DriveTrain::straightForDistance(double distance) {

    }
            
    // Have the robot turn for some number of degrees. If degrees
    // is negative, turn left; if positive turn right.
    void DriveTrain::turnForDegrees(double degrees) {

    }

}
