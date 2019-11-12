
#ifndef SRC_LETIMER_H_
#define SRC_LETIMER_H_

#include <stdbool.h>
#include "em_letimer.h"
#include "em_cmu.h"
#include "My_Sleep.h"

extern volatile bool waitForResp;

extern uint16_t GlobalEventReg;
void LETIMER0_Setup(int TotalPeriod_ms);
void timerWaitUs(uint32_t us_wait);
uint32_t loggerGetTimestamp();
void timerSetEventInMs(int ms_until_wakeup);

#endif
