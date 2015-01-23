#include <cassert>
#include <cmath>
#include <csignal>
#include <iostream>
#include "mraa.hpp"
#include "../include/infrared_firmware.h"
#include "../include/ultrasonic_firmware.h"
#include "../include/rangefinder_firmware.h"

int running = 1;

void sig_handler(int signo){
  if (signo == SIGINT){
    printf("closing spi nicely\n");
    running = 0;
  }
}

int main(){
  signal(SIGINT, sig_handler);
  
  map::Localizer lc = map::Localizer();
  double hpDist = 0;
  while (running){
    hpDist = rf.getHighestProbDistance();
    std::cout << hpDist << "\t";
    std::cout << rf.getProbOfBlock(); << std::endl;
  }
}
