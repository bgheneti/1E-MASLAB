#ifndef INCLUDE_COLOR_SENSOR_FIRMWARE_H
#define INCLUDE_COLOR_SENSOR_FIRMWARE_H

#include "mraa.hpp"
#include "color.h"
namespace firmware {
	class ColorSensor {
		private:
	                mraa::Gpio gpio;
	                void poll();
			utils::Color color;
			int running;
		public:
			ColorSensor(int pin);
			void startPoll();
			void stopPoll();
			utils::Color getColor();	

	};

}
#endif
