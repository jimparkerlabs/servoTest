/*
 * main.c
 *
 *  Created on: Jun 14, 2015
 *      Author: jimparker
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"

// button-handling routines
uint8_t buttonPressed(void) {
	static uint8_t buttonState;
	uint8_t pressed = 0;
	if ((PIND & (1 << PD6)) == 0) {
		_delay_ms(2);
		if ((PIND & (1 << PD6)) == 0)
			pressed=1;
	}
	buttonState = pressed;
	return pressed;
}

uint8_t newButtonPress(void) {
	static uint8_t buttonState;
	uint8_t pressed = 0;
	if ((PIND & (1 << PD6)) == 0) {
		_delay_ms(2);
		if ((PIND & (1 << PD6)) == 0)
			pressed=1;
	}
	uint8_t oldState = buttonState;
	buttonState = pressed;
	return ((pressed == 1) && (oldState == 0)); // pressed now, not before
}

volatile int trainTimer;
volatile int pulseTimer;
volatile int totalTime;
ISR(TIMER0_COMPA_vect) {
	trainTimer -=10;
	pulseTimer -=10;
	totalTime += 10;
}

int main(void) {
	int period = 20000;
	int pulseWidth = 1500;
	int testingPulseStep = 10;

	// the general idea is to set up a timer to interrupt ever 10 us and count down
	//  these 2 timers in the ISR.  the main()loop wil check to see when they are 0
	//  and act accordingly.
	trainTimer = period;
	pulseTimer = pulseWidth;
	totalTime = 0;

	uint16_t elapsedTime = 0;
	// at 8 Mhz, a prescaler of /8 and a compare value =10 should work.  Although, this might be too
	// fast for me to process. 80 clock cycles is pretty small...
	TCCR0A = 0b00000010 ; // ctc mode
	TCCR0B = 0b00000010 ; // divide by 8
	TIMSK0 = (1 << OCIE0A) ; // enable interrupt on match
	OCR0A = 10;

	DDRD &= ~(1 << PD6);    // set button port to input
	PORTD |= (1 << PD6);  // enable pull-up resistor

	DDRD &= ~(1 << PD5);    // set servo port to output

	char serialCharacter;

	LED_DDR = 0xff;         // configure LED port
	LED_PORT = 0b00000000;  // all off

	initUSART();

	printString("\r\nHello world!\r\n");
	printString("press '+' and '-' to move servo\r\n");

	sei();
	while (1) {
		serialCharacter = getByte();
		if (serialCharacter == '+') {
			pulseWidth += testingPulseStep;
			//printString("new width: %i\r\n", pulseWidth);
		}
		if (serialCharacter == '-') {
			pulseWidth -= testingPulseStep;
			//printString("new width: %i\r\n", pulseWidth);
		}

		if(pulseTimer == 0) {pulseTimer = pulseWidth; PORTD &= (1 << PD5);}
		if(trainTimer == 0) {trainTimer = period; PORTD |= (1 << PD5);}

		if(totalTime == 1000000) {
			totalTime = 0;
			elapsedTime += 1;
			printWord(elapsedTime);
			printString("\r\n");
			LED_PORT ^= 0x11111111;  // toggle
		}

	}
	return(0);
}




