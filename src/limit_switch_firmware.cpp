#include "../include/limit_switch_firmware.h"
#include <thread>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

namespace firmware {
  LimitSwitch::LimitSwitch(int pin) : running(0), state(1), gpio(pin) {};

	void LimitSwitch::poll() {
	  while(running) {
	    state=gpio.read();
	    usleep(10000);
	  }

	}

	void LimitSwitch::startPoll() {
		running = 1;
		gpio.dir(mraa::DIR_IN);
		gpio.mode(mraa::MODE_PULLUP);
		std::thread thr(&LimitSwitch::poll, this);
		thr.detach();
	}

	void LimitSwitch::stopPoll() {
		running = 0;
	}

	int LimitSwitch::getState() {
		return state;
	}
}
