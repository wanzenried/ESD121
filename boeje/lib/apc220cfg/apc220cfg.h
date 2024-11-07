/**
 *   (C) 2021
 * 
 *   Jens Dalsgaard Nielsen <jdn@es.aau.dk>
 *   http://es.aau.dk/staff/jdn
 *   http://jensd.dk
 *   Section of Automation & Control
 *   Aalborg University,
 *   Denmark
 * 
 *   "THE BEER-WARE LICENSE" (frit efter PHK)
 *   <jdn@es.aau.dk> wrote this file. As long as you
 *   retain this notice you can do whatever you want
 *   with this stuff, but You shall add your
 *   name and email and date for your
 *   modification.
 * 
 *   If we meet some day, and you think
 *   this stuff is worth it ...
 *   you can buy me a beer in return :-)
 *   or if you are real happy then ...
 *   single malt will be well received :-)
 * 
 *   Use it at your own risk - no warranty
 * 
 **/
#ifndef _APC220CFG_H
#define _APC220CFG_H

#include <SoftwareSerial.h>

 
class apc220cfg  {
	
public:
	
	apc220cfg(SoftwareSerial *s, int setPin) 
	{
		_setPin = setPin;
		sps = s;
		fl = 1;  // indicate its sw serial
 		pinMode(_setPin, OUTPUT);
	};
	
	apc220cfg(HardwareSerial *s, int setPin) 
	{
		_setPin = setPin;
		sph = s;
		fl = 0;   // indicate its hw serial
		// begin(9600);
		pinMode(_setPin, OUTPUT);
	};
	
	int rdCfg(char *reply)
	{
		char rd;
		int cnt = 0;
		
		apc220ToSetMode();
		if (fl) {
			sps->write("RD\r\n");
			delay(200 + 30); // 200 datasheet - 30 then all chars has arrived
			while (sps->available()) {
				rd =  (char) sps->read();
				*(reply+cnt) = rd;
				cnt++;
			}
		}
		else {
			sph->write("RD\r\n");
			delay(200 + 30); // 200 datasheet - 30 then all chars has arrived
			while (sph->available()) {
				rd =  (char) sph->read();
				*(reply+cnt) = rd;
				cnt++;
			}
		}
 
		apc220ToNormalMode();

		// reply format
		// PARA xxxxxx x x x x\r\n"  == 21
		// 1234567890123456789 0 1 == 21 

		*(reply+cnt-2) = 0x00; // terminate string remove \r\n
		
 		// check if reply i valid  - not 100% safe but ok I think
		if ( (cnt == 21) 
			&& (reply[0] == 'P') 
			&& (reply[1] == 'A')
			&& (reply[2] == 'R')
			&& (reply[3] == 'A')
		){
			return 0;
		}
		else {
			reply[0] = 'E';
			reply[1] = reply[2] =  'R';
			reply[3] = 0x00;
			return -1;			
		}
	}
	
	void wrCfg(const char *cmd)
	{
		char rd;
		
		// we get 412000 3 9 3 0
		//        01234567890123
		apc220ToSetMode();
		if (fl) {
			sps->write("WR ");
			sps->write(cmd);
 			sps->write("\r\n");  // expected from apc220
			delay(200 + 35); // 200 datasheet - add 35 so all chars has arrived
			while (sps->available()) {
				rd =  (char) sps->read(); // clear reply
			}
		}
		else {
			sph->write("WR ");
			sph->write(cmd);
 			sph->write("\r\n");  // expected from apc220
			delay(200 + 35); // 200 datasheet - add 35 so all chars has arrived
			while (sph->available()) {
				rd =  (char) sph->read(); // clear reply
			}
		}
		
		apc220ToNormalMode();
	}
	
private:
	void apc220ToSetMode(void)
	{
		digitalWrite(_setPin, LOW);
		delay(10); // 2acc datasheet (1msec)
	};
	
	void apc220ToNormalMode()
	{
		delay(10);
		digitalWrite(_setPin, HIGH);
		delay(2); // acc datasheet (1msec)
	};
	
protected:
	HardwareSerial *sph;
	SoftwareSerial *sps;
	int fl;
	int _setPin;
};


#endif /* not defined _APC220CONFIG_H */


/*
 *  Some specs for apc220
 * 
 *  - Frequency: 415MHz to 455MHz (1kHz step +-100Hz)
 *  - GFSK modulation
 *  - Channel spacing 200KHz(at least JDN)
 *  - Max Output power: 20mW (10 level adjustable)
 *  - Sensitivity: -117dB@1200bps
 *  - Air data rate: 1200 to 19200bps.
 *  - UART baud rate: 1200 to 19200bps.
 *  - Serial parity: 8E1\/8N1\/8O1
 *  - Data buffer: 512bytes
 *  - Humidity: 10%ï½ž90%
 *  - Temperature: -30â„ƒ - 85â„ƒ
 *  - Supply voltage: 3.4 â€“ 5.5V (the ripple is Â±50mV )
 *  - Transmit current: 35mA
 *  - Receiving current:32mA
 *  - Sleeping current: 5uA
 *  - RF line-in-sight range:1000m (2400 baud)
 * 
 */



