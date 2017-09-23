#include "usart.h"
#include "stm32f10x_it.h"

void USART1_Config(void) {
    //USART1_NVIC_Config();
    
    GPIO_InitTypeDef GPIO_InitStrucutre;
    USART_InitTypeDef USART_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitStrucutre.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStrucutre.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStrucutre.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStrucutre);
    
    GPIO_InitStrucutre.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStrucutre.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStrucutre);
    
    USART_ClockInitTypeDef USART_ClockInitStructure;
    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
    USART_ClockInit(USART1, &USART_ClockInitStructure);
    
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
    
    // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
}

void USART1_NVIC_Config(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USART1_IRQHandler(void) {
    char curChar;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        /*USART_SendData(USART1, '#');
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);*/
        curChar = USART_ReceiveData(USART1);
        USART_SendData(USART1, curChar);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

void USART1_SendCharSequence(const char * data) {
    int index = 0;
    while (data[index] != '\0') {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
        USART_SendData(USART1, data[index++]);
		}
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
}
