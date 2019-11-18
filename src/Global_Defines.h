/*
 * Global_Defines.h
 *
 *  Created on: Jun 2019
 *      Author: Khalid AlAwadhi
 *
 * # INFO: This file contains all the defines shared across the project.
 *
 */

#ifndef SRC_HEADER_FILES_GLOBAL_DEFINES_H_
#define SRC_HEADER_FILES_GLOBAL_DEFINES_H_


/*****************************************
 * Defines                               *
 *****************************************/
#define SYSTEM_ENERGY_MODE			EM3		//The lowest energy mode the system will block when idle
#define LETIMER_ENERGY_MODE			(SYSTEM_ENERGY_MODE == 0) ? 0 : SYSTEM_ENERGY_MODE-1	//The lowest energy mode the LETIMER will operate at based on the system lowest EM

//LETIMER:
#define PERIOD						5.0		//The period in seconds
#define ON_TIME						0.08	//The amount of time to wait for the sensor to warm-up

//Si7021:
#define TEMP_RES					14		//Used to set the temp resolution of the Si7021 sensor
#define TEMP_THRESHOLD				27.0	//Temperature threshold, if the read temp is above this, LED0 will turn ON

//Scheduler Defines:
#define READ_TEMP_EVENT				0b00000001
#define DECODE_RX_BUFFER			0b00000010
#define TIMER_UF					0b00000100

//LEUART:
#define RX_BUFF_SIZE				50

//AT Command Response
#define AT_COMMAND_PH			(0x4)	//Command reads SIM card phone number

#endif /* SRC_HEADER_FILES_GLOBAL_DEFINES_H_ */
