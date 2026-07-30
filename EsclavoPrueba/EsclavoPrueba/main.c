/*
 * EsclavoPrueba.c
 *
 * Created: 30/7/2026 08:27:23
 * Author : crist
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include "SPI/SPI.h"
#include "ADC/ADC.h"

void setup(void);
void refreshLEDs(uint8_t value);

int main(void) {
	setup();
	SPI_init(SPI_SLAVE, SPI_MSB_FIRST, SPI_CLOCK_IDLE_LOW, SPI_SAMPLE_ON_LEADING);
	SPI_enable_interrupt();
	sei();

	while(1) {
		// Todo se maneja en la interrupción SPI
	}
}

void setup(void) {
	DDRB |= (1 << DDB0) | (1 << DDB1);   // PB0, PB1 como salida
	DDRD |= 0xFC;                        // D2–D7 como salida

	DDRB &= ~(1 << DDB2);   // SS esclavo en D10
	PORTB |= (1 << PORTB2);

	ADC_init();
	
	// Inicialización: todos apagados
	PORTB &= ~0x03;   // apaga B0–B1
	PORTD &= ~0xFC;   // apaga D2–D7
}

void refreshLEDs(uint8_t value) {
	PORTD = ((value & 0x3F) << 2);                 // D2–D7 controlados
	PORTB = (PORTB & 0xFC) | ((value >> 6) & 0x03); // B0–B1 controlados
}

ISR(SPI_STC_vect) {
	static uint8_t sendSecond = 0;
	uint8_t received = SPDR;

	if(received == 'p') {
		uint8_t pot1 = (ADC_read(2) >> 2); // A2
		SPDR = pot1;
		refreshLEDs(pot1);   // LEDs muestran Pot1
		sendSecond = 1;
	}
	else if(sendSecond) {
		uint8_t pot2 = (ADC_read(3) >> 2); // A3
		SPDR = pot2;
		refreshLEDs(pot2);   // LEDs muestran Pot2
		sendSecond = 0;
	}
	else {
		refreshLEDs(received); // cualquier otro dato ? LEDs
		SPDR = received;
	}
}
