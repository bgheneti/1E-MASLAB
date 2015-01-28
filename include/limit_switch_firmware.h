#ifndef INCLUDE_LIMIT_SWITCH_H_
#define INCLUDE_LIMIT_SWITCH_H_

#include "mraa.hpp"

namespace firmware {
	class LimitSwitch {
		private:
			int state;
			void poll();
			int running;
			mraa::Gpio gpio;
	        public:
			LimitSwitch(int pin);
			void startPoll();
			void stopPoll();
			int getState();
	};
}

#endif
