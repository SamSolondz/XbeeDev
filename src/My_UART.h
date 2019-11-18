/**
 * @file My_UART.h
 * @author Khalid AlAwadhi
 * @date 11 Nov 2019
 * @brief Library that contains all the functions related to setting up UARTs (Asynchronous).
 *
 * @note Built on functions and definitions provided by Silicon Labs \n
 * <b> Pearl Gecko does NOT support UART 2 and 3! Do not use it. I wrote the library to support
 * them so might as well keep it for future SiLabs boards that may support them. </b>
 */


#ifndef MY_UART_H_
#define MY_UART_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "em_cmu.h"
#include "em_usart.h"
#include "em_core.h"


/********************************
 * <!-- Defines -->
 ********************************/
#define UART0_RX_BUFF_SIZE		10			/**< UART0 RX buffer size */
#define UART1_RX_BUFF_SIZE		1			/**< UART1 RX buffer size */
#define UART2_RX_BUFF_SIZE		1			/**< UART2 RX buffer size */
#define UART3_RX_BUFF_SIZE		1			/**< UART3 RX buffer size */


/******************************************************************
 * @brief Custom UART structure to initialize a UART instance.
 *
 * This structure can be used to initialize a chosen UART instance
 * on the Pearl Gecko (Only supports UART 0 and 1). Offers a lot of
 * parameters to be modified. However, for more functionality,
 * please explore the initialization function itself as not all
 * parameters are exposed in this custom structure.
 ******************************************************************/
typedef struct
{
	/** The UART to init:
	 * - 0: Uses UART0
	 * - 1: Uses UART1
	 * - 2: Uses UART2
	 * - 3: Uses UART3 */
	uint8_t UART_Instance;

	/** The UART base memory location. This is auto set based on uint8_t UART_Instance */
	USART_TypeDef * UARTx;

	/** Enable UART after initialization:
	 * - True => Enables UART after initialization
	 * - False => Keeps the UART disabled after initialization */
	bool EnableWhenDone;

	/** GPIO Port of the RX Port. Use the enum defined by SiLabs (gpioPortX) */
	uint8_t RX_Port;

	/** GPIO Pin of the RX */
	uint8_t RX_Pin;

	/** GPIO Port of the TX Port. Use the enum defined by SiLabs (gpioPortX - in 'em_gpio.h') */
	uint8_t TX_Port;

	/** GPIO Pin of the TX */
	uint8_t TX_Pin;

	/** Route of the RX. Use defines by SiLabs (USART_ROUTELOC0_RXLOC_LOCxx - in 'efm32pg12b_usart.h') */
	uint32_t RX_RouteLoc;

	/** Route of the TX. Use defines by SiLabs (USART_ROUTELOC0_TXLOC_LOCxx - in 'efm32pg12b_usart.h') */
	uint32_t TX_RouteLoc;

	/** UART Baud Rate */
	uint32_t BaudRate;

	/** Oversampling selection, used for asynchronous operation. Use defines by SiLabs in 'em_usart.h':
	 * - usartOVS16 => 16x oversampling [Normal]
	 * - usartOVS8 => 8x oversampling
	 * - usartOVS6 => 6x oversampling
	 * - usartOVS4 => 4x oversampling */
	uint8_t Oversampling;

	/** UART Databit selection. Use defines by SiLabs in 'em_usart.h':
	 * - usartDatabits8 => 8 data bits
	 * - usartDatabits9 => 9 data bits */
	uint8_t DataBits;

	/** UART Parity selection. Use defines by SiLabs in 'em_usart.h':
	 * - usartNoParity => No parity
	 * - usartEvenParity => Even parity
	 * - usartOddParity => Odd parity */
	uint16_t Parity;

	/** UART Stop bits selection. Use defines by SiLabs in 'em_usart.h':
	 * - usartStopbits0p5 => 0.5 stop bit
	 * - usartStopbits1 => 1 stop bit
	 * - usartStopbits1p5 => 1.5 stop bits
	 * - usartStopbits2 => 2 stop bits */
	uint16_t StopBits;

	/** UART Start frame (Start RXing when this char is received) */
	uint8_t StartFrame;

	/** UART End frame (End RXing when this char is received) */
	uint8_t EndFrame;

} MyUART_Struct;



