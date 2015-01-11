#ifndef INCLUDE_I2C_PWM_WRAPPER_H_
#define INCLUDE_I2C_PWM_WRAPPER_H_

#define SHIELD_I2C_ADDR 0x40

namespace utils {
    class I2cPwmWrapper {
        private:
            mraa::I2c i2c;
            char writeBuf[5];
            int outputPin;
        public:
            I2cPwmWrapper(int bus, outputPin);
            write(double duty);
           
        }

}

#endif 
