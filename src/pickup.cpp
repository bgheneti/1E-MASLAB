#include "../include/pickup.h"
#include "../include/color.h"
#include <iostream>
#define SORT_CENTER 5.0
#define SORT_DELTA 72.0
namespace control {
	Pickup::Pickup(firmware::Motor& pickupMotor,
		       firmware::Servo& sorter, firmware::LimitSwitch& limitSwitch, firmware::ColorSensor& colorSensor) : pickupMotor(pickupMotor), sorter(sorter), limitSwitch(limitSwitch), colorSensor(colorSensor), numRedBlocks(0), numGreenBlocks(0){}

        void Pickup::start(){
	  std::thread thr(&Pickup::run, this);
	  std::swap(thr, runner);

	} 

        void Pickup::run(){
	  std::cout<<"run"<<std::endl;
	  limitSwitch.startPoll();
	  sorter.setServoPosition(SORT_CENTER);
	  running=1;
	  pickupMotor.setSpeed(-0.4);
	  while(running){
	    if(limitSwitch.getState()==0){
	      colorSensor.startPoll();
	      numBlocks+=1;
	      pickupMotor.setSpeed(0);
	      usleep(800000);
	      std::cout<<"color:"<<colorSensor.getColor()<<std::endl;
	      while(colorSensor.getColor()==utils::Color::NONE){
		usleep(10000);
	      }
	      if(colorSensor.getColor()==utils::Color::RED){
		for(int delta=0;delta<SORT_DELTA+1;delta++){
		  sorter.setServoPosition(SORT_CENTER + delta);
		  usleep(10000);
		}
		numRedBlocks+=1;
	      }
	      else if(colorSensor.getColor()==utils::Color::GREEN){
		for(int delta=0;delta<SORT_DELTA+1;delta++){
                  sorter.setServoPosition(SORT_CENTER - delta);
                  usleep(10000);
                }
		numGreenBlocks+=1;
	      }
	      usleep(3000000);
	      sorter.setServoPosition(SORT_CENTER);
	      usleep(500000);
	      pickupMotor.setSpeed(-0.4);
	      colorSensor.stopPoll();
	    }
	    usleep(10000);
	  }
	  usleep(50000);
	  pickupMotor.setSpeed(0.0);
	  limitSwitch.stopPoll();
	  colorSensor.stopPoll();
	}

        void Pickup::stop() {
	  running=0;
	  runner.join();
        }

        int Pickup::numRedBlocksPickedUp(){
	  return numRedBlocks;
	}

        int Pickup::numGreenBlocksPickedUp(){
	  return numGreenBlocks;
	}

        void Pickup::releaseStack(utils::Color stackColor){
	  if(stackColor==utils::Color::RED){
	    numRedBlocks=0;
	  }
	  else if(stackColor==utils::Color::GREEN){
	    numGreenBlocks=0;
	  }
	}
}
