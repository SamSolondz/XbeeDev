
#include "ATCommandsLib.h"

#define DELIM_CHAR "\r"

/*Send an AT command over configured UART*/
/* "AT" + ASCII Command + Space(Optional) + Parameter (Optional, HEX) + Carriage Return"*/
/*Omit param to read a field*/
int sendATCommandWrite(const char * cmd, const char * param)
{
	char command[30] = "AT";
	strcat(command, cmd);
	if(param != NULL)
		strcat(command, param);
	strcat(command, DELIM_CHAR);

	LOG_INFO("Command sent = %%s\n", command);

	LEUART0_Putchar_n(command);

	return AT_OK;
}

void XbeeEnterCommandMode()
{
	XbeeWait(1050);	//wait 1 second
	LEUART0_Putchar_n("+++");
	//Wait one second after this is sent
	XbeeWait(1050);	//wait 1 second
	LOG_INFO("Done");
}


void XbeeExitCommandMode()
{
	sendATCommandWrite("CN", NULL);
}

void XbeeWait(int waitMs)
{
	waitForResp = true;
	timerSetEventInMs(waitMs);	//wait 1 second
	while(waitForResp);
}

void XbeeSetSleep()
{
	//sendATCommandWrite("SM", );
}

void XbeeAirplaneMode(bool AirplaneMode)
{
	if(AirplaneMode)
		sendATCommandWrite("AM", "0x01");
	else
		sendATCommandWrite("AM", "0x00");
}

void ParseXbeeRx(const char * RxCmd)
{

}

/*Call this function to write SMS parameters to Xbee memory*/
void XbeeSetupSMSSend(const char * phoneNumber)
{
	/*Ensure red associate light is steady blinking - connected to network*/
	XbeeEnterCommandMode();

	sendATCommandWrite("BD", "0x03");	//Set Xbee Baud to 9600
	sendATCommandWrite("WR", NULL);
	XbeeWait(500);

	sendATCommandWrite("IP", "0x02");	//Set Xbee to SMS protocol
	sendATCommandWrite("WR", NULL);
	XbeeWait(500);
	sendATCommandWrite("P#", phoneNumber);
	sendATCommandWrite("WR", NULL);
	XbeeWait(500);

//	sendATCommandWrite("TD", "0x0D"); //Text delimiter for transparent mode, 0x0D = is for carriage return
//	sendATCommandWrite("WR", NULL);
//	XbeeWait(500);

	sendATCommandWrite("RO", "0x00");
	sendATCommandWrite("WR", NULL);
	XbeeWait(500);


	sendATCommandWrite("AP", "0x00");
	XbeeWait(1000);

	XbeeExitCommandMode();
	//XbeeWait(1000);


}

void XbeeChangePH(const char * phoneNumber)
{
	XbeeEnterCommandMode();

	sendATCommandWrite("P#", phoneNumber);
	sendATCommandWrite("WR", NULL);
	XbeeWait(500);

	XbeeExitCommandMode();

}
