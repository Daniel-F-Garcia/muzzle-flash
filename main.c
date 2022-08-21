#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

// burst and interval lengths are measured in cycles, not shots. Each shot is 2 cycles

#define minBurst 8
#define maxBurst 24
#define minInterval 4
#define maxInterval 8

#define randBurst() (rand() % (maxBurst-minBurst))+ minBurst
#define randInterval() (rand() % (maxInterval-minInterval))+ minInterval

int main (void) {
	// 1 = output, 0 = input
	DDRA  = 0b00111111;
	DDRB  = 0b11111000;
	// All Off
	PORTA = 0b00000000; 
	PORTB = 0b00000000; 
	
	// 1 = Burst, 0 = Interval
	unsigned char mode = rand() % 32;
	// number of cycles remaining for the current mode
	unsigned char remaining[5] = {0, 0, 0, 0, 0, 0};
	// chanel to port mapping
	unsigned char* channelPortMap[5] = {&PINB, &PINB, &PINB, &PINA, &PINA};
	unsigned char channelPinMap[5] = {0,1,2,7,6};
	
    while(1)  {
		for (unsigned char i=0; i<5; i++) {
			unsigned char channelMode = _BV(i) & mode;
			if (channelMode && (remaining[i] % 2)==0 && (bit_is_set(*channelPortMap[i], channelPinMap[i]))) {	
				PORTA |= channelMode;
			} else {
				PORTA &= ~_BV(i);
			}
			
			if (remaining[i]==0) {
				if (channelMode) {
					remaining[i] = randInterval();
				} else {
					remaining[i] = randBurst();
				}
				mode ^=  _BV(i);
			} else {
				remaining[i] = remaining[i] -1;
			}
		}
		
		
		_delay_ms(40);
    }
	
	 return(0);
}
