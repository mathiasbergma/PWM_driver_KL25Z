/*
 * pwm_driver.c
 *
 *  Created on: 22. nov. 2021
 *      Author: Mathias
 */
#include "pwm_driver.h"
/**
 * @brief Initializes PWM on builtin blue LED / Pin D1
 * @param period Sets the period of the PWM signal. 1000 = 48kHz. 48000 = 500Hz
 */
void initPWM(uint16_t period)
{
	/**Enable clock to port D*/
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	// Enable clock to TPM0
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

	// Set pin to FTM
	PORTD->PCR[PWMPIN] &= ~PORT_PCR_MUX_MASK;
	// PWMPIN FTM0_CH1, MUX Alt 4
	PORTD->PCR[PWMPIN] |= PORT_PCR_MUX(4);

	//Configure TPM
	// Set clock source for TPM: 48 MHz
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);
	// Load the counter and mod
	TPM0->MOD = period-1;
	// Set TPM count direction to up with a divide by 2 prescaler
	TPM0->SC = TPM_SC_PS(0);
	// Continue operation in debug mode
	TPM0->CONF |= TPM_CONF_DBGMODE(3);
	// Set channel 1 to edge-alligned high-true PWM
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
	// Set initial duty cycle
	TPM0->CONTROLS[1].CnV = 0;
	// Start TPM
	TPM0->SC |= TPM_SC_CMOD(1);

}
/**
 *
 * @param duty Dutycycle of the PWM signal. 0 - <period of PWM>
 * @return 1 if duty is greater than 2^16-1. Else 0
 */
int setDuty(uint16_t duty)
{
	if (duty > 65535)
	{
		return 1;
	}
	TPM0->CONTROLS[1].CnV = duty;
	return 0;
}

/**
 * @brief Used to scale the input value to PWM according to the period being used
 * @param period Period of PWM
 * @param input Value to be scaled for use with PWM
 */
unsigned short scalePWM(unsigned short period, unsigned short input)
{
	return (unsigned short)((double)period*((double)input/(double)USHRT_MAX));
}
