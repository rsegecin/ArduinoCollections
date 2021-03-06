#ifndef SerialInterpreter_h
#define SerialInterpreter_h

#define SERIAL_TX		PD1
#define DEF_MSG_SIZE	80

#include <avr/io.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "Communs.h"

struct sSerialCommand {
	char * Name;
	void(*ExecFunction)(void) = nullptr;
};

enum eSerialState
{
    Ready, PCommand, PParameters, Done
};

class SerialInterpreterClass
{
public:
	SerialInterpreterClass(sSerialCommand * pSerialCommands, int pNumberOfCommands);

    bool MessageReady = false;		        // Serial message's flag
    void(*ExecFunction)(void) = nullptr;	// Tells what command should be executed
    char MessageBuffer[DEF_MSG_SIZE];       // Buffer from parameters passed in the message

    void ClearBuffer(void);
    char *GetParameter(unsigned char pIndex);
    void Send(const char *s);
    void OnInterrupt(char pChar);
    void SetPreamble(bool pValue);

private:
    sSerialCommand * SerialCommands = nullptr;
    int NumberOfCommands = 0;

    eSerialState ProcessingState = eSerialState::Ready;
    int MatchingCmd = -1;
    int MsgPosition = 0;
    bool flagD = false;
    bool flagDA = false;
    bool Preamble = false;                  // Indicates wether serial communication precedes with the preamble 0d 0a

    void SetInitialState();
    void SetCommandState();
    void SetParametersState();
    void SetDoneState();

    void CheckPreamble(char pChar);
    bool CommandExists(bool pAllMatch, char pChar);
    void ProcessCommand(char pChar);
    void ProcessParameters(char pChar);

    void usart_tx(char c);
};

#endif