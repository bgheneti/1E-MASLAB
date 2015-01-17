#ifndef INCLUDE_I2C_PWM_WRAPPER_H_
#define INCLUDE_I2C_PWM_WRAPPER_H_

#include "mraa.hpp"
#define SHIELD_I2C_ADDR 0x40
#define BUS_NUMBER 6

namespace utils {
    class I2cPwmWrapper {
        private:
            mraa::I2c i2c;
            char writeBuf[5];
            int outputPin;
        public:
            I2cPwmWrapper(int outputPin);
            void write(double duty);
           
        };

}

#endif 
