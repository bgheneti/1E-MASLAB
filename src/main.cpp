#include <cassert>
#include <cmath>
#include <csignal>
#include <iostream>
#include <thread>
#include <chrono>
#include <complex>
#include "../include/cam_vision.h"
#include "mraa.hpp"
#include "../include/motor_firmware.h"
#include "../include/encoder_firmware.h"
#include "../include/servo_firmware.h"
#include "../include/limit_switch_firmware.h"
#include "../include/i2c_pwm_wrapper.h"
#include "../include/drive_ctrl.h"
//#include "../include/wallfollower.h"
#include "../include/dropoff.h"
#include "../include/pickup.h"
#include "../include/gyro_firmware.h"
#include "../include/drive_ctrl.h"
#include "../include/block.h"

#define MS 1000
#define timeout 250

int running = 1;

void sig_handler(int signo) {
    if(signo == SIGINT) {
        printf("closing spi nicely\n");
        running = 0;
    }
}

int main() {
    signal(SIGINT, sig_handler);
 
    // Initialize motors
    firmware::Motor leftMotor(0, 1);
    firmware::Motor rightMotor(2, 3);
   
    // Set up gyro 
    rightMotor.setSpeed(0.0);
    leftMotor.setSpeed(0.0);
    usleep(500000);
    firmware::Gyro gyro(10);
    gyro.startPoll();
    usleep(500000);
   
    // Set up encoders
    mraa::Gpio leftEncoderPin1(3);
    mraa::Gpio leftEncoderPin2(2); 
    firmware::Encoder leftEncoder(leftEncoderPin1, leftEncoderPin2);
    //firmware::Encoder rightEncoder(4,5);
    leftEncoder.startPolling();
    //rightEncoder.startPolling();
    while(running) {
      std::cout << gyro.getAngle() << ' ';
      std::cout << leftEncoder.getDistance()<<std::endl;
      usleep(300000);
    }
    /*
    // Set up pickup 
    firmware::Motor pickupMotor = firmware::Motor(4,5);
    firmware::Servo sorter = firmware::Servo(10);
    firmware::LimitSwitch limitSwitch = firmware::LimitSwitch(0);
    firmware::ColorSensor colorSensor = firmware::ColorSensor(1);
    control::Pickup pickup = control::Pickup(pickupMotor,sorter,limitSwitch, colorSensor);

    // Set up dropoff
    firmware::Servo rightFloor = firmware::Servo(7);
    firmware::Servo leftFloor = firmware::Servo(6);
    firmware::Servo leftOpener = firmware::Servo(9);
    firmware::Servo rightOpener = firmware::Servo(8);
    leftOpener.off();
    rightOpener.off(); 
    control::Dropoff leftStack(leftFloor, leftOpener, 1);
    control::Dropoff rightStack(rightFloor, rightOpener, -1); 
    rightStack.resetStack();
    leftStack.resetStack();
    */
//    driveCtrl.turnForDegrees(60.0);

/*    pickup.start();
    driveCtrl.straightForDistance(0.3);
    usleep(10000000);
    pickup.stop();
*/ 
 /*   rightStack.dropStack();
    usleep(5000000);
    rightStack.resetStack();
    usleep(1000000);
    leftStack.dropStack();
    usleep(5000000);
    leftStack.resetStack();
*/


    leftEncoder.stopPolling();
    //rightEncoder.stopPolling(); 
  gyro.stopPoll(); 
  return 0;
}
  


