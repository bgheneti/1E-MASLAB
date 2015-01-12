#ifndef INCLUDE_SERVO_FIRMWARE_H_
#define INCLUDE_SERVO_FIRMWARE_H_

#include "mraa.hpp"
#include "../include/i2c_pwm_wrapper.h"

#define SHIELD_I2C_ADDR 0x40
namespace firmware {
    class Servo {
        private:
            utils::I2cPwmWrapper i2c;
        public:
            Servo(int outputPin);
            
            // The servo position is measured from the neutral position.
            // +90 degrees is counterclockwise and -90 degrees is clockwise.
            void setServoPosition(double degrees);
   };
}
#endif
