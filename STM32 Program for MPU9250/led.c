#include "stm32f10x.h"
#include "led.h"
void Init_Led()
{
   GPIO_InitTypeDef GPIO_InitStruct; 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;   
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;  
   GPIO_Init(GPIOB, &GPIO_InitStruct); 
   GPIO_SetBits(GPIOB, GPIO_Pin_5);
}
void Led_ON()
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}
void Led_OFF()
{
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
}
