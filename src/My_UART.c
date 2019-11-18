/*
 * My_UART.c
 *
 *  # ALL INFO CAN BE FOUND IN THE .h FILE #
 */
#include "My_UART.h"


/*********************************************
 * Private Variables
 *********************************************/
//UART0:
volatile uint8_t UART0_TXC_Flag = 0;							//Stores TXC interrupt flag from UART0 IRQ
uint8_t UART0_StartFrame = 0;									//Start frame to start RXing after
uint8_t UART0_EndFrame = 0;										//End frame to stop RXing and process buffer
volatile uint8_t UART0_RX_Buffer[UART0_RX_BUFF_SIZE] = {0};		//RX buffer

//UART1:
volatile uint8_t UART1_TXC_Flag = 0;							//Stores TXC interrupt flag from UART1 IRQ
uint8_t UART1_StartFrame = 0;									//Start frame to start RXing after
uint8_t UART1_EndFrame = 0;										//End frame to stop RXing and process buffer
volatile uint8_t UART1_RX_Buffer[UART1_RX_BUFF_SIZE] = {0};		//RX buffer

//UART2:
volatile uint8_t UART2_TXC_Flag = 0;							//Stores TXC interrupt flag from UART2 IRQ
uint8_t UART2_StartFrame = 0;									//Start frame to start RXing after
uint8_t UART2_EndFrame = 0;										//End frame to stop RXing and process buffer
volatile uint8_t UART2_RX_Buffer[UART2_RX_BUFF_SIZE] = {0};		//RX buffer

//UART3:
volatile uint8_t UART3_TXC_Flag = 0;							//Stores TXC interrupt flag from UART3 IRQ
uint8_t UART3_StartFrame = 0;									//Start frame to start RXing after
uint8_t UART3_EndFrame = 0;										//End frame to stop RXing and process buffer
volatile uint8_t UART3_RX_Buffer[UART3_RX_BUFF_SIZE] = {0};		//RX buffer



