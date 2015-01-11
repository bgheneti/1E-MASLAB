#include "../include/servo_firmware.h"

namespace firmware {
    Servo::Servo(int register) : i2c(6) {
        i2c.initPWM();
    };
 
    // The servo position is measured from the neutral position.
    // +90 degrees is counterclockwise and -90 degrees is clockwise.
    void setServoPosition(double degrees) {
        double onTime = degrees/90.0 * 500.0 + 1500.0;
        double duty = onTime * .00004;
        
        i2c.writePWM(duty);     
    } 


}
