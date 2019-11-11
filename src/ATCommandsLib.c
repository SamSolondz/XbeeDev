
#include "ATCommandsLib.h"

/*Send an AT command over configured UART*/
/* "AT" + ASCII Command + Space(Optional) + Parameter (Optional, HEX) + Carriage Return"*/
/*Omit param to read a field*/
int sendATCommandWrite(const char * cmd, const char * param)
{
	char command[30] = "AT";
	strcat(command, cmd);
	if(param != NULL)
		strcat(command, param);
	strcat(command, "\r");

	LOG_INFO("Command sent = %%s\n", command);

	LEUART0_Putchar_n(command);

	return AT_OK;
}

void XbeeEnterCommandMode()
{
	LEUART0_Putchar_n("+++");
	//Wait one second after this is sent
}

void XbeeExitCommandMode()
{
	sendATCommandWrite("CN", NULL);
}

void XbeeSetupSMSSend()
{
	XbeeEnterCommandMode();
	sendATCommandWrite("AP", "0x00");	//API Enable Transparent mode
	sendATCommandWrite("IP", "0x02");	//Set Xbee to SMS protocol

	const char phoneNumber[] = "7325705880";
	sendATCommandWrite("P#", phoneNumber);
	sendATCommandWrite("TD", "0x0D"); //Text delimiter for transparent mode, 0x0D = is for carriage return
	sendATCommandWrite("AC", NULL);


	  //Receive Packet SMS
}
