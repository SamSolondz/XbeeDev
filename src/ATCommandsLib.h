/*
 * ATparse.h
 *
 *  Created on: Nov 10, 2019
 *      Author: Sam Solondz
 */


// Enclosing macro to prevent multiple inclusion
#ifndef __ATCOMMANDS__
#define __ATCOMMANDS__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "My_LEUART.h"
#include "log.h"
#include "letimer.h"
#include "My_Logger.h"

extern volatile bool waitForResp;
enum ATResponse {AT_OK, AT_ERROR};

int sendATCommandWrite(const char * cmd, const char * param);
void XbeeEnterCommandMode();
void XbeeExitCommandMode();
void XbeeSetupSMSSend();
void XbeeChangePH(const char * phoneNumber);
void XbeeWait(int waitMs);
void ParseXbeeRx(const char * RxCmd);


#endif // AT_PARSE
