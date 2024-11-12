#include <apc220.h>


softAPC::softAPC(int rxPin, int txPin, int en, int set, int aux) :
    SoftwareSerial(rxPin, txPin)

{
    _EN = en;
    pinMode(_EN, OUTPUT);
    _SET = set;
    pinMode(_SET, OUTPUT);
    _AUX = aux;
    pinMode(_AUX, INPUT);
}

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

    sprintf(cmd, "%ld %d %d %d %d", freq, airBaud, power, baud, parity);

    wrConfig(cmd, rpl);

    if(strcmp(cmd, rpl) == 0)
    {
        return 0;
    }
    else
    {
        return 32;
    }

}

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

    return (1200 << param);

}

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

    while(available())
    {
        incomingChar = (char) read();
        tmpReply[replyLen] = incomingChar;
        replyLen++;
    }
    exitSetMode();

    sprintf(reply, "%.14s", tmpReply + 5); //only put the subset we care about into the reply string

    
}

void softAPC::rdConfig(char *reply)
{
    enterSetMode();

    write("RD\r\n");

    delay(235); // 200 millisecond pr the datasheet + 35 for time to actually send the data
    char incomingChar;
    int replyLen = 0;

    while(available())
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
    delay(1);
}

void softAPC::exitSetMode()
{
    digitalWrite(_SET, HIGH);
    delay(10);
}