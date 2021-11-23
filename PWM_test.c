/**
 * @file    PWM_First.c
 * @brief   Application entry point.
 * @author	MathiasBergMa
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#include "pwm_driver.h"
#include "adc_irq.h"

#define PERIOD (256)

volatile unsigned short adc_res;

void ADC0_IRQHandler()
{
	adc_res = (unsigned short) ADC0->R[0];
}

int main(void)
{

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif
	// Initialize variable for debug
	unsigned short scaledVal = 0;

	/* Initialize ADC
	 * Channel 0
	 * Continous conversion
	 * Harware averaging enabled
	 * 4 sample averaging
	 */
	adcInit(0, 1, 1, 4);
	// Initialize PWM
	initPWM(PERIOD);
	//Start first conversion
	adcStart(0);
	/* Force the counter to be placed into memory. */
	volatile static int i = 0;
	/* Enter an infinite loop, just incrementing a counter. */
	while (1)
	{
		i++;
		// Scale adc value for PWM
		scaledVal = scalePWM(PERIOD, adc_res);
		// Set dutyCycle of PWM with scaled value
		setDuty(scaledVal);
		//PRINTF("%d\n", scaledVAL);

		__asm volatile ("nop");
	}
	return 0;
}
