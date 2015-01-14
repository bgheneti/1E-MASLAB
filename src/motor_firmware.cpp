#include <cassert>
#include <cmath>
#include <iostream>

#include "../include/motor_firmware.h"

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
        assert(-1.0 <= newSpeed && newSpeed <= 1.0);
        if (newSpeed > 0) {
            dir.write(0.0);
        } else {
            dir.write(1.0);
        }
        pwm.write(fabs(newSpeed));
        speed = newSpeed;
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
