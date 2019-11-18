/**
 * @file My_Logger.h
 * @author Khalid AlAwadhi
 * @date 11 Nov 2019
 * @brief Library that contains all the functions related to setting up the custom-made logger.
 *
 * @note Not real-time but should be fine for most applications.
 */


#ifndef MY_LOGGER_H_
#define MY_LOGGER_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "em_cmu.h"

#include "My_UART.h"


//BT Logger UART PINs
#define	BT_RX_PORT		gpioPortF
#define BT_RX_PIN		4
#define BT_RX_LOC		USART_ROUTELOC0_RXLOC_LOC27

#define	BT_TX_PORT		gpioPortF
#define BT_TX_PIN		3
#define BT_TX_LOC		USART_ROUTELOC0_TXLOC_LOC27


/***************************************
 * @brief Log Level enum:
 * Used distinguish different log levels
 ***************************************/
typedef enum
{
	INFO = 0,
	WARNING = 1,
    ERROR = 2,
	CRITICAL = 3,
	FATAL = 4
} LogLevel_Enum;


/*******************************************************************************************************
 * @brief Initializes UART0 to support logging via UART to BT module attached. It also inits Systick
 * interrupts to be generated every 1ms for log monotonic timing
 *
 * @param
 * 			None
 *
 * @return
 * 			None
 *******************************************************************************************************/
void Log_Msg_UART_Init();



/*******************************************************************************************************
 * @brief Logs a message to the UART port. Adds monotonic time to every log.
 *
 * @param[in] Src
 * 			Source of the log (ex: function name)
 *
 * @param[in] LogLvl
 * 			Log level of the message, use the enums defined in this file:
 * 			- INFO
 * 			- WARNING
 * 			- ERROR
 * 			- CRITICAL
 * 			- FATAL
 *
 * @param[in] Msg
 * 			Message to output in the log
 *
 * @return
 * 			None
 *******************************************************************************************************/
void Log_Msg_UART(char *Src, uint8_t LogLvl, char* Msg);



/*******************************************************************************************************
 * @brief SysTick_IRQ handler. Simply increments the Tick private variable depending on the SysTick_Config()
 * function in Log_Msg_UART_Init()
 *
 * @param
 * 			None
 *
 * @return
 * 			None
 *******************************************************************************************************/
void SysTick_Handler();



#endif /* MY_LOGGER_H_ */