bool UARTx_Init(MyUART_Struct *UARTComm)
{
	/* Enable needed clocks */
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_GPIO, true);

	/* Enable needed UART clock and set base memory location */
	switch (UARTComm->UART_Instance)
	{
		//UART0
		case 0:
			CMU_ClockEnable(cmuClock_USART0, true);
			UARTComm->UARTx = USART0;
			break;

		//UART1
		case 1:
			CMU_ClockEnable(cmuClock_USART1, true);
			UARTComm->UARTx = USART1;
			break;

		//UART2
		case 2:
			CMU_ClockEnable(cmuClock_USART2, true);
			UARTComm->UARTx = USART2;
			break;

		//UART3
		case 3:
			CMU_ClockEnable(cmuClock_USART3, true);
			UARTComm->UARTx = USART3;
			break;

		//Error! - Unknown UART instance
		default:
			return false;
	}

	/* To avoid false start, configure TX output as high */
	GPIO_PinModeSet(UARTComm->RX_Port, UARTComm->RX_Pin, gpioModeInput, 0);			//RX
	GPIO_PinModeSet(UARTComm->TX_Port, UARTComm->TX_Pin, gpioModePushPull, 1);		//TX

	/* Enable or disable UART after init? Use SiLabs enum */
	uint8_t UARTEnableParam = 0;
	if(UARTComm->EnableWhenDone)
	{
		UARTEnableParam = usartEnable;
	}

	/* Populate USART_InitAsync struct - Use default values from parameters not exposed by MyUART_Struct */
	USART_InitAsync_TypeDef UARTInit =
	{
			UARTEnableParam,       //Enable/Disable RX/TX when initialization is complete
			0,                     //Use current configured reference clock for configuring baud rate [default]
			UARTComm->BaudRate,
			UARTComm->Oversampling,
			UARTComm->DataBits,
			UARTComm->Parity,
			UARTComm->StopBits,
			false,                 //Do not disable majority vote [default]
			false,                 //Not USART PRS input mode [default]
			0,                     //PRS channel 0 [default]
			false,                 //Auto CS functionality enable/disable switch [default]
			0,                     //Auto CS Hold cycles [default]
			0,                     //Auto CS Setup cycles [default]
			usartHwFlowControlNone //No HW flow control [default]
	};

	/* Initialize UARTx */
	USART_InitAsync(UARTComm->UARTx, &UARTInit);

	/* Route and enable PINs */
	UARTComm->UARTx->ROUTELOC0 = UARTComm->RX_RouteLoc | UARTComm->TX_RouteLoc;
	UARTComm->UARTx->ROUTEPEN  = USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;

	/* Clear TX and RX buffers */
	UARTComm->UARTx->CMD = USART_CMD_CLEARTX | USART_CMD_CLEARRX;

	/* Clear and enable interrupts:
	 * TXC - Transmit Complete
	 * RXDATAV - Valid Data Received */
	UARTComm->UARTx->IFC = USART_IFC_TXC;
	UARTComm->UARTx->IEN = USART_IEN_TXC | USART_IEN_RXDATAV;

	/* Enable interrupts from UARTx module */
	switch (UARTComm->UART_Instance)
	{
		//UART0
		case 0:
			UART0_StartFrame = UARTComm->StartFrame;
			UART0_EndFrame = UARTComm->EndFrame;
			NVIC_EnableIRQ(USART0_RX_IRQn);
			NVIC_EnableIRQ(USART0_TX_IRQn);
			break;

		//UART1
		case 1:
			UART1_StartFrame = UARTComm->StartFrame;
			UART1_EndFrame = UARTComm->EndFrame;
			NVIC_EnableIRQ(USART1_RX_IRQn);
			NVIC_EnableIRQ(USART1_TX_IRQn);
			break;

		//UART2
		case 2:
			UART2_StartFrame = UARTComm->StartFrame;
			UART2_EndFrame = UARTComm->EndFrame;
			NVIC_EnableIRQ(USART2_RX_IRQn);
			NVIC_EnableIRQ(USART2_TX_IRQn);
			break;

		//UART3
		case 3:
			UART3_StartFrame = UARTComm->StartFrame;
			UART3_EndFrame = UARTComm->EndFrame;
			NVIC_EnableIRQ(USART3_RX_IRQn);
			NVIC_EnableIRQ(USART3_TX_IRQn);
			break;
	}

	return true;
}



void UARTx_Putchar(MyUART_Struct *UARTComm, char TX_Char)
{
	/* Error check: Makes sure the UART was initialized */
	EFM_ASSERT(UARTComm->UARTx != NULL);

	CORE_AtomicDisableIrq();				//Disables interrupts

	UARTComm->UARTx->TXDATA = TX_Char;		//Transmit a char

	CORE_AtomicEnableIrq();					//Enables interrupts
}



void UARTx_Putchar_n(MyUART_Struct *UARTComm, char* TX_String)
{
	/* Error check: Makes sure the UART was initialized */
	EFM_ASSERT(UARTComm->UARTx != NULL);

	uint16_t i;
	uint16_t Length = strlen(TX_String);				//Get the length of the string

	/* Call Putchar function to transmit one char at a time */
	for(i=0; i < Length; i++)
	{
		UARTx_Putchar(UARTComm, TX_String[i]);

		/* Wait until TX Complete Interrupt flag */
		while( !Check_TXC_UARTx(UARTComm) )
		{
			//Sleep maybe?
		}
	}
}



bool Check_TXC_UARTx(MyUART_Struct *UARTComm)
{
	/* Error check: Makes sure the UART was initialized */
	EFM_ASSERT(UARTComm->UARTx != NULL);

	/* Get the appropriate TXC interrupt */
	switch (UARTComm->UART_Instance)
	{
		//UART0
		case 0:
			if(UART0_TXC_Flag)
			{
				UART0_TXC_Flag = 0;
				return true;
			}
			break;

		//UART1
		case 1:
			if(UART1_TXC_Flag)
			{
				UART1_TXC_Flag = 0;
				return true;
			}
			break;

		//UART2
		case 2:
			if(UART2_TXC_Flag)
			{
				UART2_TXC_Flag = 0;
				return true;
			}
			break;

		//UART3
		case 3:
			if(UART3_TXC_Flag)
			{
				UART3_TXC_Flag = 0;
				return true;
			}
			break;
	}

	return false;
}



