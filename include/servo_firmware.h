#ifndef INCLUDE_SERVO_FIRMWARE_H_
#define INCLUDE_SERVO_FIRMWARE_H_

#include "mraa.hpp"

#define SHIELD_I2C_ADDR 0x40
namespace firmware {
    class Servo {
        private:
            utils::I2cWrapper i2c;
        public:
            Servo(int outputPin);
            
            // The servo position is measured from the neutral position.
            // +90 degrees is counterclockwise and -90 degrees is clockwise.
            void setServoPosition(double degrees);
    } 
#endif
