#include "../include/dropoff.h"
#include <iostream>

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
		std::cout << "reseting stack" << std::endl;
		floor.setServoPosition(35.0*dir);
		if(dir > 0) {
			opener.setServoPosition(-76.0);
		} else {
			opener.setServoPosition(-85.0);
		}
	}

}
