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
#include "../include/wallfollower.h"
#include "../include/dropoff.h"
#include "../include/pickup.h"
#include "../include/gyro_firmware.h"
#include "../include/drive_ctrl.h"
#include "../include/block.h"
#include "../include/map.h"
//#include "../include/angle_localizer.h"
#include "../include/robot.h" 

#define MS 1000
#define timeout 250

int running = 1;

void sig_handler(int signo) {
    if(signo == SIGINT) {
        printf("closing spi nicely\n");
        running = 0;
    }
}

int main(){
  signal(SIGINT, sig_handler);
  
  // Set up rangefinders 
    firmware::Rangefinder front(0, 6, 16);
    firmware::Rangefinder right(1, 8, 17);


    // Initialize motors
    firmware::Motor leftMotor(0, 1);
    firmware::Motor rightMotor(2, 3);
    
    // Set up gyro 
    rightMotor.setSpeed(0.0);
    leftMotor.setSpeed(0.0);
    usleep(500000);
    firmware::Gyro gyro(10);
    vision::Cam cam;
    gyro.startPoll();
    cam.startPoll();
    usleep(1000000);

    // Set up encoders
    firmware::Encoder leftEncoder(3, 2);
    firmware::Encoder rightEncoder(4,5);

    // Set up
    drive::DriveTrain driveCtrl(leftMotor, rightMotor, leftEncoder, rightEncoder, gyro, front);

    // Set up pickup 
    firmware::Motor pickupMotor = firmware::Motor(4,5);
    firmware::Servo sorter = firmware::Servo(10);
    firmware::LimitSwitch limitSwitch = firmware::LimitSwitch(0 );
    limitSwitch.startPoll();
    firmware::ColorSensor colorSensor = firmware::ColorSensor(1);
    control::Pickup pickup = control::Pickup(pickupMotor,sorter,limitSwitch, colorSensor);

    
    
    // Set up dropoff
    firmware::Servo rightFloor = firmware::Servo(7);
    firmware::Servo leftFloor = firmware::Servo(6);
    firmware::Servo leftOpener = firmware::Servo(9);
    firmware::Servo rightOpener = firmware::Servo(8);
    control::Dropoff leftStack(leftFloor, leftOpener, 1);
    control::Dropoff rightStack(rightFloor, rightOpener, -1); 
    
    control::Wallfollower wf(driveCtrl, front, right);
    
    std::cout << "limit switch ready" << std::endl;
    
    while(limitSwitch.getState() != 0) {
	usleep(100000);
    }
    wf.start();
    while(running){
      usleep(50000);
    }
    wf.stop();
  cam.stopPoll();
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
    gyro.stopPoll(); 
    limitSwitch.startPoll();

  return 0; 
}

/*
int main() {
    signal(SIGINT, sig_handler);
 
    // Set up rangefinders 
    firmware::Rangefinder front(0, 6, 16);
    firmware::Rangefinder right(1, 8, 17);


    // Initialize motors
    firmware::Motor leftMotor(0, 1);
    firmware::Motor rightMotor(2, 3);
    
    // Set up gyro 
    rightMotor.setSpeed(0.0);
    leftMotor.setSpeed(0.0);
    usleep(500000);
    firmware::Gyro gyro(10);
    vision::Cam cam;
    gyro.startPoll();
    cam.startPoll();
    usleep(1000000);

    // Set up encoders
    firmware::Encoder leftEncoder(3, 2);
    firmware::Encoder rightEncoder(4,5);

    // Set up
    drive::DriveTrain driveCtrl(leftMotor, rightMotor, leftEncoder, rightEncoder, gyro, front);

    // Set up pickup 
    firmware::Motor pickupMotor = firmware::Motor(4,5);
    firmware::Servo sorter = firmware::Servo(10);
    firmware::LimitSwitch limitSwitch = firmware::LimitSwitch(0 );
    limitSwitch.startPoll();
    firmware::ColorSensor colorSensor = firmware::ColorSensor(1);
    control::Pickup pickup = control::Pickup(pickupMotor,sorter,limitSwitch, colorSensor);

    
    
    // Set up dropoff
    firmware::Servo rightFloor = firmware::Servo(7);
    firmware::Servo leftFloor = firmware::Servo(6);
    firmware::Servo leftOpener = firmware::Servo(9);
    firmware::Servo rightOpener = firmware::Servo(8);
    control::Dropoff leftStack(leftFloor, leftOpener, 1);
    control::Dropoff rightStack(rightFloor, rightOpener, -1); 

    std::cout << "READY TO GO! HIT THE LIMIT SWITCH TO START ME." << std::endl;
    while(limitSwitch.getState() != 0) {
	usleep(100000);
    }
    
    utils::Color mode = utils::Color::RED;
    Robot robot(driveCtrl, pickup, leftStack, rightStack, cam, mode);
    struct timeval currentTime;
    double currentSeconds = 0.0; 
    while(currentSeconds < 150) { // After 150 seconds, drop stacks that we have 
	// update time
        gettimeofday(&currentTime, NULL);
        currentSeconds = ((double)currentTime.tv_sec);

	// If it sees a block, drive towards it and pick it up.
    	map::DriveInstruction instruction = cam.getBlockDirection();
	if(instruction.distance > 0) {
            robot.pickupStack(instruction);
	} 
            
    }

    cam.stopPoll();
    gyro.stopPoll(); 
    limitSwitch.startPoll();
  return 0;
}
*/
