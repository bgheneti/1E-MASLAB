#ifndef INCLUDE_MOTOR_FIRMWARE_H_
#define INCLUDE_MOTOR_FIRMWARE_H_

#include "i2c_pwm_wrapper.h"

namespace firmware {
    class Motor {
        private:
            double speed;
            utils::I2cPwmWrapper pwm;
            utils::I2cPwmWrapper dir;
        public:
            // Creates a new motor object that has a pwm pin and a gpio direction pin
            Motor(int pwmPin, int dirPin);
            
            // Sets the speed of the motor to be newSpeed
            void setSpeed(double newSpeed);

            // Stops the motor abruptly
            void stop();

            // Gets the speed of the motor
            double getSpeed();
    };
}
#endif