/*******************************************************************************************************
 * @brief Initializes the desired UART based on the structure's settings. You must populate the structure
 * @p MyUART_Struct properly first. <b> Read the struct comments! </b>
 *
 * @param[in] UARTComm
 * 			Structure that contains all the wanted UARTx settings to init
 *
 * @return
 * 			- TRUE => Successfully init desired UART
 * 			- FALSE => Failed to init desired UART
 *
 * @note Need to implement proper start and end frame detection. Currently just coding that. Not sure
 * if the hardware for that exists like the LEUART module.
 *******************************************************************************************************/
bool UARTx_Init(MyUART_Struct *UARTComm);



/*******************************************************************************************************
 * @brief Transmit one char to desired UARTx TX buffer.
 *
 *
 * @param[in] UARTComm
 * 			Struct that contains all the wanted UART settings
 *
 * @param[in] TX_Char
 * 			Char to send
 *
 * @return
 * 			None
 *******************************************************************************************************/
void UARTx_Putchar(MyUART_Struct *UARTComm, char TX_Char);



/*******************************************************************************************************
 * @brief Transmit a string to desired UARTx TX buffer.
 *
 *
 * @param[in] UARTComm
 * 			Struct that contains all the wanted UART settings
 *
 * @param[in] TX_String
 * 			String to send
 *
 * @return
 * 			None
 *******************************************************************************************************/
void UARTx_Putchar_n(MyUART_Struct *UARTComm, char* TX_String);



/*******************************************************************************************************
 * @brief Check if TXC was asserted in the desired UARTx instance. If received, a true is returned and
 * the interrupt flag is cleared. Else, a false is returned.
 *
 * @param[in] UARTComm
 * 			The UART instance to check for TXC interrupt
 *
 * @return
 * 			- TRUE => TXC received
 * 			- FALSE => No TXC received
 *******************************************************************************************************/
bool Check_TXC_UARTx(MyUART_Struct *UARTComm);



/*******************************************************************************************************
 * @brief UART0 RX interrupt handler
 *
 *
 * @param
 * 			None
 *
 * @return
 * 			None
 *******************************************************************************************************/
void USART0_RX_IRQHandler();

/*******************************************************************************************************
 * @brief UART0 TX interrupt handler
 *
 *
 * @param
 * 			None
 *
 * @return
 * 			None
 *******************************************************************************************************/
void USART0_TX_IRQHandler();



/*******************************************************************************************************
 * @brief UART1 RX interrupt handler
 *
 *
 * @param
 * 			None
 *
 * @return
 * 			None
 *******************************************************************************************************/
void USART1_RX_IRQHandler();

/*******************************************************************************************************
 * @brief UART1 TX interrupt handler
 *
 *
 * @param
 * 			None
 *
 * @return
 * 			None
 *******************************************************************************************************/
void USART1_TX_IRQHandler();



/*******************************************************************************************************
 * @brief UART2 RX interrupt handler
 *
 *
 * @param
 * 			None
 *
 * @return
 * 			None
 *******************************************************************************************************/
void USART2_RX_IRQHandler();

/*******************************************************************************************************
 * @brief UART2 TX interrupt handler
 *
 *
 * @param
 * 			None
 *
 * @return
 * 			None
 *******************************************************************************************************/
void USART2_TX_IRQHandler();



/*******************************************************************************************************
 * @brief UART3 RX interrupt handler
 *
 *
 * @param
 * 			None
 *
 * @return
 * 			None
 *******************************************************************************************************/
void USART3_RX_IRQHandler();

/*******************************************************************************************************
 * @brief UART3 TX interrupt handler
 *
 *
 * @param
 * 			None
 *
 * @return
 * 			None
 *******************************************************************************************************/
void USART3_TX_IRQHandler();



#endif /* MY_UART_H_ */
