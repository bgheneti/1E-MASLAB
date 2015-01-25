#ifndef INCLUDE_DROPOFF_H_
#define INCLUDE_DROPOFF_H_

#include "servo_firmware.h"

namespace control {
	class Dropoff {
		private:
			firmware::Servo floor;
			firmware::Servo opener;
                        int dir;
		public:
			Dropoff(firmware::Servo floor,
				firmware::Servo opener,
				int dir);
			void dropStack();
			void resetStack();

	};

}
#endif
