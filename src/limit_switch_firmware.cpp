#include "../include/limitswitchfirmware.h"

namespace firmware {
	LimitSwitch::LimitSwitch() : running(false), state(false) {}

	LimitSwitch::poll() {
		while(running) {
			// Check for state and set it here
		}

	}

	LimitSwitch::startPoll() {
		running = true;
		std::thread thr(&LimitSwitch::poll, this);  
		thr.detach();
	}

	LimitSwitch::stopPoll() {
		running = false;
	}

	LimitSwitch::getState() {
		return state;
	}
}
