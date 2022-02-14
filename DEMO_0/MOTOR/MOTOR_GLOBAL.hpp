#ifndef __MOTOR_GLOBAL_HPP
#define __MOTOR_GLOBAL_HPP
#include "main.h"
#include "motor.hpp"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_pin);

void GlobalPosPID(void *argument);

void GlobalSpeedPID(void *argument);

void GlobalCalSpeed(const char *name);

#endif
