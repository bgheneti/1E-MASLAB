#include <cassert>
#include <cmath>
#include <csignal>
#include <iostream>

#include "../include/motor_firmware.h"

void setMotorSpeed(mraa::Pwm& pwm, mraa::Gpio& dir, double speed) {
    assert(-1.0 <= speed && speed <= 1.0);
    if (speed < 0) {
        dir.write(1);
    } else {
        dir.write(0);
    }
    pwm.write(fabs(speed));
}

    
