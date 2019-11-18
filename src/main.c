/***************************************************************************//**
 * @file
 * @brief LEUART/LDMA in EM2 example for SLSTK3402A starter kit
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_leuart.h"
#include "em_letimer.h"
#include "em_ldma.h"
#include "bspconfig.h"
#include "retargetserial.h"
#include "My_LEUART.h"
#include "My_Sleep.h"
#include "My_CMU.h"
#include "Global_Defines.h"
#include "ATCommandsLib.h"
#include "letimer.h"
#include "My_Logger.h"
#include "My_UART.h"


#define XBEE_SLEEP_RQ_9		()

/***************************************************************************//**
 * @brief  Main function
 ******************************************************************************/

volatile bool TXC_Flag = false;				//This global variable is used to set a flag whenever a TXC interrupt is received (used in LEUART0_TX_String function)
volatile uint8_t RX_Index = 0;				//Index used to keep track of what element we are adding a char into in our RX buffer
volatile char RX_Buffer[RX_BUFF_SIZE];		//Buffer used to store RXed data from the BLE module

volatile bool waitForResp = false;

volatile uint8_t EnergyMode_Counter[5];		//Used to keep track of the energy modes being blocked
volatile uint8_t SchedulerEvent = 0;		//Stores scheduler events to execute

int main(void)
{
  /* Chip errata */
  CHIP_Init();
  /* Initialize Clocks */
  CMU_Init();

  /*Initialize LETIMER*/
  LETIMER0_Setup(5000); 	//1000 ms underflow period
  /* Initialize LEUART */
  LEUART0_Setup();

  Block_Energy_Mode(SYSTEM_ENERGY_MODE);


  Log_Msg_UART_Init();
  Log_Msg_UART("Main", INFO, "BT logging");

  //XbeeSetupSMSSend("4087661874");

  int flag = 0;
  while (1) {
	 // Log_Msg_UART("Main", INFO, "BT logging");
	  /*If we have no events to execute, go to sleep */
	  if(SchedulerEvent == 0)
		  Enter_Sleep();

	  //XbeeChangePH("4087661874");
	  if(SchedulerEvent & TIMER_UF)
	  {
		  char snum[20];
		  int timestamp = loggerGetTimestamp();
		  // convert 123 to string [buf]
		  itoa(timestamp, snum, 10);


		  //LEUART0_Putchar_n(snum);
		  //LEUART0_Putchar_n("\r");

		  CORE_AtomicDisableIrq();				//Disable interrupts
		  SchedulerEvent &= ~TIMER_UF;
		  CORE_AtomicEnableIrq();					//Enable interrupts
	  }


		/*Parse Xbee response in RX Buffer */
	  	if(SchedulerEvent & DECODE_RX_BUFFER)
	  	{
	  			// Store the RXed data into a local buffer, then reset our RX buffer - make it atomic
	  			char Resp_Buff[RX_BUFF_SIZE];
	  			CORE_AtomicDisableIrq();				//Disable interrupts
	  			// Loop and copy elements based on the index
	  			for(uint8_t i = 0; i < RX_Index; i++)
	  			{
	  				Resp_Buff[i] = RX_Buffer[i];
	  			}
	  			Resp_Buff[RX_Index] = '\0';				//End the string
	  			RX_Index = 0;							//Reset RX index
	  			CORE_AtomicEnableIrq();					//Enable interrupts



	  			/*Process the RESP string*/
	  			if(strcmp(Resp_Buff, "ERROR\r") == 0)
	  			{
	  				 Log_Msg_UART("Main", ERROR, "Unsuccessful Xbee Command");
	  			}
  				if(strcmp(Resp_Buff, "OK\r") == 0)
				{
	  				Log_Msg_UART("Main", INFO, "Xbee: Command OK");
				}
  				else	//Parse User SMS Command
  				{
  					Log_Msg_UART("Main", INFO, Resp_Buff);
  				}



	  			// Clear the event – making it atomic
	  			CORE_AtomicDisableIrq();				//Disable interrupts
	  			SchedulerEvent &= ~DECODE_RX_BUFFER;
	  			LEUART0->CMD = LEUART_CMD_RXBLOCKDIS;		//Stop blocking RX
	  			while (LEUART0->SYNCBUSY);
	  			CORE_AtomicEnableIrq();					//Enable interrupts
	  		}



  }
}
