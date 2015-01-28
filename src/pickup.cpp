#include "../include/pickup.h"
#include "../include/color.h"
#include <iostream>
#define SORT_CENTER 6.0
#define SORT_DELTA 70.0
namespace control {
	Pickup::Pickup(firmware::Motor& pickupMotor,
		       firmware::Servo& sorter, firmware::LimitSwitch& limitSwitch, firmware::ColorSensor& colorSensor) : pickupMotor(pickupMotor), sorter(sorter), limitSwitch(limitSwitch), colorSensor(colorSensor), numRedBlocks(0), numGreenBlocks(0){}

        void Pickup::start(){
	  std::thread thr(&Pickup::run, this);
	  thr.detach();
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
	      usleep(400000);
	      std::cout<<"color:"<<colorSensor.getColor()<<std::endl;
	      while(colorSensor.getColor()==utils::Color::NONE){
		usleep(10000);
	      }
	      if(colorSensor.getColor()==utils::Color::RED){
		sorter.setServoPosition(SORT_CENTER + SORT_DELTA);
		numRedBlocks+=1;
	      }
	      else if(colorSensor.getColor()==utils::Color::GREEN){
		sorter.setServoPosition(SORT_CENTER - SORT_DELTA);
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
	  pickupMotor.setSpeed(0);
	  limitSwitch.stopPoll();
	  colorSensor.stopPoll();
	}

        void Pickup::stop() {
	  running=0;
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
	  else{
	    numGreenBlocks=0;
	  }
	}
}