void USART0_RX_IRQHandler()
{
	uint32_t Flags;								//This will store any interrupt flags
	Flags = USART0->IF & USART0->IEN; 			//Get any pending interrupts that are enabled
	USART0->IFC = Flags;						//Clear interrupts

	uint8_t RX_Byte =  0;						//Stores the RXed byte
	static bool RX_Enabled = false;				//Flag that decides whether to RX or discard data
	static uint8_t RX_Buff_Index = 0; 			//Index used to keep track of what element we are adding a char into in our RX buffer


	/* RXDATAV Handler */
	if(Flags & USART_IF_RXDATAV)
	{
		RX_Byte = USART0->RXDATA;				//Get data

		/* Store in RX buffer if RX_Enabled is set */
		if(RX_Enabled)
		{
			/* Check if we got the End frame to stop RXing */
			if(RX_Byte == UART0_EndFrame)
			{
				RX_Enabled = false;
			}
			/* Add the data to the buffer */
			else
			{
				UART0_RX_Buffer[RX_Buff_Index] = USART0->RXDATA;	//Store received char
				RX_Buff_Index++;									//Increment buffer index
			}

			/* Error handling:
			 * If we reached the max size of our buffer, reset our index and
			 * overwrite older data */
			if(RX_Buff_Index == UART0_RX_BUFF_SIZE)
			{
				RX_Buff_Index = 0;
			}
		}
		/* Check if we got the Start frame to start RXing */
		else if(RX_Byte == UART0_StartFrame)
		{
			RX_Enabled = true;
		}
	}
}

void USART0_TX_IRQHandler()
{
	uint32_t Flags;								//This will store any interrupt flags
	Flags = USART0->IF & USART0->IEN; 			//Get any pending interrupts that are enabled
	USART0->IFC = Flags;						//Clear interrupts


	/* TXC Handler */
	if(Flags & USART_IF_TXC)
	{
		UART0_TXC_Flag = true;					//Set flag
	}
}






void USART1_RX_IRQHandler()
{
	uint32_t Flags;								//This will store any interrupt flags
	Flags = USART1->IF & USART1->IEN; 			//Get any pending interrupts that are enabled
	USART1->IFC = Flags;						//Clear interrupts

	uint8_t RX_Byte =  0;						//Stores the RXed byte
	static bool RX_Enabled = false;				//Flag that decides whether to RX or discard data
	static uint8_t RX_Buff_Index = 0; 			//Index used to keep track of what element we are adding a char into in our RX buffer


	/* RXDATAV Handler */
	if(Flags & USART_IF_RXDATAV)
	{
		RX_Byte = USART1->RXDATA;				//Get data

		/* Store in RX buffer if RX_Enabled is set */
		if(RX_Enabled)
		{
			/* Check if we got the End frame to stop RXing */
			if(RX_Byte == UART1_EndFrame)
			{
				RX_Enabled = false;
			}
			/* Add the data to the buffer */
			else
			{
				UART1_RX_Buffer[RX_Buff_Index] = USART1->RXDATA;	//Store received char
				RX_Buff_Index++;									//Increment buffer index
			}

			/* Error handling:
			 * If we reached the max size of our buffer, reset our index and
			 * overwrite older data */
			if(RX_Buff_Index == UART1_RX_BUFF_SIZE)
			{
				RX_Buff_Index = 0;
			}
		}
		/* Check if we got the Start frame to start RXing */
		else if(RX_Byte == UART1_StartFrame)
		{
			RX_Enabled = true;
		}
	}
}
void USART1_TX_IRQHandler()
{
	uint32_t Flags;								//This will store any interrupt flags
	Flags = USART1->IF & USART1->IEN; 			//Get any pending interrupts that are enabled
	USART1->IFC = Flags;						//Clear interrupts


	/* TXC Handler */
	if(Flags & USART_IF_TXC)
	{
		UART1_TXC_Flag = true;					//Set flag
	}
}


