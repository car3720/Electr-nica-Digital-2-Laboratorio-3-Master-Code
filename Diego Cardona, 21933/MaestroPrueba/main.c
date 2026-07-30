/*
 * MaestroPrueba.c
 *
 * Created: 30/7/2026 08:18:07
 * Author : crist
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "USART./USART.h"
#include "SPI/SPI.h"

void setup(void) {
	DDRB |= (1 << DDB0) | (1 << DDB1);   // PB0, PB1 como salida
	DDRD |= 0xFC;                        // D2–D7 como salida

	DDRC |= (1 << DDC0);   // SS en PC0
	PORTC |= (1 << PORTC0);
}

void refreshLEDs(uint8_t value) {
	PORTD = ((value & 0x3F) << 2);                 // D2–D7 controlados
	PORTB = (PORTB & 0xFC) | ((value >> 6) & 0x03); // B0–B1 controlados
}

uint8_t readPots(void) {
	PORTC &= ~(1 << PORTC0); // SS bajo
	_delay_us(10);

	SPI_transmit('p');               // Enviar comando
	uint8_t pot1 = SPI_receive();    // Leer Pot1

	SPI_transmit(0x00);              // Dummy transmit
	uint8_t pot2 = SPI_receive();    // Leer Pot2

	PORTC |= (1 << PORTC0); // SS alto

	char buffer[32];
	sprintf(buffer, "P1:%d P2:%d\n", pot1, pot2);
	writeTextUART(buffer);

	refreshLEDs(pot1);   // Maestro muestra Pot1

	return pot1;
}

int main(void) {
	setup();
	SPI_init(SPI_MASTER, SPI_MSB_FIRST, SPI_CLOCK_IDLE_LOW, SPI_SAMPLE_ON_LEADING);
	initUART9600();

	writeTextUART("Sistema Maestro listo\n");
	writeTextUART("Envie:\n- 'p' para potenciometros\n- 0-255 para LEDs\n");

	while(1) {
		if(UCSR0A & (1 << RXC0)) {
			uint8_t data = UDR0;

			if(data == 'p') {
				readPots();
				} else {
				refreshLEDs(data);

				PORTC &= ~(1 << PORTC0);
				_delay_us(10);
				SPI_transmit(data);
				PORTC |= (1 << PORTC0);

				writeTextUART("LED:");
				char num[4];
				itoa(data, num, 10);
				writeTextUART(num);
				writeTextUART("\n");
			}
		}
	}
}