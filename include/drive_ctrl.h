// This file contains code to control the drive train of the robot.

#ifndef INCLUDE_DRIVE_CTRL_H_
#define INCLUDE_DRIVE_CTRL_H_

#include "mraa.hpp"
#include "encoder_firmware.h"
#include "motor_firmware.h"
#include "point.h"
#include <mutex>

namespace drive {
    
    class DriveTrain {
        private:
            firmware::Motor leftMotor;
            firmware::Motor rightMotor;
            firmware::Encoder leftEncoder;
            firmware::Encoder rightEncoder;
            std::mutex speedLock;
            double bias;  // The speed the robot should move forward at.
            double diff;  // The difference between the motors and the bias
            bool driving;  // Indicates whether the drive module should be
                           // active, ie, should be maintaining bias and diffs.

            // Return requestedSpeed if it doesn't change the current speed by
            // more than .2. Otherwise change the speed by .2 in the correct 
            // direction.
            double getNewSpeed(double requestedSpeed, double currentSpeed);

            // Maintain the bias and the diff in the left and right motors.
            // Don't let the speeds change abruptly.
            void maintainSpeeds();

            // Get the robot to start moving straight
            void straight();

            // Have the robot move straight until some point
            void straightForDistance(double distance);
            
            // Get the robot to turn. If direction is negative, turn left;
            // if positive turn right.
            void turn(int direction);

            // Have the robot turn for some number of degrees. If degrees
            // is negative, turn left; if positive turn right.
            void turnForDegrees(double degrees);

            // Stop the robot, decelerating to avoid skidding.
            void stop();

        public:
            // Constructor for drive, specifying the motors, encoders, and
            // initial orientation.
            Drive(firmware::Motor leftMotor, firmware::Motor rightMotor,
                  firmware::Encoder leftEncoder, firmware::Encoder rightEncoder);

            // Go to nextPosition from currentPosition given the current heading
            void goToPoint(utils::Point currentPosition, float currentHeading,
                           utils::Point nextPosition);

            // Turn to a specified heading
            void goToHeading(float currentHeading, float nextHeading);
            
    }


}

#endif
