#ifndef INCLUDE_COLOR_SENSOR_FIRMWARE_H
#define INCLUDE_COLOR_SENSOR_FIRMWARE_H

namespace firmware {
	class ColorSensor {
		private:
			utils::Color color;
		public:
			void startPoll();
			void stopPoll();
			void getColor();	

	};

}
#endif
