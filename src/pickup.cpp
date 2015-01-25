#include "../include/pickup.h"

#define SORT_CENTER 5.0
#define SORT_DELTA 70.0
namespace control {
	Pickup::Pickup(firmware::Motor pickupMotor,
		       firmware::Servo sorter) : pickupMotor(pickupMotor),
						 sorter(sorter) {}

        void Pickup::start() {
		sorter.setServoPosition(SORT_CENTER + SORT_DELTA);
                usleep(5000000);
                sorter.setServoPosition(SORT_CENTER - SORT_DELTA);
	}

        void Pickup::stop() {

        }



}
