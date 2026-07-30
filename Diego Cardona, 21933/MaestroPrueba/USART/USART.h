/*
 * USART.h
 *
 * Created: 30/7/2026 08:25:37
 *  Author: crist
 */ 



#ifndef USART_H
#define USART_H

#include <avr/io.h>
#include <stdint.h>

void initUART9600(void);
void writeUART(char character);
void writeTextUART(char* text);
unsigned char read_UART(void);

#endif