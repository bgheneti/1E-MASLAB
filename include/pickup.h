#ifndef INCLUDE_PICKUP_H_
#define INCLUDE_PICKUP_H_

#include "motor_firmware.h"
//#include "color_sensor_firmware.h"
#include "servo_firmware.h"
#include "limit_switch_firmware.h"
#include "color_sensor_firmware.h"
#include <thread>

namespace control {
	class Pickup {

		private:
			firmware::Motor& pickupMotor;
			firmware::Servo& sorter;
			firmware::LimitSwitch& limitSwitch;
			firmware::ColorSensor& colorSensor;
			int numRedBlocks;
			int numGreenBlocks;
			int numBlocks;
			int running;
		public:
			Pickup(firmware::Motor& pickupMotor,
			       firmware::Servo& sorter,
			       firmware::LimitSwitch& limitSwitch,
			       firmware::ColorSensor& colorSensor);

			void start();
			void run();
			void stop();

			int numRedBlocksPickedUp();
			int numGreenBlocksPickedUp();
			void releaseStack(utils::Color stackColor);

	};


}


#endif

