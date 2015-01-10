// This file contains code to control the drive train of the robot.

#ifndef INCLUDE_DRIVE_CTRL_H_
#define INCLUDE_DRIVE_CTRL_H_

#include "mraa.hpp"
#include "encoder_firmware.h"
#include "motor_firmware.h"
#include "point.h"
namespace drive {
    
    class DriveTrain {
        private:
            firmware::Motor leftMotor;
            firmware::Motor rightMotor;
            firmware::Encoder leftEncoder;
            firmware::Encoder rightEncoder;
            double bias;

            // Sets the motor speeds ensuring no abrupt starts or stops
            void setMotorSpeeds(leftSpeed, rightSpeed);

            // Get the robot to start moving straight
            void straight();
            
            // Get the robot to turn. If direction is negative, turn left;
            // if positive turn right.
            void turn(int direction);

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
