// This file contains functions to control the drive train of the
// robot.

namespace drive {

    DriveTrain::DriveTrain(firmware::Motor leftMotor, 
                           firmware::Motor rightMotor,
                           firmware::Encoder leftEncoder,
                           firmware::Encoder rightEncoder) {
        
        leftMotor = leftMotor;
        rightMotor = rightMotor;
        leftEncoder = leftEncoder;
        rightEncoder = rightEncoder;
        bias = 0;
        diff = 0;
        driving = false;
    }                    

    // Return requestedSpeed if it doesn't change the current speed by more
    // than .2. Otherwise change the speed by .2 in the correct direction.
    void DriveTrain::trySetMotorSpeed(double requestedSpeed, firmware::Motor motor) {
        if(fabs(requestedSpeed) > 1) {return;}
        double diff = requestedSpeed - motor.getSpeed();
    
        if(fabs(diff) < .2) {
            motor.setSpeed(requestedSpeed);
        } else {
            if(diff > 0) {
                motor.setSpeed(currentSpeed + .2);
            } else {
                motor.setSpeed(curentSpeed - .2);
            }
        }
    }

    // Maintain the bias and the diff in the left and right motors.
    // Don't let the speeds change abruptly.
    void DriveTrain::maintainSpeeds() {
        while(driving) {
            speedLock.lock();

            double requestedLeftSpeed = bias + diff;
            double requestedRightSpeed = bias - diff;

            DriveTrain::trySetMotorSpeed(requestedLeftSpeed, leftMotor);
            DriveTrain::trySetMotorSpeed(requestedRightSpeed, rightMotor);

            speedLock.unlock();
        }
    }

    // Get the robot to start moving straight
    void DriveTrain::straight() {

    }

    // Have the robot move straight until some point
    void DriveTrain::straightForDistance(double distance) {

    }
            
    // Get the robot to turn. If direction is negative, turn left;
    // if positive turn right.
    void DriveTrain::turn(int direction) {

    }

    // Have the robot turn for some number of degrees. If degrees
    // is negative, turn left; if positive turn right.
    void DriveTrain::turnForDegrees(double degrees) {

    }

    // Stop the robot, decelerating to avoid skidding.
    void stop() {

    }

                           



}