void USART2_RX_IRQHandler()
{
	uint32_t Flags;								//This will store any interrupt flags
	Flags = USART2->IF & USART2->IEN; 			//Get any pending interrupts that are enabled
	USART2->IFC = Flags;						//Clear interrupts

	uint8_t RX_Byte =  0;						//Stores the RXed byte
	static bool RX_Enabled = false;				//Flag that decides whether to RX or discard data
	static uint8_t RX_Buff_Index = 0; 			//Index used to keep track of what element we are adding a char into in our RX buffer


	/* RXDATAV Handler */
	if(Flags & USART_IF_RXDATAV)
	{
		RX_Byte = USART2->RXDATA;				//Get data

		/* Store in RX buffer if RX_Enabled is set */
		if(RX_Enabled)
		{
			/* Check if we got the End frame to stop RXing */
			if(RX_Byte == UART2_EndFrame)
			{
				RX_Enabled = false;
			}
			/* Add the data to the buffer */
			else
			{
				UART2_RX_Buffer[RX_Buff_Index] = USART2->RXDATA;	//Store received char
				RX_Buff_Index++;									//Increment buffer index
			}

			/* Error handling:
			 * If we reached the max size of our buffer, reset our index and
			 * overwrite older data */
			if(RX_Buff_Index == UART2_RX_BUFF_SIZE)
			{
				RX_Buff_Index = 0;
			}
		}
		/* Check if we got the Start frame to start RXing */
		else if(RX_Byte == UART2_StartFrame)
		{
			RX_Enabled = true;
		}
	}
}
void USART2_TX_IRQHandler()
{
	uint32_t Flags;								//This will store any interrupt flags
	Flags = USART2->IF & USART2->IEN; 			//Get any pending interrupts that are enabled
	USART2->IFC = Flags;						//Clear interrupts


	/* TXC Handler */
	if(Flags & USART_IF_TXC)
	{
		UART2_TXC_Flag = true;					//Set flag
	}
}


void USART3_RX_IRQHandler()
{
	uint32_t Flags;								//This will store any interrupt flags
	Flags = USART3->IF & USART3->IEN; 			//Get any pending interrupts that are enabled
	USART3->IFC = Flags;						//Clear interrupts

	uint8_t RX_Byte =  0;						//Stores the RXed byte
	static bool RX_Enabled = false;				//Flag that decides whether to RX or discard data
	static uint8_t RX_Buff_Index = 0; 			//Index used to keep track of what element we are adding a char into in our RX buffer


	/* RXDATAV Handler */
	if(Flags & USART_IF_RXDATAV)
	{
		RX_Byte = USART3->RXDATA;				//Get data

		/* Store in RX buffer if RX_Enabled is set */
		if(RX_Enabled)
		{
			/* Check if we got the End frame to stop RXing */
			if(RX_Byte == UART3_EndFrame)
			{
				RX_Enabled = false;
			}
			/* Add the data to the buffer */
			else
			{
				UART3_RX_Buffer[RX_Buff_Index] = USART3->RXDATA;	//Store received char
				RX_Buff_Index++;									//Increment buffer index
			}

			/* Error handling:
			 * If we reached the max size of our buffer, reset our index and
			 * overwrite older data */
			if(RX_Buff_Index == UART3_RX_BUFF_SIZE)
			{
				RX_Buff_Index = 0;
			}
		}
		/* Check if we got the Start frame to start RXing */
		else if(RX_Byte == UART3_StartFrame)
		{
			RX_Enabled = true;
		}
	}
}
void USART3_TX_IRQHandler()
{
	uint32_t Flags;								//This will store any interrupt flags
	Flags = USART3->IF & USART3->IEN; 			//Get any pending interrupts that are enabled
	USART3->IFC = Flags;						//Clear interrupts


	/* TXC Handler */
	if(Flags & USART_IF_TXC)
	{
		UART3_TXC_Flag = true;					//Set flag
	}
}




