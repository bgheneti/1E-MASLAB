#include <cassert>
#include <cmath>
#include <csignal>
#include <iostream>
#include <thread>
#include <chrono>
#include <complex>

#include "mraa.hpp"
#include "../include/motor_firmware.h"
#include "../include/encoder_firmware.h"
#include "../include/servo_firmware.h"
#include "../include/i2c_pwm_wrapper.h"
#include "../include/drive_ctrl.h"
//#include "../include/wallfollower.h"
//#include "../include/cam.h"
#include "../include/dropoff.h"
#include "../include/pickup.h"
#include "../include/arduino_firmware.h"
int running = 1;

void sig_handler(int signo) {
    if(signo == SIGINT) {
        printf("closing spi nicely\n");
        running = 0;
    }
}

int main() {
    signal(SIGINT, sig_handler);
//    firmware::Motor pickupMotor(4, 5);
//    firmware::Servo sorter(10);
//    control::Pickup pickup(pickupMotor, sorter);
//    pickup.start();

	firmware::Arduino arduino(6);
	arduino.startPoll();
	for(int i=0; i<10; i++) {
		std::cout << arduino.getBlockColor() << std::endl;
		usleep(1000000);
	}
	arduino.stopPoll();
}

