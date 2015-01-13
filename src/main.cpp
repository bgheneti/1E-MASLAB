#include <cassert>
#include <cmath>
#include <csignal>
#include <iostream>
#include <thread>
#include <chrono>

#include "mraa.hpp"
#include "../include/motor_firmware.h"
#include "../include/encoder_firmware.h"
#include "../include/servo_firmware.h"
#include "../include/i2c_pwm_wrapper.h"
#include "../include/drive_ctrl.h"

int running = 1;

void sig_handler(int signo) {
    if(signo == SIGINT) {
        printf("closing spi nicely\n");
        running = 0;
    }
}

int main() {
    signal(SIGINT, sig_handler);
    // Motor setup
    firmware::Motor motorR(9, 8);
    firmware::Motor motorL(3, 2);

    // Encoder setup
    firmware::Encoder encoderL(4, 5);
    encoderL.startPolling();
    firmware::Encoder encoderR(6, 1);
    encoderR.startPolling();

    // Gyro setup
    firmware::Gyro gyro(10);
    gyro.startPoll();
    drive::DriveTrain dt(&motorL, &motorR, &encoderL, &encoderR, &gyro);
    std::cout << "running" << std::endl;
    dt.straightForDistance(.5);
    std::cout << "running" << std::endl;

    encoderL.stopPolling();
    encoderR.stopPolling();
    gyro.stopPoll();
    /*
    double speed = 0.25;
    motorR.setSpeed(speed);
    motorL.setSpeed(speed);
    encoderL.startPolling();
    encoderR.startPolling();
    std::chrono::milliseconds sleep_time(10000);
    std::this_thread::sleep_for(sleep_time);
    encoderL.stopPolling();
    encoderR.stopPolling();
    printf("Left encoder reads: %f",  encoderL.getDistance());
    printf("Right encoder reads: %f",  encoderR.getDistance());
    motorR.stop();
    motorL.stop();
   */ 

    /*firmware::Servo servo = firmware::Servo(0);
    servo.setServoPosition(-90.0);
    std::chrono::milliseconds sleep_time(10000);
    std::this_thread::sleep_for(sleep_time);
    servo.setServoPosition(90.0);
    */
}

