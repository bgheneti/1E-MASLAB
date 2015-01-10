#ifndef INCLUDE_GYRO_FIRMWARE_H_
#define INCLUDE_GYRO_FIRMWARE_H_

#include "mraa.hpp"
#include <thread>
namespace firmware{
    class Gyro{
        private:
            double angle;
	    double angularVelocity;
	    mraa::Spi* spi;
            mraa::Gpio* slaveSelect;
            int slaveSelectPin;
	    int running;
	    int zero;
	    std::thread runner;
        public:
	    //Creates a new gyro object that has a slave select pin  
	    Gyro(int slaveSelectPin);
	    
	    void poll();
	    
	    //start a thread for polling gyro data
	    void startPoll();
	    
	    //stops polling gyro data
	    void stopPoll();
	    
	    //returns the current angle in degrees
	    double getAngle();
	    
	    //returns the current angular velocity in degrees/second
	    double getAngularV();

	    //sets the current angle to zero
	    void zeroAngle();
    };
}
#endif
