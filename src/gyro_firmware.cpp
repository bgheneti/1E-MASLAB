#include "../include/gyro_firmware.h"
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <sys/time.h>
#include <thread>
#include <assert.h>
namespace firmware{
    
  Gyro::Gyro(int slaveSelectPinNumber): slaveSelectPin(slaveSelectPinNumber), slaveSelect(slaveSelectPin), spi(0) {zero=0;}
    
    void Gyro::poll(){
        printf("polling");
        char rxBuf[2];
        char writeBuf[4];
        unsigned int sensorRead = 0x20000000;
        writeBuf[0] = sensorRead & 0xff;
        writeBuf[1] = (sensorRead >> 8) & 0xff;
        writeBuf[2] = (sensorRead >> 16) & 0xff;
        writeBuf[3] = (sensorRead >> 24) & 0xff;
        struct timeval tv;
        int init = 0;
	while(running){
	    if(zero){
		angle=0;
		zero=0;
	    }
	    slaveSelect.write(0);
            printf("before");
	    char * recv={spi.write(writeBuf, 4)};
	    slaveSelect.write(1);
	    if (recv != NULL) {
	        unsigned int recvVal = 0 | *(recv+3);
	        recvVal = (recvVal << 8) | *(recv+2);
	        recvVal = (recvVal << 8) | *(recv+1);
	        recvVal = (recvVal << 8) | *recv;
		// Sensor reading
		short reading = (recvVal >> 10) & 0xffff;
		if (init) {
		    unsigned long long ms = (unsigned long long)(tv.tv_sec)*1000 +
			(unsigned long long)(tv.tv_usec) / 1000;
		    gettimeofday(&tv, NULL);
		    ms -= (unsigned long long)(tv.tv_sec)*1000 +
			(unsigned long long)(tv.tv_usec) / 1000;
		    int msi = (int)ms;
		    float msf = (float)msi;
		    angularVelocity = (double)reading;
		    angle += -0.001 * msf * (angularVelocity / 80.0);
		}
		else {
		    init = 1;
		    gettimeofday(&tv, NULL);
		}
	    }
	    else {
		printf("No recv\n");
	    }
	    usleep(5000);
	}
    }
    
    void Gyro::startPoll(){
      printf("dir: %d",mraa::DIR_IN);
      fflush(stdout);
      //slaveSelect.dir(mraa::DIR_OUT);
      mraa::Gpio * slave = new mraa::Gpio(3);
      slave->dir(mraa::DIR_IN);
      delete(slave);
	//slaveSelect.write(1);
        //spi.bitPerWord(32);
	//	std::thread thr(&Gyro::poll, this);
	//	std::swap(thr, runner);
	//poll();
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