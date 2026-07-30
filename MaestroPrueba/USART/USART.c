//***************************************************************************
// Universidad del Valle de Guatemala
// IE3054: Electrónica Digital 2
// Autores: Diego Cardona 21933 & Cristian Juárez 22478
// Proyecto: Laboratorio 3, Comunicación SPI
// Hardware: Atmega328P
// Created: 23/07/2026 17:49:31
//***************************************************************************
// Laboratorio 3 - Comunicación SPI
// USART
//***************************************************************************

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

/*
 * Lee una secuencia de dígitos ASCII desde el UART y los convierte
 * a un valor numérico real (0-255). Termina al recibir Enter (\r o \n).
 */
uint8_t leerNumeroUART(void) {
	uint16_t valor = 0;     // uint16_t para detectar overflow antes de castear a uint8_t
	unsigned char c;

	while(1) {
		c = read_UART();  // espera y lee un caracter

		// Si es Enter, terminamos de leer el número
		if(c == '\r' || c == '\n') {
			break;
		}

		// Solo procesamos si es un dígito ASCII ('0' a '9')
		if(c >= '0' && c <= '9') {
			valor = (valor * 10) + (c - '0'); // acumula el dígito, "corriendo" el número a la izquierda

			// Si se pasa de 255, lo saturamos para no hacer overflow al castear
			if(valor > 255) {
				valor = 255;
			}
		}
		// Cualquier otro caracter (letras, espacios) se ignora silenciosamente
	}

	return (uint8_t)valor;
}
