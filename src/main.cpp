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
<<<<<<< HEAD
=======
#include "../include/drive_ctrl.h"
>>>>>>> 1c5985843c50e35f4db7cfa6f4e7c7032c8f8e03

int running = 1;

void sig_handler(int signo) {
    if(signo == SIGINT) {
        printf("closing spi nicely\n");
        running = 0;
    }
}

int main() {
    signal(SIGINT, sig_handler);
<<<<<<< HEAD
    /*// Motor setup
    firmware::Motor motorR = firmware::Motor(9, 8);
    firmware::Motor motorL = firmware::Motor(3, 2);
=======
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
>>>>>>> 1c5985843c50e35f4db7cfa6f4e7c7032c8f8e03

    
    // Encoder setup
    firmware::Encoder encoderL(2, 3);
    encoderL.startPolling();
    firmware::Encoder encoderR(4, 5);
    encoderR.startPolling();

    // Gyro setup
    firmware::Gyro gyro(10);
    gyro.startPoll();
    drive::DriveTrain dt(motorL, motorR, encoderL, encoderR, gyro);
    usleep(500000);
    std::cout << "running" << std::endl;
    dt.straightForDistance(.2);
    dt.turnForDegrees(50.0);
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
<<<<<<< HEAD
    encoderL.stopPolling();
    encoderR.stopPolling();
    printf("Left encoder reads: %d",  encoderL.getNumTicks());
    printf("Right encoder reads: %d",  encoderR.getNumTicks());
    motorR.stop();
    motorL.stop();
    */

    firmware::Servo servo = firmware::Servo(0);
    servo.setServoPosition(-90.0);
    std::chrono::milliseconds sleep_time(10000);
    std::this_thread::sleep_for(sleep_time);
    servo.setServoPosition(90.0);
=======
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
   
>>>>>>> 1c5985843c50e35f4db7cfa6f4e7c7032c8f8e03
}

