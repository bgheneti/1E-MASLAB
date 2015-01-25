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
    firmware::Motor motorR(0, 1);
    firmware::Motor motorL(2, 3); 
    firmware::Motor pickupMotor(4, 5);

	pickupMotor.setSpeed(-.4);
/*
    // Encoder setup
    firmware::Encoder encoderL(2, 3);
    encoderL.startPolling();
    firmware::Encoder encoderR(4, 5);
    encoderR.startPolling();

    // Gyro setup
    firmware::Gyro gyro(10);
    gyro.startPoll();

    // Camera setup
    vision::Cam cam;
    cam.startPoll();

    // Wall follower setup
    drive::DriveTrain dt(motorL, motorR, encoderL, encoderR, gyro);
    control::Wallfollower wallfollower(dt, rangefinderfront, rangefinderleft, rangefinderright);
    // Main code
    wallfollower.start();
    while(running) {
	std::vector<vision::Block> blocks = cam.getBlocks();
	if(blocks.size() > 0) {
		wallfollower.stop();
		std::complex pos = blocks[0].getPosition();
		double heading = std::arg(pos);
		double distance = std::abs(pos);
		dt.turnForDegrees(heading);
		dt.straightForDistance(distance + .2);
		wallfollower.start();
	}
		
    	usleep(500000);
    }

    // Stop sensors
    encoderL.stopPolling();
    encoderR.stopPolling();
    gyro.stopPoll();
*/
}

