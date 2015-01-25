#ifndef INCLUDE_LIMIT_SWITCH_H_
#define INCLUDE_LIMIT_SWITCH_H_

namespace firmware {
	class LimitSwitch {
		private:
			bool state;
			void poll();
			bool running;
		public:
			LimitSwitch();
			void startPoll();
			void stopPoll();
			bool getState();

	};
}

#endif
