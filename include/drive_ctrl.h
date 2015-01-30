// This file contains code to control the drive train of the robot.

#ifndef INCLUDE_DRIVE_CTRL_H_
#define INCLUDE_DRIVE_CTRL_H_

#include "mraa.hpp"
#include "../include/encoder_firmware.h"
#include "../include/motor_firmware.h"
#include "../include/gyro_firmware.h"
#include "../include/rangefinder_firmware.h"
#include <mutex>
#include <time.h>
#include <cmath>

 namespace drive {
    
    class DriveTrain {
        private:
            firmware::Motor &leftMotor;
            firmware::Motor &rightMotor;
            firmware::Encoder &leftEncoder;
            firmware::Encoder &rightEncoder;
            firmware::Gyro &gyro;
            firmware::Rangefinder &front;
            
            double angleOffset;
            double bias;
            double power;
            bool driving;
            // Zero the encoders and gyro
            void resetSensors();
            double controlPID(double distance, double heading, double tolerance, double wallAllowance);


        public:
            // Constructor for drive, specifying the motors, encoders, and
            // gyroscope.
            DriveTrain(firmware::Motor &leftMotor, 
                  firmware::Motor &rightMotor,
                  firmware::Encoder &leftEncoder, 
                  firmware::Encoder &rightEncoder,
                  firmware::Gyro &gyro,
		  firmware::Rangefinder &front);

            // Move straight for some distance. If distance > 0, forward
            // otherwise backward.
            double straightForDistance(double distance, double wallAllowance);

            // Have the robot turn for some number of degrees. If degrees
            // is negative, turn left; if positive turn right.
            double turnForDegrees(double degrees, double tolerance);

    };


}

#endif
