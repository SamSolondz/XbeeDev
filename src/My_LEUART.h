/*
 * My_LEUART.h
 *
 *  Created on: Jun 30, 2019
 *      Author: Khalid AlAwadhi
 ---------------------------------------------------------------------------------------------
 * # INFO: Library that contains all the functions related to the LEUART
 *
 * # NOTES:
 *   -
 *
 *  # FUTURE PLANS:
 *   -
 ---------------------------------------------------------------------------------------------*/

#ifndef SRC_HEADER_FILES_MY_LEUART_H_
#define SRC_HEADER_FILES_MY_LEUART_H_

//RX:
#define LEUART0_RX_PORT				gpioPortD
#define LEUART0_RX_PIN				11
#define LEUART0_RX_DEFAULT			0

//TX:
#define LEUART0_TX_PORT				gpioPortD
#define LEUART0_TX_PIN				10
#define LEUART0_TX_DEFAULT			1



/**************************************************************************************************************
 * INFO: This function sets up the LEUART0
 *        - Enable needed clocks that were not already enabled
 *        - Init and sets up RX and TX PINs
 *        - Configures and initializes LEUART0 structure
 *        - Routes RX and TX PINs
 *        - Enables TXC interrupt and LEUART0_IRQn
 *        - Starts LEUART
 *
 * PARAMETERS:
 *            - NONE
 *
 * RETURNS: NONE
 **************************************************************************************************************/
void LEUART0_Setup();



/**************************************************************************************************************
 * USAGE: This function will transmit one char to the UART TX buffer
 *
 * PARAMETERS:
 *            - char TX_Char => char to send
 *
 * RETURNS: NONE
 *
 **************************************************************************************************************/
void LEUART0_Putchar(char TX_Char);



/**************************************************************************************************************
 * USAGE: This function will transmit a string to the UART. It will go to sleep between character
 *        transmissions.
 *
 * PARAMETERS:
 *            - char* TX_String => String to send to UART
 *
 * RETURNS: NONE
 *
 **************************************************************************************************************/
void LEUART0_Putchar_n(char* TX_String);



/**************************************************************************************************************
 * INFO: LEUART0 IRQ Handler.
 *
 * PARAMETERS:
 *            - NONE
 *
 * RETURNS: NONE
 *
 **************************************************************************************************************/
void LEUART0_IRQHandler();



#endif /* SRC_HEADER_FILES_MY_LEUART_H_ */
