#include "../include/color_sensor_firmware.h"
#include <thread>
#include <iostream>
namespace firmware {
  ColorSensor::ColorSensor(int pin): gpio(pin), running(0), color(utils::Color::NONE){}

  void ColorSensor::poll(){
    while(running){
      if(gpio.read()==1){
	color= utils::Color::RED;
      }
      else{
	color=utils::Color::GREEN;
      }
      usleep(5000);
    }
  }
  
  void ColorSensor::startPoll(){
    running = 1;
    color=utils::Color::NONE;
    gpio.dir(mraa::DIR_IN);
    gpio.mode(mraa::MODE_PULLUP);
    std::thread thr(&ColorSensor::poll, this);
    thr.detach();
  }

  void ColorSensor::stopPoll(){
    running=0;
  }
  
  utils::Color ColorSensor::getColor(){
    return color;
  }
}

