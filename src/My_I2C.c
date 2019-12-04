/*
 * My_I2C.c
 *
 *  # ALL INFO CAN BE FOUND IN THE .h FILE #
 */
#include "My_I2C.h"


/*********************************************
 * Private Variables
 *********************************************/
volatile uint8_t I2C0_ACK_Flag = 0;			//Stores ACK interrupt flag from I2C0
volatile uint8_t I2C0_RXDATAV_Flag = 0;		//Stores RXDATAV interrupt flag from I2C0
volatile uint8_t I2C0_RX_Data = 0;			//Stores received byte from I2C0
volatile uint8_t I2C0_NACK_Flag = 0;		//Stores NACK interrupt flag from I2C0



void MyI2Cx_Init(MyI2C_Struct *I2CBusComm)
{
	/* Get the I2Cx location - helps reduce repeated code */
	if(I2CBusComm->I2CBus != 0)
	{
		return;
	}
	else
	{
		I2CBusComm->I2CX = I2C0;
	}

	/* Enables needed clocks */
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_GPIO, true);

	CMU_ClockEnable(cmuClock_I2C0, true);



	/* Define and Init local I2C structure */
	I2C_Init_TypeDef I2CInit =
	{
			I2CBusComm->EnableWhenDone,		//Enable/Disable when initialization done
			I2CBusComm->SetToMaster,    	//Set to master mode
			I2CBusComm->RefClock,       	//Reference clock to use
			I2CBusComm->BusFreq, 			//I2C bus frequency to use
			I2CBusComm->ClockRatio    		//Clock low/high ratio control
	};

	/* Initializing the I2C */
	I2C_Init(I2CBusComm->I2CX, &I2CInit);


	/* Toggle I2C SCL 9 times to reset any I2C slave that may require it. After this sequence of operations,
	 * both the I2C slave and master (Pearl Gecko) state machines will be in the same state, IDLE*/
	for(uint8_t i = 0; i < 9; i++)
	{
		GPIO_PinOutClear(I2CBusComm->SCL_Port, I2CBusComm->SCL_Pin);
		GPIO_PinOutSet(I2CBusComm->SCL_Port, I2CBusComm->SCL_Pin);
	}

	/* Route I2C PINS */
	I2CBusComm->I2CX->ROUTELOC0 = I2CBusComm->SCL_RouteLoc | I2CBusComm->SDA_RouteLoc;
	I2CBusComm->I2CX->ROUTEPEN = I2C_ROUTEPEN_SCLPEN | I2C_ROUTEPEN_SDAPEN;


	/* Reset the I2C bus -> Exit the busy state.  The I2C will be in this state out of RESET */
	if (I2CBusComm->I2CX->STATE & I2C_STATE_BUSY)
	{
		I2CBusComm->I2CX->CMD = I2C_CMD_ABORT;
	}

	/* Clear any pending interrupts from I2C module */
	NVIC_ClearPendingIRQ(I2C0_IRQn);


	/* Enable ACK, NACK and RXDATAV interrupts */
	I2CBusComm->I2CX->IEN = I2C_IF_ACK | I2C_IF_NACK | I2C_IF_RXDATAV;

	/* Enable I2C IRQ Handler */
	NVIC_EnableIRQ(I2C0_IRQn);


	MyI2Cx_Enable_SCL_SDA(I2CBusComm, true);
}



void MyI2Cx_Enable_SCL_SDA(MyI2C_Struct *I2CBusComm, bool Enable)
{
	if(Enable)
	{
		GPIO_PinModeSet(I2CBusComm->SCL_Port, I2CBusComm->SCL_Pin, gpioModeWiredAnd, 1);		//SCL
		GPIO_PinModeSet(I2CBusComm->SDA_Port, I2CBusComm->SDA_Pin, gpioModeWiredAnd, 1);		//SDA
	}
	else
	{
		GPIO_PinModeSet(I2CBusComm->SCL_Port, I2CBusComm->SCL_Pin, gpioModeDisabled, 1);		//SCL
		GPIO_PinModeSet(I2CBusComm->SDA_Port, I2CBusComm->SDA_Pin, gpioModeDisabled, 1);		//SDA
	}
}



void Clear_I2Cx(MyI2C_Struct *I2CBusComm)
{
	/* Error check: Makes sure the I2C bus was initialized */
	EFM_ASSERT(I2CBusComm->I2CX != NULL);

	/* Forces clearing the I2C from any pending CMDs */
	if (I2CBusComm->I2CX->STATE & I2C_STATE_BUSY)
	{
		I2CBusComm->I2CX->CMD = I2C_CMD_ABORT;
	}
	/* Ensure buffers are empty */
	I2CBusComm->I2CX->CMD = I2C_CMD_CLEARPC | I2C_CMD_CLEARTX;
	while(I2CBusComm->I2CX->STATUS & I2C_STATUS_RXDATAV)
	{
		I2CBusComm->I2CX->RXDATA;
	}
}



bool Check_ACK_I2Cx(MyI2C_Struct *I2CBusComm)
{
	/* Error check: Makes sure the I2C bus was initialized */
	EFM_ASSERT(I2CBusComm->I2CX != NULL);

	/* Check for the ACK flag */
	if( I2C0_ACK_Flag )
	{
		I2C0_ACK_Flag = 0;		//Clear the INT flag and return true
		return true;
	}
	else
	{
		return false;			//No ACK yet, return false
	}
}



bool Check_RXDATAV_I2Cx(MyI2C_Struct *I2CBusComm)
{
	/* Error check: Makes sure the I2C bus was initialized */
	EFM_ASSERT(I2CBusComm->I2CX != NULL);

	/* Check for the RXDATAV flag */
	if( I2C0_RXDATAV_Flag )
	{
		I2C0_RXDATAV_Flag = 0;		//Clear the INT flag and return true
		return true;
	}
	else
	{
		return false;			//No RXDATAV yet, return false
	}
}



uint8_t RX_Byte_I2Cx(MyI2C_Struct *I2CBusComm)
{
	/* Error check: Makes sure the I2C bus was initialized */
	EFM_ASSERT(I2CBusComm->I2CX != NULL);

	uint8_t Byte = 0;				//Makes a local copy of the private variable

	Byte = I2C0_RX_Data;		//Copy private variable data
	I2C0_RX_Data = 0;			//Clear private variable
	return Byte;				//return byte

}



bool Check_NACK_I2Cx(MyI2C_Struct *I2CBusComm)
{
	/* Error check: Makes sure the I2C bus was initialized */
	EFM_ASSERT(I2CBusComm->I2CX != NULL);

	/* Check for the NACK flag */
	if( I2C0_NACK_Flag )
	{
		I2C0_NACK_Flag = 0;		//Clear the INT flag and return true
		return true;
	}
	else
	{
		return false;			//No NACK, return false
	}
}



void I2C0_IRQHandler()
{
	uint32_t Flags;							//This will store any interrupt flags
	Flags = I2C0->IF & I2C0->IEN;			//Get any pending interrupts
	I2C0->IFC = Flags;						//Clear interrupts


	/* ACK Handler */
	if(Flags & I2C_IF_ACK)
	{
		I2C0_ACK_Flag = 1;
	}

	/* RXDATAV Handler */
	if(Flags & I2C_IF_RXDATAV)
	{
		I2C0_RXDATAV_Flag = 1;
		I2C0_RX_Data = I2C0->RXDATA;
	}

	/* NACK Handler */
	if(Flags & I2C_IF_NACK)
	{
		I2C0_NACK_Flag = 1;
	}
}


