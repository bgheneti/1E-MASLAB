#include "../include/pickup.h"

namespace control {
	Pickup::Pickup(firmware::Motor pickupMotor,
		       firmware::ColorSensor colorSensor,
		       firmware::Servo sorter,
	               firmware::LimitSwitch switch) : pickupMotor(pickupMotor),
					              colorSensor(colorSensor),
						      sorter(sorter),
						      switch(switch) {}



}
