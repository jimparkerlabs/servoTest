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

#define TIMER_RESOLUTION 80

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
volatile unsigned long totalTime;
ISR(TIMER0_COMPA_vect) {
	if(trainTimer > 0) trainTimer -= TIMER_RESOLUTION;
	if(pulseTimer > 0) pulseTimer -= TIMER_RESOLUTION;
	totalTime += TIMER_RESOLUTION;
}

int main(void) {
	unsigned int period = 20000;
	unsigned int pulseWidth = 1500;
	unsigned int minPulseWidth = 600;
	unsigned int maxPulseWidth = 2400;

	unsigned int testingPulseStep = TIMER_RESOLUTION;

	// the general idea is to set up a timer to interrupt ever 10 us and count down
	//  these 2 timers in the ISR.  the main()loop wil check to see when they are 0
	//  and act accordingly.
	trainTimer = period;
	pulseTimer = pulseWidth;
	totalTime = 0;

	uint16_t elapsedSeconds = 0;
	// I seem to need about 100 clock cycles for the ISR to maintain accurate timing.
	// at 1 Mhz, that means I anly have about 0.01 ms (100 us) resuolution...
	// TODO:  figure out TIMER_RESOLUTION based on F_CPU
	// TODO:  figure out a prescaler / OCR value combination to give me the least power
	//        consumption for the given TIMER_RESOLUTION
	TCCR0A = 0b00000010 ;       // ctc mode
	TCCR0B = 0b00000001 ;       // divide by nothing, so ticks = microseconds
	TIMSK0 = (1 << OCIE0A) ;    // enable interrupt on match A
	OCR0A = TIMER_RESOLUTION;   // value to compare

	DDRD &= ~(1 << PD6);     // set button port to input
	PORTD |= (1 << PD6);     // enable pull-up resistor

	DDRD |= (1 << PD5);     // set servo port to output

	char serialCharacter;

	LED_DDR = 0xff;         // configure LED port
	LED_PORT = 0b00000000;  // all off

	initUSART();

	printString("\r\nHello world!\r\n");
	printString("press '+' and '-' to move servo\r\n");

	sei();
	while (1) {
		serialCharacter = getByte();
		if (serialCharacter == '+' || serialCharacter == '=') {
			pulseWidth += testingPulseStep;
			if (pulseWidth > maxPulseWidth) pulseWidth = maxPulseWidth;
			printString("new width:");
			printWord(pulseWidth);
			printString("\r\n");
		}
		if (serialCharacter == '-' || serialCharacter == '_') {
			pulseWidth -= testingPulseStep;
			if (pulseWidth < minPulseWidth) pulseWidth = minPulseWidth;
			printString("new width:");
			printWord(pulseWidth);
			printString("\r\n");
		}
		if (serialCharacter == 'x') {
			printWord(elapsedSeconds);
			printString("\r\n");
		}

		if(pulseTimer == 0) {pulseTimer = pulseWidth; PORTD &= (1 << PD5);}
		if(trainTimer == 0) {trainTimer = period; PORTD |= (1 << PD5);}

		if(totalTime >= 1000000) {
			totalTime = 0;
			elapsedSeconds += 1;
			LED_PORT ^= 0b11111111;  // toggle
		}

	}
	return(0);
}




