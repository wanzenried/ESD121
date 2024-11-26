#include <slave/radio.h>


char rInput[50];
uint8_t rLen;

const uint8_t ID = 1; //this device id

softAPC radio(APC_TXD, APC_RXD, APC_EN, APC_SET, APC_AUX);

int beginRadio()
{
    // start and configure radio
    radio.begin(9600);
    //Serial.println(radio.getTimeout());
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

    //Serial.println("Recieved start of transmission");

    int transmissionLen = radio.read();

    //Serial.print("Transmission is of length ");
    //Serial.println(transmissionLen);

    uint8_t recvId = radio.read();      //first byte of transmission is the id of the device that is supposed to recieve the message
    transmissionLen--;

    //Serial.print("Transmission is intended for device id: ");
    //Serial.println(recvId);

    if(recvId != ID)
    {
        Serial.println("Transmission is not intended for me");
        return 1;
    }


    *bufferLen = radio.readBytes(buffer,transmissionLen);

    //Serial.print("bytes recieved: ");
    //Serial.println(rLen);

    buffer[*bufferLen] = '\0';
    


    if (radio.read() == ']')
    {
        //Serial.println("Recieved end of transmission");
        return 0;
    }
    else
    {
        //Serial.println("Sumting wrogn");
        return 2;
    }
}

cmd decode()
{
    uint8_t sendId = rInput[0];

    //Serial.print("Sender id: ");
    //Serial.println(sendId);

    if(rInput[1] == 'v' && rInput[2] == 'd')
    {
        return VARDMP;
    }
    else if (rInput[1] == 'r' && rInput[2] == 's')
    {
        return RESEND;
    }
    else
    {
    //Serial.print("msg: ");
    //Serial.println(rInput+1);
    }
    return NOCMD;


}


// Send a message over the radio, i dont think messages should be longer than 100 bytes at most, but maybe we can squeze more through if we need it
void sendMsg(uint8_t recipientId, char *msg, uint8_t msgLen)
{
    
    uint8_t transmissionLen = msgLen + 2;  // length of msg + 1 byte for sender id + 1 byte for reciever id

    radio.write('[');               // specifies start of message
    radio.write(transmissionLen);
    radio.write(recipientId);
    radio.write(ID);
    radio.write(msg, msgLen);       // send message, this way of doing it does not care if there is 0x00 chars in the middle of msg
    radio.write(']');               // specifies end of message
    
}