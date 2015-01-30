#include "../include/pickup.h"
#include "../include/color.h"
#include <iostream>
#define SORT_CENTER 5.0
#define SORT_FAST_DELTA 20.0
#define SORT_OVERSHOOT_DELTA 77.0
#define SORT_FINAL_DELTA 67.0
namespace control {
	Pickup::Pickup(firmware::Motor& pickupMotor,
		       firmware::Servo& sorter, firmware::LimitSwitch& limitSwitch, firmware::ColorSensor& colorSensor) : pickupMotor(pickupMotor), sorter(sorter), limitSwitch(limitSwitch), colorSensor(colorSensor), numRedBlocks(0), numGreenBlocks(0){}

        void Pickup::start(){
	  numPickedUpThisTime = 0;
	  std::thread thr(&Pickup::run, this);
	  std::swap(thr, runner);

	} 

        void Pickup::run(){
	  std::cout<<"run"<<std::endl;
	  sorter.setServoPosition(SORT_CENTER);
	  running=1;
	  pickupMotor.setSpeed(-0.5);
	  while(running){
	    if(limitSwitch.getState()==0){
	      colorSensor.startPoll();
	      numBlocks+=1;
	      pickupMotor.setSpeed(0);
	      usleep(1000000);
	      std::cout<<"color:"<<colorSensor.getColor()<<std::endl;
	      if(colorSensor.getColor()==utils::Color::RED){
		sorter.setServoPosition(SORT_CENTER+SORT_FAST_DELTA);
		for(int delta=SORT_FAST_DELTA;delta<SORT_OVERSHOOT_DELTA+1;delta++){
		  sorter.setServoPosition(SORT_CENTER + delta);
		  usleep(10000);
		}
		for(int i=0;i<4;i++){
                  sorter.setServoPosition(SORT_CENTER+SORT_OVERSHOOT_DELTA);
		  usleep(100000);
		  sorter.setServoPosition(SORT_CENTER+SORT_FINAL_DELTA);
		  usleep(100000);
		}
		numRedBlocks++;
	      }
	      else if(colorSensor.getColor()==utils::Color::GREEN){
		sorter.setServoPosition(SORT_CENTER-SORT_FAST_DELTA);
		for(int delta=SORT_FAST_DELTA;delta<SORT_OVERSHOOT_DELTA+1;delta++){
                  sorter.setServoPosition(SORT_CENTER - delta);
                  usleep(10000);
                }
		for(int i=0;i<4;i++){
                  sorter.setServoPosition(SORT_CENTER-SORT_OVERSHOOT_DELTA);
                  usleep(100000);
                  sorter.setServoPosition(SORT_CENTER-SORT_FINAL_DELTA);
                  usleep(100000);
		}
		numGreenBlocks++;
	      }
	      usleep(500000);
	      sorter.setServoPosition(SORT_CENTER);
	      usleep(500000);
	      pickupMotor.setSpeed(-0.5);
	      colorSensor.stopPoll();
	    }
	    sorter.setServoPosition(SORT_CENTER+2);
	    usleep(30000);
	    sorter.setServoPosition(SORT_CENTER-2);
	    usleep(30000);
	  }
	  usleep(50000);
	  pickupMotor.setSpeed(0.0);
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

	int Pickup::numOurTeamBlocks(utils::Color mode) {
		if(mode==utils::Color::RED) {
			return numRedBlocks;
		} else {
			return numGreenBlocks;
		} 
	}

	int Pickup::numTheirTeamBlocks(utils::Color mode) {
		if(mode==utils::Color::RED) {
			return numGreenBlocks;
		} else {
			return numRedBlocks;
		}
	}

	int Pickup::getNumPickedUpThisTime() {
		return numPickedUpThisTime;
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
