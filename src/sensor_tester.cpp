#include <iostream>
#include <csignal>
#include "mraa.hpp"
#include "../include/infrared_firmware.h"
#include "../include/ultrasonic_firmware.h"

int running = 1;

void sig_handler(int signo){
  if(signo == SIGINT){
    printf("closing spi nicely\n");
    running = 0;
  }
}

int main(){
  signal(SIGINT, sig_handler);
  
  firmware::InfraredSensor ir = firmware::InfraredSensor(0);
  firmware::UltrasonicSensor us = firmware::UltrasonicSensor(15, 16);
  
  sleep(1);
  while(running){
    std::cout << ir.getReading() << std::endl;
    std::cout << us.getDistance() << std::endl;
    std::cout << std::endl;
  }
}
