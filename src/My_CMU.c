/*
 * My_CMU.c
 *
 *  # ALL INFO CAN BE FOUND IN THE .h FILE #
 */
#include "My_CMU.h"

/**************************
 * Standard Includes      *
 **************************/


/**************************
 * Silicon Labs Includes  *
 **************************/
#include "em_emu.h"
#include "em_cmu.h"

/**************************
 * My Includes            *
 **************************/



void CMU_Init()
{
	/* Init DCDC regulator and HFXO with kit specific parameters
	 * Initialize DCDC. Always start in low-noise mode. */
	EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
	CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;
	CMU_LFXOInit_TypeDef lfxoInit = CMU_LFXOINIT_DEFAULT;
	EMU_EM23Init_TypeDef em23Init = EMU_EM23INIT_DEFAULT;
	EMU_DCDCInit(&dcdcInit);
	em23Init.vScaleEM23Voltage = emuVScaleEM23_LowPower;
	EMU_EM23Init(&em23Init);
	CMU_HFXOInit(&hfxoInit);
	CMU_LFXOInit(&lfxoInit);

	/* Switch HFCLK(High Frequency Clock) to use HFXO(High Frequency Crystal Oscillator) */
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);			//Note that we did not enable the oscillator, as by default, the HFXO is enabled

	//HFPER(High Frequency Peripheral) Clock is enabled by default

	/* Disable HFRCO and LFRCO as they're enabled by default and we do not need it now (to save power) */
	CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);
	CMU_OscillatorEnable(cmuOsc_LFRCO, false, false);

	/* Enable the low-energy clock */
	CMU_ClockEnable(cmuClock_CORELE, true);
}
