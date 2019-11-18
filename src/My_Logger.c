/*
 * My_Logger.c
 *
 *  # ALL INFO CAN BE FOUND IN THE .h FILE #
 */
#include "My_Logger.h"


/*********************************************
 * Private Variables
 *********************************************/
/* UART0 settings struct for BT module logging */
MyUART_Struct BT_LoggerUART =
{
		.UART_Instance = 0,				//Use UART0
		.UARTx = NULL, 					//This will be auto-populated later
		.BaudRate = 9600, 				//Baud rate
		.DataBits = usartDatabits8,		//No. of data bits
		.EnableWhenDone = true,

		.Oversampling = usartOVS16,		//Used default setting (16x)
		.Parity = usartNoParity,		//Parity
		.StopBits = usartStopbits1, 	//No. of stop bits

		.RX_Port = BT_RX_PORT,
		.RX_Pin = BT_RX_PIN,
		.RX_RouteLoc = BT_RX_LOC,
		.TX_Port = BT_TX_PORT,
		.TX_Pin = BT_TX_PIN,
		.TX_RouteLoc = BT_TX_LOC,

		.StartFrame = '?',
		.EndFrame = '#'
};

/* Log Level strings: Used in Log_Msg_UART() to modify the output log based on LogLevel_Enum */
char LogLevel_Str[5][10] =
{
		"INFO",
		"WARNING",
		"ERROR",
		"CRITICAL",
		"FATAL"
};

volatile uint32_t Ticks = 0;	//Stores the number of total ticks from Systick
volatile uint32_t Secs = 0;		//Stores the number of seconds elapsed



void Log_Msg_UART_Init()
{
	/* Initialize UART0 to connect to the BT module for logging */
	EFM_ASSERT( UARTx_Init(&BT_LoggerUART) == true );

	/* Configure Systick to interrupt every 1ms - used for log timing */
	EFM_ASSERT( SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000) == 0 );
}



void Log_Msg_UART(char *Src, uint8_t LogLvl, char* Msg)
{
	/* Error check: Makes sure the UART was initialized */
	EFM_ASSERT(BT_LoggerUART.UARTx != NULL);

	/* Variable to store current monotonic time */
	char CurrTime[14];

	/* Get current monotonic time and store in string */
	CORE_AtomicDisableIrq();									//Disables interrupts as we are accessing a private global variable
	snprintf(CurrTime, 14, "%u.%03u ms", Secs, Ticks);
	CORE_AtomicEnableIrq();										//Enables interrupts

	/* The log message is sent by segments depending on
	 * what needs to be sent or formatting */

	/* Logs current monotonic time */
	UARTx_Putchar_n(&BT_LoggerUART, "[");
	UARTx_Putchar_n(&BT_LoggerUART, CurrTime);
	UARTx_Putchar_n(&BT_LoggerUART, "] ");

	/* Logs source of log (inception) */
	UARTx_Putchar_n(&BT_LoggerUART, Src);

	UARTx_Putchar_n(&BT_LoggerUART, " (");

	/* Check if log level is defined */
	if( (LogLvl != INFO) && (LogLvl != WARNING) && (LogLvl != ERROR) && (LogLvl != CRITICAL) && (LogLvl != FATAL) )
	{
		UARTx_Putchar_n(&BT_LoggerUART, "Unknown LogLvl?");
	}
	else
	{
		UARTx_Putchar_n(&BT_LoggerUART, LogLevel_Str[LogLvl]);	//LogLevel
	}

	UARTx_Putchar_n(&BT_LoggerUART, "): ");

	/* Logs message */
	UARTx_Putchar_n(&BT_LoggerUART, Msg);
	UARTx_Putchar_n(&BT_LoggerUART, "\n");
}



void SysTick_Handler()
{
	/* Increment private variable */
	Ticks++;

	/* If ticks reached 1 ms, increments the Secs variable and clear Ticks */
	if(Ticks == 1000)
	{
		Secs++;
		Ticks = 0;
	}
}
