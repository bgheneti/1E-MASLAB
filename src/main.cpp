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
#include "../include/dropoff.h"

int running = 1;

void sig_handler(int signo) {
    if(signo == SIGINT) {
        printf("closing spi nicely\n");
        running = 0;
    }
}

int main() {
    signal(SIGINT, sig_handler);
  firmware::Servo rightFloor = firmware::Servo(7);
  firmware::Servo leftFloor = firmware::Servo(6);
  firmware::Servo leftOpener = firmware::Servo(8);
  firmware::Servo rightOpener = firmware::Servo(9);
  control::Dropoff leftStack(leftFloor, leftOpener, 1);
  control::Dropoff rightStack(rightFloor, rightOpener, -1);
  rightStack.dropStack();
  leftStack.dropStack();
  leftFloor.off();
  rightFloor.off();
}

