#include <cassert>
#include <iostream>
#include "../include/color_firmware.h"
#define MS 1000
namespace utils {

    Color::Color(int outputPin) : i2c(BUS_NUMBER) {
        i2c.address(ADDR);

	i2c.writeByte(CMD|ENA);
	i2c.write(1); //enable clock
	usleep(3 * MS);
    
	i2c.write(CMD|ENA);
	i2c.write(3); //enable clock and ADC
	usleep(3 * MS);
    }


    int Color::readColor(uchar color){
        i2c.address(ADDR);
	i2c.writeByte(CMD|WORD|color);
	int result=i2c.readByte();
	result|=i2c.readByte()<<8;
	return result;
    }

    int[3] Color::getColor(double duty) {
        rgb[0]=readColor(RED);
	rgb[1]=readColor(GREEN);
	rgb[2]=readColor(BLUE);
	return rgb;
    }
}

