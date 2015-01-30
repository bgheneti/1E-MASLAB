#include <cassert>
#include <cmath>
#include <iostream>

#include "../include/motor_firmware.h"
#define minThreshold .15
// This file contains the code to control a motor on a basic level.
// These motors can be driving motors or other motors.
namespace firmware {
    // Initialize a new motor. Provide the pin for the pwm signal and the dir signal
    Motor::Motor(int pwmPin, int dirPin) : pwm(pwmPin), dir(dirPin), speed(0.0) {
	pwm.write(0.0);
	dir.write(0);
    }

    // Sets the speed of the motor. Speed can be a number between -1.0 and 1.0.
    // Negative speeds are backwards and positive speeds are forwards.
    void Motor::setSpeed(double newSpeed) {
	double setSpeed = newSpeed;
	if (setSpeed > 0.5) {
		setSpeed = 0.5;
	} else if (setSpeed < -.5) {
		setSpeed = -.5;
	}
/*	else if(setSpeed>-minThreshold && setSpeed<-0.03){
	  setSpeed=-minThreshold;
	  std::cout<<"setThreshold"<<std::endl;
	} else if (setSpeed<minThreshold && setSpeed>0.03){
	  setSpeed=minThreshold;
	} else if (std::abs(setSpeed)<=.03){
	  setSpeed=0;
	}
*/
        assert(-0.5 <= setSpeed && setSpeed <= 0.5);
        if (setSpeed > 0) {
            dir.write(1.0);
        } else {
            dir.write(0.0);
        }
        pwm.write(fabs(setSpeed));
        speed = setSpeed;
    }

    // Abruptly stops the motor (sets its speed to 0)
    void Motor::stop() {
        pwm.write(0.0);
        dir.write(0);
        speed = 0.0;
    }

    // Get the current speed of the motor
    double Motor::getSpeed() {
        return speed;
    }
} 
