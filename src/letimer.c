#include "letimer.h"
#include "em_core.h"
#include "Global_Defines.h"
#include <stdlib.h>

#define INCLUDE_LOG_DEBUG 1
#define FREQ_ULFRCO 1000

#define LETIMER_VALUE_FROM_MS(ms_period, clock_frequency_hz, leclock_divider) \
	(uint16_t) (((ms_period) * ((clock_frequency_hz)/(leclock_divider)))/1000)


LETIMER_Init_TypeDef letimerInit;
uint32_t rolloverCount = 0;			//Will break at somepoint when overflows
int TimerPeriod = 0;

extern volatile uint8_t SchedulerEvent;

typedef struct {
	int frequency_hz;
	int divider;
}clock_attr;
clock_attr currentClock;

void LETIMER0_Setup(int TotalPeriod_ms)
{

	/* Enable LETIMER0 clock */
	CMU_ClockEnable(cmuClock_LETIMER0, true);

	TimerPeriod = TotalPeriod_ms;

	/* Disable LFXO to save power and it is not needed */
	CMU_OscillatorEnable(cmuOsc_LFXO, false, false);
	/* Change LFA clock source to be from the ULFRCO */
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);

	currentClock.frequency_hz = CMU_ClockFreqGet(cmuClock_LFA);
	currentClock.divider = 1;

	//CMU_ClockEnable(cmuClock_LFA, true);
	CMU->LFACLKEN0 |= CMU_LFACLKEN0_LETIMER0;

	/*Enable LETIMER Interrupts*/
	//LETIMER_IntEnable(LETIMER0, LETIMER_IEN_UF);
	LETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP1);
	LETIMER_CompareSet(LETIMER0, 0, LETIMER_VALUE_FROM_MS(TotalPeriod_ms, currentClock.frequency_hz, currentClock.divider));

	letimerInit.enable	 	=	true;
	letimerInit.debugRun	=	true;
	letimerInit.comp0Top	=	true;
	letimerInit.bufTop		=	false;
	letimerInit.out0Pol		= 	0;
	letimerInit.out1Pol		= 	0;
	letimerInit.ufoa0		= 	letimerUFOANone;
	letimerInit.ufoa1		= 	letimerUFOANone;
	letimerInit.repMode		=   letimerRepeatFree;

	LETIMER_Init(LETIMER0, &letimerInit);

	NVIC_EnableIRQ(LETIMER0_IRQn);
}

void LETIMER0_IRQHandler(void)
{
	CORE_irqState_t irq_state;

	irq_state = CORE_EnterCritical();
	uint32_t interruptVal = LETIMER_IntGetEnabled(LETIMER0);
	LETIMER_IntClear(LETIMER0, interruptVal);
	CORE_ExitCritical(irq_state);

	if(interruptVal & LETIMER_IFC_UF) //underflow interrupt triggered
	{
		//SchedulerEvent |= TIMER_UF;
		rolloverCount++;
	}

	/*Timer wait delay*/
	if (interruptVal & LETIMER_IFC_COMP1)
	{
		LETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP1); //Disable after one expiration
		waitForResp = false;
	}
}

/*Blocks for at least us_wait microseconds, uses LETIMER0 tick counts as reference
 	supports waits for Load Power Mangaement and I2C */
void timerWaitUs(uint32_t us_wait)
{
	uint32_t currentWait = 0;
	uint16_t currentCnt = LETIMER0->CNT;

	while(currentWait < us_wait)
	{
		if(currentCnt != LETIMER0->CNT)
		{
			currentCnt = LETIMER0->CNT;
			if(currentClock.frequency_hz == 32768)
				currentWait = currentWait + 60;  /*Each timer tick is ~60us for LETIMER frequency (32768/2 Hz)*/
			else
				currentWait = currentWait + 1000; /*Each timer tick is ~1ms for ULFRCO frequency (1000 Hz)*/
		}
	}
}


/*TODO: consider testing a max value for calc rollover*/
void timerSetEventInMs(int ms_until_wakeup)
{
	uint32_t setTick;
	uint32_t waitUntilTick = LETIMER_VALUE_FROM_MS(ms_until_wakeup, currentClock.frequency_hz, currentClock.divider);
	uint32_t currentCount = LETIMER0->CNT;

	/*Check for wrap around*/
	if(waitUntilTick > currentCount)
	{
		setTick = abs(LETIMER0->COMP0 - (waitUntilTick - currentCount));
	}
	else
		setTick = currentCount - waitUntilTick;

	LETIMER_CompareSet(LETIMER0, 1, setTick);
	LETIMER_IntClear(LETIMER0, LETIMER_IFC_COMP1);
	LETIMER_IntEnable(LETIMER0, LETIMER_IEN_COMP1);
}


/*Returns ms elapsed since system start.*/
uint32_t loggerGetTimestamp()
{
	uint16_t currentTimerCnt = LETIMER0->CNT;
	uint32_t elapsedMs = 0;
	if(currentClock.frequency_hz == FREQ_ULFRCO)
		elapsedMs = (uint32_t)(rolloverCount * TimerPeriod) + (LETIMER0->COMP0 - currentTimerCnt);

	//LOG_INFO("Elapsed ms = %d", elapsedMs);
	return elapsedMs/1000;
}
