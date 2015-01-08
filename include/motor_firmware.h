#ifndef __MOTOR_FIRMWARE_H_INCLUDED__
#define __MOTOR_FIRMWARE_H_INCLUDED__

#include "mraa.hpp"

void setMotorSpeed(mraa::Pwm& pwm, mraa::Gpio& dir, double speed);

#endif
