/*
 * My_Sleep.c
 *
 *  # ALL INFO CAN BE FOUND IN THE .h FILE #
 */
#include "My_Sleep.h"

#include <stdbool.h>
#include "em_emu.h"
#include "em_core.h"


#include <stdio.h>

//Global Variables
extern uint8_t EnergyMode_Counter[5];



void Enter_Sleep()
{
	/* Check if something blocked EM0, if something did return and stay in EM0 */
	if (EnergyMode_Counter[EM0] > 0)
	{
		return;
	}

	/* Check if something blocked EM1, if something did return and stay in EM0 */
	else if(EnergyMode_Counter[EM1] > 0)
	{
		return;
	}

	/* Check if something blocked EM2, if something did, enter EM1 */
	else if(EnergyMode_Counter[EM2] > 0)
	{
		EMU_EnterEM1();					//This function is provided by Silicon Labs to enter EM1
	}

	/* Check if something blocked EM3, if something did, enter EM2 */
	else if(EnergyMode_Counter[EM3] > 0)
	{
		EMU_EnterEM2(true);				//This function is provided by Silicon Labs to enter EM2
	}

	/* Else, if nothing is blocked that means we enter the lowest energy mode, EM3 */
	else
	{
		EMU_EnterEM3(true);				//This function is provided by Silicon Labs to enter EM3
	}
}


void Block_Energy_Mode(uint8_t Mode)
{
	CORE_AtomicDisableIrq();				//Disables interrupts

	/* Error check: We only want the defined energy modes */
	if(Mode <= 4)
	{
		EnergyMode_Counter[Mode]++;
	}
	else
	{
		printf("> ERROR: Out of bounds Energy Mode\n");
	}

	CORE_AtomicEnableIrq();					//Enables interrupts
}


void Unblock_Energy_Mode(uint8_t Mode)
{
	CORE_AtomicDisableIrq();				//Disables interrupts

	/* Error check: We only want the defined energy modes */
	if(Mode <= 4)
	{
		if(EnergyMode_Counter[Mode] > 0)	//If there's a non-zero value:
		{
			EnergyMode_Counter[Mode]--;
		}
	}
	else
	{
		printf("> ERROR: Out of bounds Energy Mode\n");
	}

	CORE_AtomicEnableIrq();					//Enables interrupts
}

