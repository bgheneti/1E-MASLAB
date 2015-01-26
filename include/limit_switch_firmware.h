#ifndef INCLUDE_LIMIT_SWITCH_H_
#define INCLUDE_LIMIT_SWITCH_H_

#include "mraa.hpp"

namespace firmware {
	class LimitSwitch {
		private:
			bool state;
			void poll();
			bool running;
			mraa::Gpio* gpio = new mraa::Gpio(9);
		public:
			LimitSwitch();
			void startPoll();
			void stopPoll();
			bool getState();
	};
}

#endif
