#ifndef INCLUDE_GYRO_FIRMWARE_H_
#define INCLUDE_GYRO_FIRMWARE_H_

#include "mraa.hpp"
#include <thread>
namespace firmware{
    class Gyro {
        private:
            double angle;
	    double angularVelocity;
	    mraa::Spi spi;
            mraa::Gpio slaveSelect;
            int slaveSelectPin;
<<<<<<< HEAD
	    int running;
	    std::thread runner;
            int zero;
=======
	    std::thread runner;
            int running;
	    int zero;
>>>>>>> 1c5985843c50e35f4db7cfa6f4e7c7032c8f8e03
	    const double newRateConstant;
	    void poll();
        public:
	    //Creates a new gyro object that has a slave select pin
	    Gyro(int slaveSelectPin);

	    //start a thread for polling gyro data
	    void startPoll();
<<<<<<< HEAD

=======
	    
>>>>>>> 1c5985843c50e35f4db7cfa6f4e7c7032c8f8e03
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

