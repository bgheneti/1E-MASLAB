#ifndef INCLUDE_COLOR_FIRMWARE_H_
#define INCLUDE_COLOR_FIRMWARE_H_

#include "mraa.hpp"
#include <thread>

#define SHIELD_I2C_ADDR 0x40
namespace firmware {
    class Color {
        private:
            uchar rgb[3];
            int running;
	    std::thread runner;
        public:
	    Color();
	    uchar * getColor();
    };
}
#endif
