#include "delay.h"
#include "stm32f10x_it.h"
uint16_t TIM6_cnt;
uint16_t TIM7_cnt;

/*
TIM6 configurations.
Generate time delay of microseconds.
*/
void TIM6_RCC_Config(void);
void TIM6_NVIC_Config(void);
void TIM6_Config(void);

void TIM6_RCC_Config(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
}
void TIM6_NVIC_Config(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);
}
void TIM6_Config(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    
    TIM_TimeBaseInitStructure.TIM_Period = 71; // 1 us between interruptions
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);
    
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
}

/*
TIM7 configurations.
Generate time delay of milliseconds.
*/
void TIM7_RCC_Config(void);
void TIM7_NVIC_Config(void);
void TIM7_Config(void);

void TIM7_RCC_Config(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
}
void TIM7_NVIC_Config(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);
}
void TIM7_Config(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    
    TIM_TimeBaseInitStructure.TIM_Period = 7199; // 0.1 ms between interruptions
    TIM_TimeBaseInitStructure.TIM_Prescaler = 9;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStructure);
    
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
}

// Delay functions.
void DelayInit(void) 
{	
		TIM6_RCC_Config();
    TIM6_NVIC_Config();
    TIM6_Config();
    TIM7_RCC_Config();
    TIM7_NVIC_Config();
    TIM7_Config();
}

void Delay_us(uint16_t us) 
{
    TIM6_cnt = us + 1;
    
    TIM_Cmd(TIM6, ENABLE);
    while (TIM6_cnt > 0);
    TIM_Cmd(TIM6, DISABLE);
}
// Interruption handler.
void TIM6_IRQHandler(void) {
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    if (TIM6_cnt > 0) {
        TIM6_cnt--;
    }
}

void Delay_ms(uint16_t ms) 
{
    TIM7_cnt = ms + 1;
    
    TIM_Cmd(TIM7, ENABLE);
    while (TIM7_cnt > 0);
    TIM_Cmd(TIM7, DISABLE);
}
// Interruption handler.
void TIM7_IRQHandler(void) {
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    if (TIM7_cnt > 0) {
        TIM7_cnt--;
    }
}
