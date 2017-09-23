#include "stm32f10x.h"
#include "I2C.h"
#include "usart.h"
#include "led.h"
#include "delay.h"
#include "stdio.h"
#include "main.h"

int main()
{
	Sys_Init();
	while (1)
	{
		MPU_Measure();
		MPU_Calc();
		Led_ON();//to show the transmission
		USART1_SendCharSequence(str);
		Led_OFF();
	}
}

void Sys_Init(void)
{
	Init_Led();// Initilize the led
	USART1_Config();// Config the usart1 port
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
	Led_ON();//to show the transmission
	USART_SendData(USART1, 'a');//Shwo that the program begins.
	Led_OFF();
	DelayInit();
	I2C_Initi();
	I2C_Initi1();
	I2C_Initi2();
	Init_MPU();//We must init adxl345 before init I2C bus
	Init_MPU1();
	Init_MPU2();
}

void MPU_Measure(void)
{
	gx1l = SingleRead(MPUADDR,REG_DATA_GX_LOW);
	gx1h = SingleRead(MPUADDR,REG_DATA_GX_HIGH);
	gy1l = SingleRead(MPUADDR,REG_DATA_GY_LOW);
	gy1h = SingleRead(MPUADDR,REG_DATA_GY_HIGH);
	gz1l = SingleRead(MPUADDR,REG_DATA_GZ_LOW);
	gz1h = SingleRead(MPUADDR,REG_DATA_GZ_HIGH);

	wx1l = SingleRead(MPUADDR,REG_DATA_WX_LOW);
	wx1h = SingleRead(MPUADDR,REG_DATA_WX_HIGH);
	wy1l = SingleRead(MPUADDR,REG_DATA_WY_LOW);
	wy1h = SingleRead(MPUADDR,REG_DATA_WY_HIGH);
	wz1l = SingleRead(MPUADDR,REG_DATA_WZ_LOW);
	wz1h = SingleRead(MPUADDR,REG_DATA_WZ_HIGH);

	SingleWrite(MPUADDR,REG_MAG_CONFIG,0x02);
	Delay_ms(10);
	SingleWrite(MAGADDR,REG_MAG_MODE,0x01);
	Delay_ms(10);

	bx1l = SingleRead(MAGADDR,REG_DATA_BX_LOW);
	bx1h = SingleRead(MAGADDR,REG_DATA_BX_HIGH);
	by1l = SingleRead(MAGADDR,REG_DATA_BY_LOW);
	by1h = SingleRead(MAGADDR,REG_DATA_BY_HIGH);
	bz1l = SingleRead(MAGADDR,REG_DATA_BZ_LOW);
	bz1h = SingleRead(MAGADDR,REG_DATA_BZ_HIGH);


	gx2l = SingleRead1(MPUADDR,REG_DATA_GX_LOW);
	gx2h = SingleRead1(MPUADDR,REG_DATA_GX_HIGH);
	gy2l = SingleRead1(MPUADDR,REG_DATA_GY_LOW);
	gy2h = SingleRead1(MPUADDR,REG_DATA_GY_HIGH);
	gz2l = SingleRead1(MPUADDR,REG_DATA_GZ_LOW);
	gz2h = SingleRead1(MPUADDR,REG_DATA_GZ_HIGH);

	wx2l = SingleRead1(MPUADDR,REG_DATA_WX_LOW);
	wx2h = SingleRead1(MPUADDR,REG_DATA_WX_HIGH);
	wy2l = SingleRead1(MPUADDR,REG_DATA_WY_LOW);
	wy2h = SingleRead1(MPUADDR,REG_DATA_WY_HIGH);
	wz2l = SingleRead1(MPUADDR,REG_DATA_WZ_LOW);
	wz2h = SingleRead1(MPUADDR,REG_DATA_WZ_HIGH);

	SingleWrite1(MPUADDR,REG_MAG_CONFIG,0x02);
	Delay_ms(10);
	SingleWrite1(MAGADDR,REG_MAG_MODE,0x01);
	Delay_ms(10);
	
	bx2l = SingleRead1(MAGADDR,REG_DATA_BX_LOW);
	bx2h = SingleRead1(MAGADDR,REG_DATA_BX_HIGH);
	by2l = SingleRead1(MAGADDR,REG_DATA_BY_LOW);
	by2h = SingleRead1(MAGADDR,REG_DATA_BY_HIGH);
	bz2l = SingleRead1(MAGADDR,REG_DATA_BZ_LOW);
	bz2h = SingleRead1(MAGADDR,REG_DATA_BZ_HIGH);
	
}

void MPU_Calc(void)
{
	gx1h <<= 8; 	wx1h <<= 8; 	bx1h <<= 8;
	gy1h <<= 8; 	wy1h <<= 8; 	by1h <<= 8;
	gz1h <<= 8; 	wz1h <<= 8; 	bz1h <<= 8;

	gx2h <<= 8; 	wx2h <<= 8; 	bx2h <<= 8;
	gy2h <<= 8; 	wy2h <<= 8; 	by2h <<= 8;
	gz2h <<= 8; 	wz2h <<= 8; 	bz2h <<= 8;

	gx1 = (gx1h + gx1l) / (double)256;
	gy1 = (gy1h + gy1l) / (double)256;
	gz1 = (gz1h + gz1l) / (double)256;
	wx1 = (wx1h + wx1l) / (double)256;
	wy1 = (wy1h + wy1l) / (double)256;
	wz1 = (wz1h + wz1l) / (double)256;
	bx1 = (bx1h + bx1l) / (double)256;
	by1 = (by1h + by1l) / (double)256;
	bz1 = (bz1h + bz1l) / (double)256;

	gx2 = (gx2h + gx2l) / (double)256;
	gy2 = (gy2h + gy2l) / (double)256;
	gz2 = (gz2h + gz2l) / (double)256;
	wx2 = (wx2h + wx2l) / (double)256;
	wy2 = (wy2h + wy2l) / (double)256;
	wz2 = (wz2h + wz2l) / (double)256;
	bx2 = (bx2h + bx2l) / (double)256;
	by2 = (by2h + by2l) / (double)256;
	bz2 = (bz2h + bz2l) / (double)256;

	sprintf(str, "%5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f\n",gx1,gy1,gz1,wx1,wy1,wz1,bx1,by1,bz1,gx2,gy2,gz2,wx2,wy2,wz2,bx2,by2,bz2);
}
