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
void angleTest(map::AngleLocalizer al, int numReadings){
  std::cout << "angle" << al.getAngle(10) << std::endl;
  
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
    vision::Cam cam;
    gyro.startPoll();
    cam.startPoll();
    usleep(1000000);
    
    // Set up encoders
    firmware::Encoder leftEncoder(3, 2);
    firmware::Encoder rightEncoder(4,5);
    drive::DriveTrain driveCtrl(leftMotor, rightMotor, leftEncoder, rightEncoder, gyro);
    // Set up pickup 
    firmware::Motor pickupMotor = firmware::Motor(4,5);
    firmware::Servo sorter = firmware::Servo(10);
    firmware::LimitSwitch limitSwitch = firmware::LimitSwitch(0);
    firmware::ColorSensor colorSensor = firmware::ColorSensor(1);
    control::Pickup pickup = control::Pickup(pickupMotor,sorter,limitSwitch, colorSensor);
    
    bool found=false;
    int time=10000000;
    //look for blocks
    std::vector<vision::Block> blocks;
    
    while(time>0){
      std::cout<< "block"<<std::endl;

      blocks = cam.getBlocks();
      if(blocks.size()>0){
	found=true;
	std::cout<<"found block"<<std::endl;
        break;
      }
      usleep(100000);
      time-= 100000;

    }

    //if a block is found drive towards it and pick it up
    if(found==true){
      double degrees = blocks[0].getAngle();
      double distance = blocks[0].getDistance();

      std::cout<<"block: "<<degrees<<" degrees, "<<distance<<" meters, "<<"color: "<<blocks[0].getColor()<<std::endl;
      driveCtrl.turnForDegrees(degrees);
      if(distance>.3){
	driveCtrl.straightForDistance(distance-.2);
      }
      distance=.3;
      pickup.start();
      driveCtrl.straightForDistance(distance);
      usleep(10000000);
      pickup.stop();
     }

    pickup.start();
    usleep(30000000);
    pickup.stop();
    std::cout<<"done with pickup"<<std::endl;
    usleep(5000000);
    pickupMotor.setSpeed(0);
/*
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

    //pickup.start();
//    driveCtrl.straightForDistance(1.0);

    //usleep(30000000);
//    pickup.stop();
    //usleep(1000000);
 
 /*   rightStack.dropStack();
    usleep(5000000);
    rightStack.resetStack();
    usleep(1000000);
    leftStack.dropStack();
    usleep(5000000);
    leftStack.resetStack();
*/

    //cam.stopPoll();
    //gyro.stopPoll(); 
  return 0;
}
  


