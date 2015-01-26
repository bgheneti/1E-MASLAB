#include "../include/gyro_firmware.h"
#include <unistd.h>
#include <math.h>
#include <stdint.h>
#include <signal.h>
#include <thread>
#include <sys/time.h>

int MS=1000;
namespace firmware{

    Gyro::Gyro(int slaveSelectPinNumber): slaveSelectPin(slaveSelectPinNumber), slaveSelect(slaveSelectPinNumber), spi(0), zero(0), running(0), newRateConstant(15){}
    void Gyro::poll(){
        char rxBuf[2];
        char writeBuf[4];
        unsigned int sensorRead = 0x20000000;
        writeBuf[0] = sensorRead & 0xff;
        writeBuf[1] = (sensorRead >> 8) & 0xff;
        writeBuf[2] = (sensorRead >> 16) & 0xff;
        writeBuf[3] = (sensorRead >> 24) & 0xff;
        struct timeval tv;
        int init = 0;
	int biasCalcRuns=200;
	int totalBiasCalcRuns=biasCalcRuns;
	double bias=0;
	
	double p=0; 
	double q=.001;
	double r=0.0006;
	double k;
	angle=0;
	while(running){
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
	    usleep(2*MS);
	}
    }

    void Gyro::startPoll(){
	if(running==0){
	    running=1;
	    slaveSelect.dir(mraa::DIR_OUT);
            slaveSelect.write(1);
            spi.bitPerWord(32);
	    std::thread thr(&Gyro::poll, this);
	    std::swap(thr, runner);
	    runner.detach();
        }
    }

    double Gyro::getAngle(){
	return angle;
    }

    double Gyro::getAngularV(){
	return angularVelocity;
    }

    void Gyro::stopPoll(){
	running=0;
    }

    void Gyro::zeroAngle(){
	zero=1;
    }
}
