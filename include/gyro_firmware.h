#ifndef INCLUDE_GYRO_FIRMWARE_H_
#define INCLUDE_GYRO_FIRMWARE_H_

#include "mraa.hpp"

namespace{
    class GYRO {
        private:
            double angle;
	    double angularVelocity;
        public:
	    Gyro(int dirPin);
	    
	    void startPoll();

	    void stopPoll();
	    
	    void getAngle();

	    double getAngularV();

	    double zeroAngle();
    };
}
