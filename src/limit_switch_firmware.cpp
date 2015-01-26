#include "../include/limit_switch_firmware.h"
#include <thread>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

namespace firmware {
  LimitSwitch::LimitSwitch() : running(false), state(false) {}

	void LimitSwitch::poll() {
	  std::cout<<"gpio"<<gpio<<std::endl;
	  while(running) {
	    std::cout<<gpio->read()<<std::endl;
	    usleep(500000);
	  }

	}

	void LimitSwitch::startPoll() {
		running = true;
		std::cout<<"endl:"<<gpio->dir(mraa::DIR_IN)<<std::endl;
		gpio->mode(mraa::MODE_PULLUP);
		std::thread thr(&LimitSwitch::poll, this);  
		thr.detach();
	}

	void LimitSwitch::stopPoll() {
		running = false;
	}

	bool LimitSwitch::getState() {
		return state;
	}
}
