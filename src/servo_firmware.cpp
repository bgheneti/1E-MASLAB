#include "../include/servo_firmware.h"
#include <iostream>
#include "mraa.hpp"
namespace firmware {
    Servo::Servo(int outputPin) : i2c(outputPin) {
    }
 
    // The servo position is measured from the neutral position.
    // +90 degrees is counterclockwise and -90 degrees is clockwise.
    void Servo::setServoPosition(double degrees) {
        double onTime = degrees/90.0 * 875.0 + 1450.0;
        double duty = onTime * .00004;
        i2c.write(duty);     
    } 
    void Servo::off() {
	i2c.write(0.0);
    }


}
