/*
 * ADC.h
 *
 * Created: 30/7/2026 08:30:06
 *  Author: crist
 */ 

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

void ADC_init(void);
uint16_t ADC_read(uint8_t channel);

#endif /* ADC_H_ */