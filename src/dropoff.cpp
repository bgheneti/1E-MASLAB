#include "../include/dropoff.h"

namespace control {
	Dropoff::Dropoff(firmware::Servo floor, 
		         firmware::Servo opener,
	                             int dir) : floor(floor),
			                        opener(opener),
	                                        dir(dir) {}
	void Dropoff::dropStack() {
	        floor.setServoPosition(90.0*dir);	
		opener.setServoPosition(0.0);
	}

	void Dropoff::resetStack() {
		floor.setServoPosition(45.0*dir);
		opener.setServoPosition(-90.0);
	}

}
