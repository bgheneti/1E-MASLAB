#ifndef INCLUDE_PICKUP_H_
#define INCLUDE_PICKUP_H_

#include "motor_firmware.h"
//#include "color_sensor_firmware.h"
#include "servo_firmware.h"
#include "limit_switch_firmware.h"
#include "color_sensor_firmware.h"
#include "color.h"
#include <thread>

namespace control {
	class Pickup {

		private:
			firmware::Motor& pickupMotor;
			firmware::Servo& sorter;
			firmware::LimitSwitch& limitSwitch;
			firmware::ColorSensor& colorSensor;
			std::thread runner;
			int numRedBlocks;
			int numGreenBlocks;
			int numBlocks;
			int running;
			int numPickedUpThisTime;
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
			int numOurTeamBlocks(utils::Color mode);
			int numTheirTeamBlocks(utils::Color mode);
			int getNumPickedUpThisTime();
			void releaseStack(utils::Color stackColor);

	};


}


#endif

