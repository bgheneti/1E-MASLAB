#include "../include/gyro_firmware.h"
#include <unistd.h>
#include <math.h>
#include <stdint.h>
#include <signal.h>
#include <thread>

#define r .0006
#define q .001
#define totalBiasCalcRuns 200
int MS=1000;
namespace firmware{

  Gyro::Gyro(int slaveSelectPinNumber): slaveSelectPin(slaveSelectPinNumber), slaveSelect(slaveSelectPinNumber), spi(0), zero(0), running(0), newRateConstant(15), p(0.0), angle(0.0), bias(0.0), biasCalcRuns(totalBiasCalcRuns), init(0), deltaTime(0) { 
	slaveSelect.dir(mraa::DIR_OUT);
	slaveSelect.write(1);
        
	}

    void Gyro::poll(mraa::Spi& spi){
	char writeBuf[4];
        unsigned int sensorRead = 0x20000000;
        writeBuf[0] = sensorRead & 0xff;
        writeBuf[1] = (sensorRead >> 8) & 0xff;
        writeBuf[2] = (sensorRead >> 16) & 0xff;
        writeBuf[3] = (sensorRead >> 24) & 0xff;
	    double k;
	    if(zero){
		angle=0;
		zero=0;
	    }
	    slaveSelect.write(0);
            char* recv = spi.write(writeBuf, 4);
            slaveSelect.write(1);
	    if (recv != NULL) {
	        unsigned int recvVal = ((uint8_t) recv[3] & 0xFF);
	        recvVal = (recvVal << 8) | ((uint8_t)recv[2] & 0xFF);
	        recvVal = (recvVal << 8) | ((uint8_t)recv[1] & 0xFF);
	        recvVal = (recvVal << 8) | ((uint8_t)recv[0] & 0xFF);  
	        // Sensor reading
		short reading = (recvVal >> 10) & 0xffff;
		if (init) {
		    unsigned long long ms = (unsigned long long)(tv.tv_sec) * 1000 + 
		      (unsigned long long)(tv.tv_usec) / 1000;
		    gettimeofday(&tv, NULL);
		    ms -= (unsigned long long)(tv.tv_sec) * 1000 +
		      (unsigned long long)(tv.tv_usec) / 1000;
		    int msi = (int)ms;
		    float msf = (float)msi;
		    deltaTime=-msf;

		    float rf = (float) reading;
		    if(biasCalcRuns>0){
		        bias += -0.001 * msf * ((rf / 80.0) / totalBiasCalcRuns);
		        biasCalcRuns--;
		    }
		    else{
		      double newAngularVelocity= -0.001 * msf * (rf / 80.0) - bias;
		      //printf("new: %f \n", newAngularVelocity);
		      p = p + q;
		      k = p / (p + r);
		      angularVelocity += k * (newAngularVelocity - angularVelocity);
			//angularVelocity = (-msf/newRateConstant)*(newAngularVelocity)+
			//(1+msf/newRateConstant)*angularVelocity;
		      p = (1 - k) * p;
		      angle += angularVelocity;
		      angle = fmod(angle,360);		      
		      if(angle > 180){
			angle -= 360;
		      }
		      else if(angle < -180){
			angle += 360;
		      }
		    }
		}
		else {
		    init = 1;
		    gettimeofday(&tv, NULL);
		}
	    }
	    else {
		printf("No recv\n");
	    }
    }

    double Gyro::getAngle(){
	return angle;
    }

    double Gyro::getAngularV(){
	return angularVelocity;
    }


    void Gyro::zeroAngle(){
	zero=1;
    }
}

