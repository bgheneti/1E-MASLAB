#include <cassert>
#include <cmath>

#include "../include/motor_firmware.h"

void setMotorSpeed(mraa::Pwm& pwm, mraa::Gpio& dir, double speed) {
    assert(-1.0 <= speed && speed <= 1.0);
    if (speed > 0) {
        dir.write(0);
    } else {
        dir.write(1);
    }
    pwm.write(fabs(speed));
}

    
