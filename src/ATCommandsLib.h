/*
 * ATparse.h
 *
 *  Created on: Nov 10, 2019
 *      Author: Sam Solondz
 */


// Enclosing macro to prevent multiple inclusion
#ifndef __AT_PARSE__
#define AT_PARSE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "My_LEUART.h"
#include "log.h"

enum ATResponse {AT_OK, AT_ERROR};

int sendATCommandWrite(const char * cmd, const char * param);
void XbeeSMSConfigure();
#endif // AT_PARSE
