/*
 * pwm_driver.h
 *
 *  Created on: 22. nov. 2021
 *      Author: Mathias
 */

#ifndef PWM_DRIVER_H_
#define PWM_DRIVER_H_

#include "board.h"
#include <limits.h>

#define PWMPIN (1)

void initPWM(uint16_t period);
int setDuty(uint16_t duty);
unsigned short scalePWM(unsigned short period, unsigned short input);

#endif /* PWM_DRIVER_H_ */
