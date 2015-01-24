#ifndef INCLUDE_COLOR_FIRMWARE_H_
#define INCLUDE_COLOR_FIRMWARE_H_

#include "mraa.hpp"
#include <thread>

#define SHIELD_I2C_ADDR 0x40
#define ADDR 0x29
#define CMD 0x80
#define WORD 0x20
#define ENA 0x00
#define WHITE 0x14
#define RED 0x16
#define GREEN 0x18
#define BLUE 0x1A

namespace firmware{
    class Color {
        private:
            mraa::I2c i2c;
            uint16_t rgb[3];
            int running;
	    std::thread runner;
	    uint16_t readColor(int color);
        public:
	    Color();
	    uint16_t* getColor();
    };
}
#endif
