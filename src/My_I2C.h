/**
 * @file My_I2C.h
 * @author Khalid AlAwadhi
 * @date 1 Oct 2019
 * @brief Library that contains all the functions related to setting up the I2C bus on the Pearl Gecko.
 *
 * @note Built on functions and definitions provided by Silicon Labs \n
 * <b> Custom-built for EFM32PG1B200F256GM48 (our custom PCB) \n
 * Only 1 I2C is available </b>
 */


#ifndef MY_I2C_H_
#define MY_I2C_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "em_i2c.h"
#include "em_cmu.h"

#include "Global_Defines.h"


/********************************
 * <!-- Defines -->
 ********************************/
#define READ					1			/**< Used in I2C communication */
#define WRITE					0			/**< Used in I2C communication */



/******************************************************************
 * @brief Custom I2C structure to initialize an I2C bus.
 *
 * This structure can be used to initialize a chosen I2C bus on
 * the Pearl Gecko. Offers a lot of parameters to be modified.
 ******************************************************************/
typedef struct
{
	/** The I2C Bus to init:
	 * - 0: Uses I2C0 */
	bool I2CBus;

	/** The I2C Bus location. This is auto set based on bool I2CBus */
	I2C_TypeDef * I2CX;

	/** I2C bus after initialization:
	 * - True: Enables I2C bus after initialization
	 * - False: Keeps the I2C bus disabled after initialization */
	bool EnableWhenDone;

	/** Master mode:
	 * - True: Set to Master mode
	 * - False: Set to Slave mode */
	bool SetToMaster;

	/** I2C reference clock. Only available if Master. Default value is 0. */
	uint32_t RefClock;

	/** I2C bus frequency to use. Use the defines provided by SiLabs in 'em_i2c.h':
	 * - I2C_FREQ_STANDARD_MAX => 92000, use 4:4 ratio
	 * - I2C_FREQ_FAST_MAX => 392157, use 6:3 ratio
	 * - I2C_FREQ_FASTPLUS_MAX => 987167, use 11:6 ratio */
	uint32_t BusFreq;

	/** Clock low to high ratio settings. Use the defines provided by SiLabs in 'em_i2c.h':
	 * - i2cClockHLRStandard => Ratio is 4:4
	 * - i2cClockHLRAsymetric => Ratio is 6:3
	 * - i2cClockHLRFast => Ratio is 11:6 */
	uint8_t ClockRatio;

	/** GPIO Port of the SCL Port. Use the enum defined by SiLabs (gpioPortX) */
	uint8_t SCL_Port;

	/** GPIO Pin of the SCL */
	uint8_t SCL_Pin;

	/** GPIO Port of the SDA Port. Use the enum defined by SiLabs (gpioPortX - in 'em_gpio.h') */
	uint8_t SDA_Port;

	/** GPIO Pin of the SDA */
	uint8_t SDA_Pin;

	/** Route of the SCL. Use defines by SiLabs (I2C_ROUTELOC0_SCLLOC_LOCXX - in 'efm32pg12b_i2c.h') */
	uint32_t SCL_RouteLoc;

	/** Route of the SDA. Use defines by SiLabs (I2C_ROUTELOC0_SDALOC_LOCXX - in 'efm32pg12b_i2c.h') */
	uint32_t SDA_RouteLoc;

} MyI2C_Struct;




/*******************************************************************************************************
 * @brief Sets up I2C bus communication.
 *
 * More specifically, this function:
 *      - Enables needed clocks
 *      - Defines and initializes the I2C structure
 *      - Toggles I2C SCL 9 times to reset any I2C slave that may require it
 *      - Routes the PINs
 *      - Reset the I2C bus
 *      - Enables wanted interrupts
 *      - Enables I2C IRQ Handler
 *
 * You must populate the structure @p MyI2C_Struct properly first. <b> Read the struct comments </b>
 *
 *
 * @param[in] I2CBusComm
 * 			Structure that contains all the wanted I2C bus settings
 *
 * @return
 * 			None
 *******************************************************************************************************/
void MyI2Cx_Init(MyI2C_Struct *I2CBusComm);



/*******************************************************************************************************
 * @brief Enables/disables SCL and SDA pins
 *
 *
 * @param[in] I2CBusComm
 * 			Structure that contains all the wanted I2C bus settings
 *
 * @param[in] Enable
 * 			Set to TRUE to enable, FALSE to disable
 *
 * @return
 * 			None
 *******************************************************************************************************/
void MyI2Cx_Enable_SCL_SDA(MyI2C_Struct *I2CBusComm, bool Enable);



/*******************************************************************************************************
 * @brief This helper function ensures that before we start an I2C transaction, our I2C bus and
 * registers are in a clear and known state. Call it at the beginning of any I2C driver function.
 *
 * @param[in] I2CBusComm
 * 			Structure that contains all the wanted I2C bus settings
 *
 * @return
 * 			None
 *******************************************************************************************************/
void Clear_I2Cx(MyI2C_Struct *I2CBusComm);



/*******************************************************************************************************
 * @brief Check if an ACK was received on the desired I2Cx bus. If received a true is returned and
 * the interrupt flag is cleared. Else, a false is returned.
 *
 * @param[in] I2CBusComm
 * 			Structure that contains all the wanted I2C bus settings
 *
 * @return
 * 			- TRUE => ACK received
 * 			- FALSE => No ACK received
 *******************************************************************************************************/
bool Check_ACK_I2Cx(MyI2C_Struct *I2CBusComm);



/*******************************************************************************************************
 * @brief Check if an RXDATAV flag was set on the desired I2Cx bus. If valid data is received a true is
 * returned and the interrupt flag is cleared. Else, a false is returned.
 *
 * @param[in] I2CBusComm
 * 			Structure that contains all the wanted I2C bus settings
 *
 * @return
 * 			- TRUE => RXDATAV received
 * 			- FALSE => No RXDATAV received
 *******************************************************************************************************/
bool Check_RXDATAV_I2Cx(MyI2C_Struct *I2CBusComm);



/*******************************************************************************************************
 * @brief Returns received data from the I2Cx bus. Must be called after Check_RXDATAV_I2Cx() to ensure
 * valid data was received from the I2C bus.
 *
 * @param[in] I2CBusComm
 * 			Structure that contains all the wanted I2C bus settings
 *
 * @return
 * 			uint8_t RX_Data => Byte received from I2C bus
 *******************************************************************************************************/
uint8_t RX_Byte_I2Cx(MyI2C_Struct *I2CBusComm);



/*******************************************************************************************************
 * @brief Check if a NACK was received on the desired I2Cx bus. If received a true is returned and
 * the interrupt flag is cleared. Else, a false is returned.
 *
 * @param[in] I2CBusComm
 * 			Structure that contains all the wanted I2C bus settings
 *
 * @return
 * 			- TRUE => NACK received
 * 			- FALSE => No NACK received
 *******************************************************************************************************/
bool Check_NACK_I2Cx(MyI2C_Struct *I2CBusComm);



/*******************************************************************************************************
 * @brief I2C0 Interrupt Handler
 *
 *
 * @param
 * 			None
 *
 * @return
 * 			None
 *******************************************************************************************************/
void I2C0_IRQHandler();



#endif /* MY_I2C_H_ */
