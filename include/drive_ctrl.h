// This file contains code to control the drive train of the robot.

#ifndef INCLUDE_DRIVE_CTRL_H_
#define INCLUDE_DRIVE_CTRL_H_

#include "mraa.hpp"
#include "../include/encoder_firmware.h"
#include "../include/motor_firmware.h"
#include "../include/gyro_firmware.h"
#include "point.h"
#include <mutex>
#include <ctime>

#define SPEED .5
#define P 0
#define I 0
#define D 0

namespace drive {
    
    class DriveTrain {
        private:
            firmware::Motor leftMotor;
            firmware::Motor rightMotor;
            firmware::Encoder leftEncoder;
            firmware::Encoder rightEncoder;
            firmware::Gyro gyro;
            std::mutex speedLock;
            double bias;  // The speed the robot should move forward at.
            double diff;  // The difference between the motors and the bias
            bool driving;  // Indicates whether the drive module should be
                           // active, ie, should be maintaining bias and diffs.
            // Set the speed to the requested speed if it is not different
            // from the current speed by more than .2. Otherwise change the
            // speed by .2 in the correct direction.
            double getNewSpeed(double requestedSpeed, double currentSpeed);

            // Maintain the bias and the diff in the left and right motors.
            // Don't let the speeds change abruptly.
            void maintainSpeeds();

            // Zero the encoders and gyro
            void resetSensors();

            // Move forward if direction is positive, backward if direction
            // is negative
            void straight(int direction);
            
            // Get the robot to turn. If direction is negative, turn left;
            // if positive turn right.
            void turn(int direction);

            // Stop the robot, decelerating to avoid skidding.
            void stop();

        public:
            // Constructor for drive, specifying the motors, encoders, and
            // gyroscope.
            Drive(firmware::Motor leftMotor, 
                  firmware::Motor rightMotor,
                  firmware::Encoder leftEncoder, 
                  firmware::Encoder rightEncoder,
                  firmware::Gyro gyro);

            // Move straight for some distance. If distance > 0, forward
            // otherwise backward.
            void straightForDistance(double distance)

            // Have the robot turn for some number of degrees. If degrees
            // is negative, turn left; if positive turn right.
            void turnForDegrees(double degrees);

    }


}

#endif
