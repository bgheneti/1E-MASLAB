#include "../include/arduino_firmware.h"
#include <thread>
#include <iostream>

namespace firmware {

	Arduino::Arduino(int gpioPin) : ss(gpioPin), 
		//			spi(0),
					blockColor(utils::Color::NONE), 
					running(false) {
		ss.dir(mraa::Dir(0)); // DIR_OUTPUT
		ss.write(1);
	}

	utils::Color Arduino::getBlockColor() {
		return blockColor;
	}

	void Arduino::poll(mraa::Spi& spi) {
		std::cout << "polling arduino" << std::endl;
		char writeBuf[7] = {CLRLO, CLRHI, REDLO, REDHI,
				    GRNLO, GRNHI, 0xfe};
		char* rxBuf;
		ss.write(0);
		rxBuf = spi.write(writeBuf, 7);
		ss.write(1);
		uint16_t c = (unsigned char) (rxBuf[1]);
		c |= ((unsigned char) (rxBuf[2]))<<8;
		uint16_t r = (unsigned char) (rxBuf[3]);
		r |= ((unsigned char) (rxBuf[4]))<<8;
		uint16_t g = (unsigned char) (rxBuf[5]);
		g |= ((unsigned char) (rxBuf[6]))<<8;

		float ratio = float(r)/float(g);
		if(c>3000 && c<20000) {
			blockColor = ratio > 1.8 ? utils::Color::RED 
						 : utils::Color::GREEN;
		} else {
			blockColor = utils::Color::NONE;
		}
		free(rxBuf);
	}	
	

}
