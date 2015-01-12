#include <cassert>
#include <iostream>
#include "i2c_pwm_wrapper.h"

namespace utils {

    I2cPwmWrapper::I2cPwmWrapper(int outputPin) : i2c(BUS_NUMBER) {
        outputPin = outputPin;

        writeBuf[0] = 0x00; // Write to MODE 1 Register;
        writeBuf[1] = 1 << 4; // Enale Sleep Mode

        i2c.address(SHIELD_I2C_ADDR);
        i2c.write(writeBuf, 2);

        writeBuf[0] = 0xFE; // Write Prescalar Register
        writeBuf[1] = 0xA3; // Set pwm frequency to ~40 Hz

        i2c.address(SHIELD_I2C_ADDR);
        i2c.write(writeBuf, 2);

        writeBuf[0] = 0;
        writeBuf[1] = 1 << 5 //Enable auto-increment mode
                    | 0 << 4; // Enable the oscillator
    
        i2c.address(SHIELD_I2C_ADDR);
        i2c.write(writeBuf, 2);
    }

    void I2cPwmWrapper::write(double duty) {
        assert(0.0 <= duty && duty <= 1.0);
        int on = (int) (4095.0*duty);

        std::cout << on << std::endl;

        writeBuf[0] = 6 + 4*outputPin;
        writeBuf[1] = 0x00; // ON LSB
        writeBuf[2] = 0x00; // ON MSB
        writeBuf[3] = on & 0xFF; // OFF LSB
        writeBuf[4] = (on >> 8) & 0xFF; // OFF MSB
	i2c.address(SHIELD_I2C_ADDR);
	i2c.write(writeBuf, 5);
    }
}

