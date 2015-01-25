#include "../include/limit_switch_firmware.h"
#include <thread>
namespace firmware {
	LimitSwitch::LimitSwitch() : running(false), state(false) {}

	void LimitSwitch::poll() {
		while(running) {
			// Check for state and set it here
		}

	}

	void LimitSwitch::startPoll() {
		running = true;
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
