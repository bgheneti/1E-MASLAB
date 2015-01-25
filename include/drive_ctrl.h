// This file contains code to control the drive train of the robot.

#ifndef INCLUDE_DRIVE_CTRL_H_
#define INCLUDE_DRIVE_CTRL_H_

#include "mraa.hpp"
#include "../include/encoder_firmware.h"
#include "../include/motor_firmware.h"
#include "../include/gyro_firmware.h"
#include <mutex>
#include <time.h>
#include <cmath>

#define SPEED .3
#define P (.0025 * 1.5)
#define I (0.0000015 * 1.5)
#define D (-0.05 *1.5)

namespace drive {
    
    class DriveTrain {
        private:
            firmware::Motor &leftMotor;
            firmware::Motor &rightMotor;
            firmware::Encoder &leftEncoder;
            firmware::Encoder &rightEncoder;
            firmware::Gyro &gyro;

            double bias;
            double power;
            bool driving;
            // Zero the encoders and gyro
            void resetSensors();
            void controlPID(double distance, double heading);


        public:
            // Constructor for drive, specifying the motors, encoders, and
            // gyroscope.
            DriveTrain(firmware::Motor &leftMotor, 
                  firmware::Motor &rightMotor,
                  firmware::Encoder &leftEncoder, 
                  firmware::Encoder &rightEncoder,
                  firmware::Gyro &gyro);

            // Move straight for some distance. If distance > 0, forward
            // otherwise backward.
            void straightForDistance(double distance);

            // Have the robot turn for some number of degrees. If degrees
            // is negative, turn left; if positive turn right.
            void turnForDegrees(double degrees);

    };


}

#endif