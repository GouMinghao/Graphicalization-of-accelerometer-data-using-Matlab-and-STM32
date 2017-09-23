#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x.h"

void USART1_Config(void);

void USART1_NVIC_Config(void);

void USART1_SendCharSequence(const char * data);

#endif
