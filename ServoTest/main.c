/*
 * main.c
 *
 *  Created on: Jun 14, 2015
 *      Author: jimparker
 */

#include <avr/io.h>
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

int main(void) {
	int period = 20000;
	int width = 1500;
	int step = 10;

	DDRD &= ~(1 << PD6);    // set button port to input
	PORTD |= (1 << PD6);  // enable pull-up resistor

	DDRD &= ~(1 << PD5);    // set servo port to output

	char serialCharacter;

	LED_DDR = 0xff;
	LED_PORT = 0b01010101;

	initUSART();
	transmitByte(0x0d);  //  cr
	transmitByte(0x0a);  // lf
	printString("Hello world!");
	transmitByte(0x0d);  //  cr
	transmitByte(0x0a);  // lf
	printString("press '+' and '-' to move servo");
	transmitByte(0x0d);  //  cr
	transmitByte(0x0a);  // lf

	while (1) {
		serialCharacter = getByte();
		if (serialCharacter == '+') {
			width += step;
			printString("new width: ");
			//printString(itoa(width));
			transmitByte(0x0d);  //  cr
			transmitByte(0x0a);  // lf
		}
		if (serialCharacter == '-') {
			width -= step;
			printString("new width: ");
			//printString(itoa(width));
			transmitByte(0x0d);  //  cr
			transmitByte(0x0a);  // lf
		}

		PORTD |= (1 << PD5);  // start pulse
		_delay_us(width);
		PORTD &= ~(1 << PD5);    // end pulse
		_delay_us(period - width);
	}
	return(0);
}




