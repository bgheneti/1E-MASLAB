#include <cassert>
#include <cmath>
#include <csignal>
#include <iostream>

#include "mraa.hpp"
#include "../include/motor_firmware.h"
int running = 1;

void sig_handler(int signo) {
    if(signo == SIGINT) {
        printf("closing spi nicely\n");
        running = 0;
    }
}

int main() {
    signal(SIGINT, sig_handler);
    
    // Motor 1 setup
    mraa::Pwm pwmL = mraa::Pwm(9);
    pwmL.write(0.0);
    pwmL.enable(true);

    mraa::Gpio dirL = mraa::Gpio(8);

    dirL.dir(mraa::DIR_OUT);
    dirL.write(0);

    // Motor 2 setup
    mraa::Pwm pwmR = mraa::Pwm(3);
    pwmR.write(0.0);
    pwmR.enable(true);

    mraa::Gpio dirR = mraa::Gpio(2);
    
    dirR.dir(mraa::DIR_OUT);
    dirR.write(0);

    double speed = 0.25;
    while(running) {
        std::cout << "Speed: " << speed << std::endl;
        setMotorSpeed(pwmL, dirL, speed);
        setMotorSpeed(pwmR, dirR, speed); 
        usleep(100000);
    }
    setMotorSpeed(pwmL, dirL, 0);
    setMotorSpeed(pwmR, dirR, 0);
}
