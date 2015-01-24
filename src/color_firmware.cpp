#include <cassert>
#include <stdio.h>
#include <iostream>
#include "../include/color_firmware.h"
#define MS 1000

namespace firmware{

    Color::Color() : i2c(6) {
        printf("addr:%d\n",int(i2c.address(ADDR)));

	printf("writereg:%d\n",int(i2c.writeReg(ENA,0x01)));

	//	char writeBuf[2]={CMD|ENA,0x01};
	//i2c.write(writeBuf,2); //enable clock
	usleep(3 * MS);
	
	i2c.writeReg(ENA|0x20,0x03);
	//writeBuf[0]=CMD|ENA;
	//writeBuf[1]=0x03;
	//i2c.write(writeBuf,2); //enable clock and ADC   
	usleep(3 * MS);

        i2c.writeByte(0x80|0x12);
	int version=i2c.readReg(0x12);
	std::cout<<"version:"<<version<<std::endl;
    }

  /*
    uint16_t Color::readColor(int color){
        i2c.address(ADDR);
	i2c.writeByte(CMD|WORD|color);
	uint8_t readBuffer[2];
	i2c.read(readBuffer,2);
	uint16_t result=uint16_t(readBuffer[1])<<8+readBuffer[0];
	return result;
    }
  */

    uint16_t * Color::getColor() {
      uint8_t readBuffer[6];
      /*
        rgb[0]=readColor(RED);
	rgb[1]=readColor(GREEN);
	rgb[2]=readColor(BLUE);
      */
      uint16_t result=i2c.readWordReg(RED);
      rgb[0]=result;
      result=i2c.readWordReg(GREEN);
      rgb[1]=result;
      result=i2c.readWordReg(BLUE);
      rgb[2]=result;
      return rgb;
    }
}

