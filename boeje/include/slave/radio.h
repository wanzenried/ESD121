#include <Arduino.h>
#include <apc220.h>

int beginRadio();
int recieveCommand();
void sendMsg(uint8_t recipientId, byte *msg, uint8_t msgLen);
int recieveMsg(char *buffer, uint8_t *bufferLen);
int decode();

//APC pins
#define APC_EN  7  //[output] HIGH enable, LOW disable
#define APC_RXD 8  //connected to TX on board
#define APC_TXD 9  //connected to RX on board
#define APC_AUX 10   //[input] HIGH when receiving data, else low
#define APC_SET 11   //[output] HIGH for normal function, LOW to change settings