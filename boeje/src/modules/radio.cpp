#include <modules/radio.h>


byte rInput[50];
uint8_t rLen;

#ifndef ID
#define ID 1 //default ID if id is not set through platformio
#endif

softAPC radio(APC_TXD, APC_RXD, APC_EN, APC_SET, APC_AUX, ID);

int beginRadio()
{
    // start and configure radio
    radio.begin(9600);
    return radio.config(433000, 3, 9, 3, 0);
}

cmd recieveCommand(uint8_t *senderID)
{
    while (radio.available() > 0)
    {
        if (radio.recieveMsg(rInput, &rLen) == 0)     //recieved a complete message, for us
        {
            *senderID = rInput[0];  //first byte is always sender id
            return decode();
        }
        else    //something is wrong, not for us, or incomplete msg
        {
            break;
        }

    }
    return NOCMD;
}

cmd decode()
{
    uint8_t sendId = rInput[0];

    if(rInput[1] == 'v' && rInput[2] == 'd')
    {
        return VARDMP;
    }
    else if (rInput[1] == 'r' && rInput[2] == 's')
    {
        return RESEND;
    }
    return NOCMD;


}

void sendMsg(uint8_t recipientId, byte *msg, uint8_t msgLen)
{
    radio.sendMsg(recipientId, msg, msgLen);
}