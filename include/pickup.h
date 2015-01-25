#ifndef INCLUDE_PICKUP_H_
#define INCLUDE_PICKUP_H_

#include "motor_firmware.h"
//#include "color_sensor_firmware.h"
#include "servo_firmware.h"
//#include "limit_switch_firmware.h"
#include "color.h"

namespace control {
	class Pickup {

		private:
			firmware::Motor pickupMotor;
			firmware::Servo sorter;
			int numRedBlocks;
			int numGreenBlocks;

		public:
			Pickup(firmware::Motor pickupMotor,
			       firmware::Servo sorter);

			void start();

			void stop();

			int numRedBlocksPickedUp();
			int numGreenBlocksPickedUp();
			void releaseStack(utils::Color stackColor);

	};


}


#endif

