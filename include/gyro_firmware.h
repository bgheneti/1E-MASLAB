#ifndef INCLUDE_GYRO_FIRMWARE_H_
#define INCLUDE_GYRO_FIRMWARE_H_

#include "mraa.hpp"
#include <thread>
#include <sys/time.h>

namespace firmware{
    class Gyro {
        private:
            double angle;
	    double angularVelocity;
	    mraa::Spi spi;
            mraa::Gpio slaveSelect;
            int slaveSelectPin;
	    std::thread runner;
            int running;
	    int zero;
	    int init;
	    double p;
	    //double bias;
	    double biasCalcRuns;
	    const double newRateConstant;
	    struct timeval tv;
        public:
	    double deltaTime;
	    //Creates a new gyro object that has a slave select pin
	    Gyro(int slaveSelectPin);

	    void poll(mraa::Spi& spi);
	    double bias;
	    //returns the current angle in degrees
	    double getAngle();

	    //returns the current angular velocity in degrees/second
	    double getAngularV();

	    //sets the current angle to zero
	    void zeroAngle();
    };
}
#endif

