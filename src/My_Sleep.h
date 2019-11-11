/*
 * My_Sleep.h
 *
 *  Created on: Jun 1, 2019
 *      Author: Khalid AlAwadhi
 ---------------------------------------------------------------------------------------------
 * # INFO: Library that contains all the sleep functions for the Pearl Gecko board
 *
 * # NOTES:
 *   -
 *
 *  # FUTURE PLANS:
 *   -
 ---------------------------------------------------------------------------------------------*/

#ifndef SRC_HEADER_FILES_MY_SLEEP_H_
#define SRC_HEADER_FILES_MY_SLEEP_H_

#include <stdint.h>

/*********************************
 * Defines                       *
 *********************************/
enum EnergyModes
{
	EM0 = 0,
	EM1 = 1,
	EM2 = 2,
	EM3 = 3,
	EM4 = 4
};


/**************************************************************************************************************
 * INFO: This function will enter the lowest possible sleep mode allowed.
 *
 * PARAMETERS:
 *            - None
 *
 * RETURNS: None
 **************************************************************************************************************/
void Enter_Sleep();


/**************************************************************************************************************
 * INFO: This function blocks an energy mode, which will not allow the board to go to sleep any lower than the
 *       chosen mode. After the peripheral is finished with the operation, it should call unblock with the
 *       same state.
 *
 * PARAMETERS:
 *            - uint8_t Mode => Energy mode to not enter or go below
 *
 * RETURNS: None
 **************************************************************************************************************/
void Block_Energy_Mode(uint8_t Mode);


/**************************************************************************************************************
 * INFO: This function unblocks an energy mode that was previously blocked. This should be called after all
 *       the work on a peripheral that required the specific energy mode is done.
 *
 * PARAMETERS:
 *            - uint8_t Mode => Energy mode to unblock
 *
 * RETURNS: None
 **************************************************************************************************************/
void Unblock_Energy_Mode(uint8_t Mode);



#endif /* SRC_HEADER_FILES_MY_SLEEP_H_ */
