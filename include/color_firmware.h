#ifndef INCLUDE_COLOR_FIRMWARE_H_
#define INCLUDE_COLOR_FIRMWARE_H_

#include "mraa.hpp"
#include <thread>
#include "../include/i2c_pwm_wrapper.h"

#define SHIELD_I2C_ADDR 0x40
namespace firmware {
    class Color {
        private:
            uchar rgb[3];
            int running;
	    utils::I2cPwmWrapper i2c;
	    std::thread runner;
	    void poll();
        public:
	    Color();
	    uchar[3] getColor();
	    void startPolling();
	    void stopPolling();
    };
}

#endif
