s#ifndef INCLUDE_GYRO_FIRMWARE_H_
#define INCLUDE_GYRO_FIRMWARE_H_

#include "mraa.hpp"

namespace{
    class GYRO {
        private:
            double angle;
	    double angularVelocity;
        public:
	    //Creates a new gyro object that has a slave select pin  
	    Gyro(int slaveSelectPin);
	    
	    //start a thread for polling gyro data
	    void startPoll();
	    
	    //stops polling gyro data
	    void stopPoll();
	    
	    //returns the current angle in degrees
	    void getAngle();
	    
	    //returns the current angular velocity in degrees/second
	    double getAngularV();

	    //sets the current angle to zero
	    double zeroAngle();
    };
}
#endif
