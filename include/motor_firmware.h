#ifndef __MOTOR_FIRMWARE_H_INCLUDED__
#define __MOTOR_FIRMWARE_H_INCLUDED__

#include "mraa.hpp"

void setMotorSpeed(Pwm& pwm, Gpio& dir, double speed);

#endif
