#include <cassert>
#include <cmath>
#include <csignal>
#include <iostream>

#include "mraa.hpp"
#include "include/motor_firmware.h"
int running = 1;

void sig_handler(int signo) {
    if(signo == SIGINT) {
        printf("closing spi nicely\n");
        running = 0;
    }
}

int main() {
    signal(SIGINT, sig_handler);
    
    mraa::Pwm pwm = mraa::Pwm(9);
    pwm.write(0.0);
    pwm.enable(true);

    mraa::Gpio dir = mraa::Gpio(8);

    dir.dir(mraa::DIR_OUT);
    dir.write(0);

    double speed = -1.0;
    while(running) {
        std::cout << "Speed: " << speed << std::endl;
        setMotorSpeed(pwm, dir, speed);
    
        speed += .1;
        if(speed > 1.0) {
            speed = -1.0;
            setMotorSpeed(pwm, dir, 0.0);
            sleep(2.0);
        }
        usleep(100000);
    }
}
