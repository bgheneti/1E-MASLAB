#include <cassert>
#include <cmath>
#include <csignal>
#include <iostream>
#include <thread>
#include <chrono>

#include "mraa.hpp"
#include "../include/wallfollowing.h"

int running = 1;

void sig_handler(int signo) {
    if(signo == SIGINT) {
        printf("closing spi nicely\n");
        running = 0;
    }
}

int main() {
    signal(SIGINT, sig_handler);
    
    firmware::Encoder encoderL(2, 3);
    firmware::Encoder encoderR(5, 4);
    
    firmware::Motor motorL(2, 3);
    firmware::Motor motorR(0, 1);
    
    firmware::Gyro gyro(10);
    
    drive::DriveTrain driveTrain(motorL, motorR, encoderL, encoderR, gyro);
    
    firmware::Rangefinder front(-1, -1, -1);
    firmware::Rangefinder left(0, 8, 16);
    firmware::Rangefinder right(1, 15, 17);
    
    control::Wallfollower wf = control::Wallfollower(driveTrain, front, left, right);
}

