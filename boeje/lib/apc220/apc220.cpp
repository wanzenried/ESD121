#include <apc220.h>

//Constructor (also calls the constructor for softwareserial)
softAPC::softAPC(int rxPin, int txPin, int en, int set, int aux, uint8_t id) :
    SoftwareSerial(rxPin, txPin)
{
    _EN = en;
    pinMode(_EN, OUTPUT);
    _SET = set;
    pinMode(_SET, OUTPUT);
    _AUX = aux;
    pinMode(_AUX, INPUT);
    _ID = id;
}

//use only for startup as this function has a built in 100ms delay
//If you want to change baud rate use the config function instead
void softAPC::begin(long speed)
{
    digitalWrite(_EN, HIGH);
    digitalWrite(_SET, HIGH);
    delay(100); //time to let the module start up. double the value in the datasheet
    
    SoftwareSerial::begin(speed);
}

int softAPC::config(uint32_t freq, uint8_t airBaud, uint8_t power, uint8_t baud, uint8_t parity)
{
    //Return an error message, and abort, if any of the numbers are too long
    int err = 0;
    if (freq > 455000)
    {
        err += 1;
    }
    if (airBaud > 4)
    {
        err += 2;
    }
    if (power > 9)
    {
        err += 4;
    }
    if (baud > 6)
    {
        err += 8;
    }
    if (parity > 2)
    {
        err += 16;
    }

    if (err > 0)
    {
        return err;
    }

    int newBaud = calcBaudFromParam(baud);

    char cmd[15]; 
    char rpl[20]; //should be the same length as cmd, but just in case

    sprintf(cmd, "%ld %d %d %d %d", freq, airBaud, power, baud, parity); // Create formatted string from the supplied values and write it to cmd

    SoftwareSerial::begin(9600); // set baudrate to setting rate (from datasheet)

    wrConfig(cmd, rpl);

    if(strcmp(cmd, rpl) == 0)   // if reply and command are the samme, the settings was saved
    {
        SoftwareSerial::begin(newBaud); // set new baud rate
        return 0;
    }
    else
    {
        return 32;
    }

}

// calculates the baud rate based on the parameter supplied
// from the datasheet following values are valid: 1200 2400 4800 9600 19200 38400 57600
//starting from n = 0 the first 6 can be calculated with the formula 1200 * 2^n
//the last allowed value doesnt follow the formula, so it is hardcoded
int softAPC::calcBaudFromParam(uint8_t param)
{
    if (param > 6)
    {
        return -1;
    }

    if (param == 6)
    {
        return 57600;
    }

    return (1200 << param); //bit shifting instead of multiplying with 2 raised to a power. This has the same result, but much faster

}

//sets the module to "settings mode" then uploads new settings. the board will reply with the settings it stored, these will be returnen in *reply
//Also trims the response so it is on the following form "FFFFFF R P B C" (14 bytes + string terminator)
void softAPC::wrConfig(char *cmd, char *reply)
{
    enterSetMode();

    write("WR ");
    write(cmd);
    write("\r\n");

    delay(235); // 200 millisecond pr the datasheet + 35 for time to actually send the data
    char tmpReply[25];
    char incomingChar;
    int replyLen = 0;

    while(available()) //read the reply
    {
        incomingChar = (char) read();
        tmpReply[replyLen] = incomingChar;
        replyLen++;
    }
    exitSetMode();

    sprintf(reply, "%.14s", tmpReply + 5); //only put the subset we care about into the reply string

    
}

//get current settings from the module
// response is on the following form: "PARA FFFFFF R P B C" (19 bytes + string terminator)
void softAPC::rdConfig(char *reply)
{
    enterSetMode();

    write("RD\r\n");

    delay(235); // 200 millisecond pr the datasheet + 35 for time to actually send the data
    char incomingChar;
    int replyLen = 0;

    while(available()) //read the reply
    {
        incomingChar = (char) read();
        reply[replyLen] = incomingChar;
        replyLen++;
    }
    exitSetMode();

    reply[replyLen-2] = 0x00; //apc sends back "\r\n" which we want to remove 

    
}


void softAPC::enterSetMode()
{
    digitalWrite(_SET, LOW);
    delay(1);   //from datasheet
}

void softAPC::exitSetMode()
{
    digitalWrite(_SET, HIGH);
    delay(10);  //from datasheet
}


// Send a message over the radio, i dont think messages should be longer than 100 bytes at most, but maybe we can squeze more through if we need it
void softAPC::sendMsg(uint8_t recipientId, byte *msg, uint8_t msgLen)
{
    uint8_t transmissionLen = msgLen + 2;  // length of msg + 1 byte for sender id + 1 byte for reciever id

    write('[');               // specifies start of message
    write(transmissionLen);
    write(recipientId);
    write(_ID);
    write(msg, msgLen);       // send message, this way of doing it does not care if there is 0x00 chars in the middle of msg
    write(']');               // specifies end of message
}

//  try to recieve a full message.
//  messages will have the following format(disregard '_'):
//  [Len_rID_sID_data]
//  returns 2 if incomplete message
//  returns 1 if message is not intended for this id
//  otherwise returns 0
int softAPC::recieveMsg(byte *buffer, uint8_t *bufferLen)
{
    //  Check for start char
    if (read() != '[')
    {
        return 2;
    }
    delay(2);   //delay so we dont try to read faster than the radio can recieve
    
    //  minimum transmissionlen is 2 (sender id + recipient id)
    int transmissionLen = read();
    if (transmissionLen < 2)
    {
        return 2;
    }
    delay(2);

    uint8_t recvId = read();
    transmissionLen--;
    if (recvId != _ID)
    {
        return 1;
    }
    delay(2);

    *bufferLen = readBytes(buffer, transmissionLen);

    delay(2);

    if (read() == ']')
    {
        return 0;
    }
    else
    {
        return 2;
    }
}