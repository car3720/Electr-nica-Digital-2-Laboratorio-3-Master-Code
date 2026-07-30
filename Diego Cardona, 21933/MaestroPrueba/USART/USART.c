/*
 * USART.c
 *
 * Created: 30/7/2026 08:25:22
 *  Author: crist
 */ 
#include "USART.h"

void initUART9600(void) {
	// Configurar pines
	DDRD |= (1 << DDD1); // TX como salida
	DDRD &= ~(1 << DDD0); // RX como entrada
	
	// Configurar UART
	UCSR0A = (1 << U2X0); // Modo rápido
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Habilitar TX y RX
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits de datos
	
	UBRR0 = 207; // 9600 bauds con U2X0
}

void writeUART(char character) {
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = character;
}

void writeTextUART(char* text) {
	for(uint8_t i = 0; text[i] != '\0'; i++) {
		writeUART(text[i]);
	}
}

unsigned char read_UART(void) {
	while(!(UCSR0A & (1 << RXC0)));  // espera a que llegue dato
	return UDR0;                     // devuelve el carácter recibido
}
