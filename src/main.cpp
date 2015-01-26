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
#include "../include/i2c_pwm_wrapper.h"
#include "../include/drive_ctrl.h"
//#include "../include/wallfollower.h"
//#include "../include/cam.h"
#include "../include/dropoff.h"
#include "../include/pickup.h"
#include "../include/arduino_firmware.h"
#include "../include/gyro_firmware.h"
#include "../include/drive_ctrl.h"
#include "../include/block.h"
#include "../include/spi_wrapper.h"

#define MS 1000
#define timeout 50

int running = 1;

void sig_handler(int signo) {
    if(signo == SIGINT) {
        printf("closing spi nicely\n");
        running = 0;
    }
}

int main() {
    signal(SIGINT, sig_handler);
	firmware::Arduino arduino(9);
    	firmware::Gyro gyro(10);
	firmware::SpiWrapper spiWrapper(gyro, arduino);
	spiWrapper.startPoll();
	usleep(1000000);
	for(int i=0; i<10; i++) {
		std::cout << "block color " << arduino.getBlockColor() << std::endl;
		std::cout << "gyro angle " << gyro.getAngle() << std::endl; 
		usleep(1000000);
	}
/*
    firmware::Motor leftMotor(2,3);
    firmware::Motor rightMotor(0,1);
    firmware::Encoder leftEncoder(2,3);
    firmware::Encoder rightEncoder(4,5);
    drive::DriveTrain driveCtrl(leftMotor,rightMotor,leftEncoder,rightEncoder,gyro);
    vision::Cam cam;

    cam.startPoll();
    bool found=false;
    int time=100 * timeout * MS;
    std::vector<vision::Block> blocks;

  firmware::Servo rightFloor = firmware::Servo(7);
  firmware::Servo leftFloor = firmware::Servo(6);
  firmware::Servo leftOpener = firmware::Servo(8);
  firmware::Servo rightOpener = firmware::Servo(9);
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
  //leftFloor.off();
  //rightFloor.off();

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

  if(found==true){
    std::complex<double> blockPosition = (blocks[0]).getPosition();

    double degrees = -gyro.getAngle()+180 * std::arg(blockPosition);
    double distance = std::abs(blockPosition);

    std::cout<<"block: "<<degrees<<" degrees, "<<distance<<" meters";
    driveCtrl.turnForDegrees(degrees);                                                                   
    driveCtrl.straightForDistance(distance);                                                          
  }
*/
  spiWrapper.stopPoll();
}

