#include <slave/radio.h>


char rInput[50];
uint8_t rLen;

const uint8_t ID = 1; //this device id

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
        if (radio.read() != '[')
        {
            break;
        }

        if (recieveMsg(rInput, &rLen) == 0)     //recieved a complete message, for us
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


int recieveMsg(char *buffer, uint8_t *bufferLen)
{

    int transmissionLen = radio.read();

    uint8_t recvId = radio.read();      //first byte of transmission is the id of the device that is supposed to recieve the message
    transmissionLen--;


    if(recvId != ID)
    {
        return 1;
    }


    *bufferLen = radio.readBytes(buffer,transmissionLen);

    buffer[*bufferLen] = '\0';
    


    if (radio.read() == ']')
    {
        return 0;
    }
    else
    {
        return 2;
    }
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