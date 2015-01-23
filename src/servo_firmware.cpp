#include "../include/servo_firmware.h"
#include <iostream>
<<<<<<< HEAD
=======
#include "mraa.hpp"
>>>>>>> 1c5985843c50e35f4db7cfa6f4e7c7032c8f8e03
namespace firmware {
    Servo::Servo(int register) : i2c(6) {
    }
 
    // The servo position is measured from the neutral position.
    // +90 degrees is counterclockwise and -90 degrees is clockwise.
    void Servo::setServoPosition(double degrees) {
        double onTime = degrees/90.0 * 875.0 + 1450.0;
        double duty = onTime * .00004;
        i2c.write(duty);     
    } 


}
