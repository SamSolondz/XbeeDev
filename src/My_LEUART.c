/*
 * My_LEUART.c
 *
 *  # ALL INFO CAN BE FOUND IN THE .h FILE #
 */

#include "My_LEUART.h"

#include <string.h>
#include "em_cmu.h"
#include "em_leuart.h"
#include "em_core.h"
#include "My_Sleep.h"
#include "Global_Defines.h"


//Global Variables:
extern bool TXC_Flag;
extern uint8_t RX_Index;
extern char RX_Buffer[RX_BUFF_SIZE];
extern uint8_t SchedulerEvent;

void LEUART0_Setup()
{
	/* Enable needed clocks:
	 * Select LFXO as a source LFB which goes to LEUART (and wait for it to stabilize) */
	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
	CMU_ClockEnable(cmuClock_LEUART0, true);

	/* To avoid false start, configure TX output as high */
	GPIO_PinModeSet(LEUART0_RX_PORT, LEUART0_RX_PIN, gpioModeInput, LEUART0_RX_DEFAULT);			//RX
	GPIO_PinModeSet(LEUART0_TX_PORT, LEUART0_TX_PIN, gpioModePushPull, LEUART0_TX_DEFAULT);			//TX

	LEUART_Init_TypeDef LEUARTInit =
	{
		leuartDisable,			/* Disable RX/TX when initialization completed. */
		0,						/* Use current configured reference clock for configuring baud rate.*/
		9600,					/* 9600 bits/s. */
		leuartDatabits8,		/* 8 data bits. */
		leuartNoParity,			/* No parity. */
		leuartStopbits1			/* 1 stop bit. */
	};

	/* Initialize LEUART */
	LEUART_Init(LEUART0, &LEUARTInit);

	/* Route and enable PINs */
	LEUART0->ROUTELOC0 = LEUART_ROUTELOC0_TXLOC_LOC18 | LEUART_ROUTELOC0_RXLOC_LOC18;
	LEUART0->ROUTEPEN  = LEUART_ROUTEPEN_TXPEN | LEUART_ROUTEPEN_RXPEN;

	/* Set the START and SIG chars */
	LEUART0->STARTFRAME = '?';
	while (LEUART0->SYNCBUSY);

	LEUART0->SIGFRAME = '#';
	while (LEUART0->SYNCBUSY);

	/* Clear TX and RX buffers - Block all incoming frames (only STARTFRAME will pass!) */
	LEUART0->CMD = LEUART_CMD_CLEARTX | LEUART_CMD_CLEARRX | LEUART_CMD_RXBLOCKEN;
	while (LEUART0->SYNCBUSY);

	/* Set SFUBRX to allow the START frame character to pass through and clear
	 * the RXBLOCK */
	LEUART0->CTRL |= LEUART_CTRL_SFUBRX;
	while (LEUART0->SYNCBUSY);

	/* Clear and enable interrupts */
	LEUART0->IFC = LEUART_IFC_TXC | LEUART_IFC_STARTF;
	LEUART0->IEN = LEUART_IEN_TXC | LEUART_IEN_STARTF;

	/* Enable interrupts from LEUART module */
	NVIC_EnableIRQ(LEUART0_IRQn);

	/* Finally enable LEUART module */
	LEUART_Enable(LEUART0, leuartEnable);
}



void LEUART0_Putchar(char TX_Char)
{
	CORE_AtomicDisableIrq();			//Disables interrupts

	LEUART0->TXDATA = TX_Char;			//Transmit a char

	CORE_AtomicEnableIrq();				//Enables interrupts
}



void LEUART0_Putchar_n(char* TX_String)
{
	Block_Energy_Mode(EM3);

	uint16_t i;
	uint16_t Length = strlen(TX_String);	//Get the length of the string


	for(i=0; i < Length; i++)
	{
		LEUART0_Putchar(TX_String[i]);		//Call our Putchar function to transmit one char at a time

		/* Wait until TX Complete Interrupt flag */
		while(!TXC_Flag)
		{
			Enter_Sleep();
		}
		TXC_Flag = false;					//Clear set flag
	}

	Unblock_Energy_Mode(EM3);
}



void LEUART0_IRQHandler(void)
{
	uint32_t Flags;								//This will store any interrupt flags
	Flags = LEUART0->IF & LEUART0->IEN; 		//Get any pending interrupts that are enabled
	LEUART0->IFC = Flags;						//Clear interrupts

	/* TXC Handler */
	if(Flags & LEUART_IF_TXC)
	{
		TXC_Flag = true;						//Set flag
	}

	/* STARTF Handler */
	if(Flags & LEUART_IF_STARTF)
	{
		LEUART0->IEN |= LEUART_IEN_RXDATAV;		//Start frame received, start RXing data
		LEUART0->IEN |= LEUART_IEN_SIGF;		//Enable SIGF
	}

	/* RXDATAV Handler */
	if(Flags & LEUART_IF_RXDATAV)
	{
		RX_Buffer[RX_Index] = LEUART0->RXDATA;	//Store received char
		RX_Index++;								//Increment global buffer index

		/* Error handling:
		 * If we reached the max size of our buffer, reset our index and
		 * overwrite older data */
		if(RX_Index > RX_BUFF_SIZE)
		{
			RX_Index = 0;
		}
	}

	/* SIGF Handler */
	if(Flags & LEUART_IF_SIGF)
	{
		LEUART0->CMD = LEUART_CMD_RXBLOCKEN;
		while (LEUART0->SYNCBUSY);
		LEUART0->IEN &= ~LEUART_IEN_RXDATAV;
		LEUART0->IEN &= ~LEUART_IEN_SIGF;
		SchedulerEvent |= DECODE_RX_BUFFER;
	}
}
