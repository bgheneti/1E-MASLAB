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
    /*
    firmware::Gyro gyro(10);
	gyro.startPoll();
	
	//vision::Cam cam;
	//cam.startPoll();
	//usleep(1000000);
	for(int i=0; i<10; i++) {
		std::cout << "gyro angle " << gyro.getAngle() << std::endl; 
		usleep(1000000);
	}
	
    */
    firmware::Motor leftMotor(2,3);
    firmware::Motor rightMotor(0,1);
    leftMotor.setSpeed(0.5);
    rightMotor.setSpeed(0.5);
    usleep(6000000);
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
    /*
    firmware::Encoder leftEncoder(2,3);
    firmware::Encoder rightEncoder(5,4);
    drive::DriveTrain driveCtrl(leftMotor,rightMotor,leftEncoder,rightEncoder,gyro);
    driveCtrl.straightForDistance(1.0);
   
    //bool found=false;
    //int time=10 * timeout * MS;
    //std::vector<vision::Block> blocks;
    */
    /*
  firmware::Motor pickupMotor = firmware::Motor(4,5);
  firmware::Servo sorter = firmware::Servo(10);
  firmware::LimitSwitch limitSwitch = firmware::LimitSwitch(0);
  firmware::ColorSensor colorSensor = firmware::ColorSensor(1);
  control::Pickup pickup = control::Pickup(pickupMotor,sorter,limitSwitch, colorSensor);
  pickup.start();
  usleep(60000000);
  pickup.stop();
  pickupMotor.setSpeed(0);
  
  firmware::Servo rightFloor = firmware::Servo(6);
  firmware::Servo leftFloor = firmware::Servo(7);
  firmware::Servo leftOpener = firmware::Servo(9);
  firmware::Servo rightOpener = firmware::Servo(8);
  control::Dropoff leftStack(leftFloor, leftOpener, 1);
  control::Dropoff rightStack(rightFloor, rightOpener, -1);
  rightOpener.off();
  leftOpener.off();
  rightStack.resetStack();
  leftStack.resetStack();
  usleep(10000000);
  rightStack.dropStack();
  usleep(5000000);
  rightStack.resetStack();
  usleep(1000000);
  leftStack.dropStack();
  usleep(5000000);
  leftFloor.off();
  rightFloor.off();
  */
    /*
  while(time>0){
    std::cout<< "block"<<std::endl;

    blocks = cam.getBlocks();
    if(blocks.size()>0){
      found=true;
      std::cout<<"found block"<<std::endl;
      break;
    }
    usleep(timeout * MS);
    time-= timeout * MS;

  }
  std::cout<<"DONE WITH WHILE"<<std::endl;
  //gyro.stopPoll();                                                                                  
  cam.stopPoll();

  if(found==true){
    double degrees = blocks[0].getAngle();
    double distance = blocks[0].getDistance();

    std::cout<<"block: "<<degrees<<" degrees, "<<distance<<" meters, "<<"color: "<<blocks[0].getColor;
      ()<<std::endl;
    driveCtrl.turnForDegrees(degrees);                                                              
    driveCtrl.straightForDistance(distance);                                                        
    pickup.start();
    usleep(20000000);
    pickup.stop();
    usleep(5000000);

  }
    */
  std::cout<<"end"<<std::endl;
  
}

