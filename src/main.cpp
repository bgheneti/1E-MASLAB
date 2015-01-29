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
#include "../include/rangefinder_firmware.h"
#include "../include/angle_localizer.h"

int running = 1;

void sig_handler(int signo) {
    if(signo == SIGINT) {
        printf("closing spi nicely\n");
        running = 0;
    }
}
void angleTest(map::AngleLocalizer al, int numReadings){
  std::cout << al.getAngle(10) << std::endl;
  
}
int main() {
    signal(SIGINT, sig_handler);
    // Motor setup

    firmware::Motor motorR(0, 1);
    firmware::Motor motorL(2, 3);
    firmware::Motor pickupMotor(4, 5);
//    pickupMotor.setSpeed(-.25);
//    usleep(5000000);
//    pickupMotor.stop();

/*    motorR.setSpeed(.2);
    motorL.setSpeed(.2);
    usleep(5000000);
    motorR.setSpeed(0.0);
    motorL.setSpeed(0.0);
*/

    
    // Encoder setup
    firmware::Encoder encoderL(2, 3);
    encoderL.startPolling();
    firmware::Encoder encoderR(5, 4);
    encoderR.startPolling();

    // Gyro setup
    firmware::Gyro gyro(10);
    gyro.startPoll();
    drive::DriveTrain dt(motorL, motorR, encoderL, encoderR, gyro);
    
    firmware::Rangefinder front(0, 6, 16);
    firmware::Rangefinder right(1, 8, 17);
    std::cout << "asdf" << std::endl;
    map::Map m("current_map.txt");
    std::cout << "fasd" << std::endl;
    //m.print();
    
    map::AngleLocalizer al(front, right, dt, m);
    
    utils::Point currLoc = m.getLocation();
    al.setLocation(currLoc.x, currLoc.y);
    
    std::thread thr = std::thread(angleTest, al, 10);
    while(running){
      usleep(10000);
    }
    thr.~thread();
    //usleep(500000);
    //std::cout << "running" << std::endl;
    //dt.straightForDistance(1.0);
 //   dt.turnForDegrees(50.0);
    //std::cout << "running" << std::endl;

    //encoderL.stopPolling();
    //encoderR.stopPolling();
    //gyro.stopPoll();

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

  /*  firmware::Servo servo = firmware::Servo(0);
    servo.setServoPosition(-40.0);
    std::chrono::milliseconds sleep_time(10000);
    std::this_thread::sleep_for(sleep_time);
    std::cout << "hi" << std::endl;
    servo.setServoPosition(90.0);
   */ 
   
}

