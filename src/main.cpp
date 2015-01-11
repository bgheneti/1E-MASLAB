#include <cassert>
#include <cmath>
#include <csignal>
#include <iostream>
#include <thread>
#include <chrono>

#include "mraa.hpp"
#include "../include/motor_firmware.h"
#include "../include/encoder_firmware.h"

int running = 1;

void sig_handler(int signo) {
    if(signo == SIGINT) {
        printf("closing spi nicely\n");
        running = 0;
    }
}

int main() {
    signal(SIGINT, sig_handler);
    
    /*// Motor setup
    firmware::Motor motorR = firmware::Motor(9, 8);
    firmware::Motor motorL = firmware::Motor(3, 2);

    // Encoder setup
    firmware::Encoder encoderL = firmware::Encoder(4, 5);
    firmware::Encoder encoderR = firmware::Encoder(6, 7);

    double speed = 0.25;
    motorR.setSpeed(speed);
    motorL.setSpeed(speed);
    encoderL.startPolling();
    encoderR.startPolling();
    std::chrono::milliseconds sleep_time(10000);
    std::this_thread::sleep_for(sleep_time);
    encoderL.stopPolling();
    encoderR.stopPolling();
    printf("Left encoder reads: %d",  encoderL.getNumTicks());
    printf("Right encoder reads: %d",  encoderR.getNumTicks());
    motorR.stop();
    motorL.stop();
    */

    firmware::Servo servo = firmware::Servo(6, 0);
    servo.setServoPosition(90);
}

