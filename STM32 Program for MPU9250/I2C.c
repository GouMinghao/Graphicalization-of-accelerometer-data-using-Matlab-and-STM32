#include "I2C.h"
#include "delay.h"
#include "led.h"
#include "usart.h"

void Init_MPU(void)
{
   SingleWrite(MPUADDR,REG_PWR_MNG,0x00);//unlock
   SingleWrite(MPUADDR,SMPLRT_DIV,0x07);//What is it used for?
   SingleWrite(MPUADDR,REG_ACC_CONFIG,0x06);//Begin the data record
   SingleWrite(MPUADDR,REG_GYR_CONFIG,0x06);
   SingleWrite(MPUADDR,REG_GYR_RANGE,0x18);
   SingleWrite(MPUADDR,REG_ACC_RANGE,0x01);
   SingleWrite(MPUADDR,REG_MAG_CONFIG,0x02);
   SingleWrite(MAGADDR,REG_MAG_MODE,0x01);
}

void I2C_Initi(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE ); //This must be changed when change the port of the I2C interface
   GPIO_InitStructure.GPIO_Pin = PIN_SCL | PIN_SDA;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;       //????
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

void I2C_Start(void)
{
   SDA_OUT();
   GPIO_SetBits(I2C_PORT,PIN_SDA);  //SDA =1
   Delay_us(10);
   GPIO_SetBits(I2C_PORT,PIN_SCL);  //SCL=1
   Delay_us(10);
   GPIO_ResetBits(I2C_PORT,PIN_SDA);//SDA=0
   Delay_us(10);
   GPIO_ResetBits(I2C_PORT,PIN_SCL);//SCL=0
   Delay_us(10);
}

void I2C_Stop(void)
{
   SDA_OUT();
   GPIO_ResetBits(I2C_PORT,PIN_SDA);
   Delay_us(10);
   GPIO_SetBits(I2C_PORT,PIN_SCL);
   Delay_us(10);
   GPIO_SetBits(I2C_PORT,PIN_SDA);
   Delay_us(10);
}

u8 I2C_Wait_Ack(void)
{
   u8 ucErrTime=0;
   SDA_IN();      //SDA?????
   GPIO_SetBits(I2C_PORT,PIN_SDA);
   Delay_us(5);
   GPIO_SetBits(I2C_PORT,PIN_SCL);
   Delay_us(5);
   while(GPIO_ReadInputDataBit(I2C_PORT,PIN_SDA))
   {
      ucErrTime++;
      if(ucErrTime>250)
      {
         I2C_Stop();
         return 1;
      }
   }
   GPIO_ResetBits(I2C_PORT, PIN_SCL);
   Delay_us(5);
   return 0;
}

void I2C_Ack(void)
{
   SDA_OUT();
   GPIO_ResetBits(I2C_PORT,PIN_SDA);
   Delay_us(5);
   GPIO_SetBits(I2C_PORT,PIN_SCL);
   Delay_us(5);
   GPIO_ResetBits(I2C_PORT,PIN_SCL);
}

void I2C_NAck(void)
{
   GPIO_ResetBits(I2C_PORT,PIN_SCL);
   SDA_OUT();
   GPIO_SetBits(I2C_PORT,PIN_SDA);
   Delay_us(5);
   GPIO_SetBits(I2C_PORT,PIN_SCL);
   Delay_us(5);
   GPIO_ResetBits(I2C_PORT,PIN_SCL);
}

void I2C_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    GPIO_ResetBits(I2C_PORT,PIN_SCL);//??????????
    for(t=0;t<8;t++)
    {
        if((txd&0x80)==0x80)
          {GPIO_SetBits(I2C_PORT, PIN_SDA);}//SDA=1,? 1
        else
          {GPIO_ResetBits(I2C_PORT, PIN_SDA);}//SDA=0,? 0
        txd<<=1;
        Delay_us(5);   //?TEA5767??????????
        GPIO_SetBits(I2C_PORT,PIN_SCL);
        Delay_us(5);
        GPIO_ResetBits(I2C_PORT,PIN_SCL);
        Delay_us(5);
    }
}

u8 I2C_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();
    GPIO_SetBits(I2C_PORT,PIN_SDA);
    for(i=0;i<8;i++ )
    {
        GPIO_ResetBits(I2C_PORT,PIN_SCL);
        Delay_us(5);
        GPIO_SetBits(I2C_PORT,PIN_SCL);
        Delay_us(5);
        receive<<=1;
        if(GPIO_ReadInputDataBit(I2C_PORT,PIN_SDA))
        {
            receive++;
        }
        Delay_us(5);
    }
    if (!ack)
        I2C_NAck();//??nACK
    else
        I2C_Ack(); //??ACK
    return receive;
}

void SingleWrite(u8 daddr,u8 addr,u8 data)
{
    I2C_Start();
    I2C_Send_Byte(daddr);
    I2C_Wait_Ack();
    I2C_Send_Byte(addr);
    I2C_Wait_Ack();
    I2C_Send_Byte(data);
    I2C_Wait_Ack();
    I2C_Stop();
}

u8 SingleRead(u8 daddr,u8 addr)
{
    u8 data;
    I2C_Start();
    I2C_Send_Byte(daddr);
    I2C_Wait_Ack();
    I2C_Send_Byte(addr);
    I2C_Wait_Ack();
    I2C_Start();
    I2C_Send_Byte(daddr+1);
    I2C_Wait_Ack();
    data=I2C_Read_Byte(0);//no ack
    I2C_Stop();
    return data;
}

void SDA_IN(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;    //????
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStruct.GPIO_Pin = PIN_SDA;
   GPIO_Init(I2C_PORT, &GPIO_InitStruct );
}

void SDA_OUT(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStruct.GPIO_Pin = PIN_SDA;
   GPIO_Init(I2C_PORT, &GPIO_InitStruct );
}

void Init_MPU1(void)
{
    SingleWrite1(MPUADDR,REG_PWR_MNG,0x00);//unlock
    SingleWrite1(MPUADDR,SMPLRT_DIV,0x07);//What is it used for?
    SingleWrite1(MPUADDR,REG_ACC_CONFIG,0x06);//Begin the data record
    SingleWrite1(MPUADDR,REG_GYR_CONFIG,0x06);
    SingleWrite1(MPUADDR,REG_GYR_RANGE,0x18);
    SingleWrite1(MPUADDR,REG_ACC_RANGE,0x01);
    SingleWrite1(MPUADDR,REG_MAG_CONFIG,0x02);
    SingleWrite1(MAGADDR,REG_MAG_MODE,0x01);
}

void I2C_Initi1(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE ); //This must be changed when change the port of the I2C interface
   GPIO_InitStructure.GPIO_Pin = PIN_SCL1 | PIN_SDA1;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;       //????
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

void I2C_Start1(void)
{
   SDA1_OUT();
   GPIO_SetBits(I2C_PORT,PIN_SDA1);  //SDA1 =1
   Delay_us(10);
   GPIO_SetBits(I2C_PORT,PIN_SCL1);  //SCL1=1
   Delay_us(10);
   GPIO_ResetBits(I2C_PORT,PIN_SDA1);//SDA1=0
   Delay_us(10);
   GPIO_ResetBits(I2C_PORT,PIN_SCL1);//SCL1=0
   Delay_us(10);
}

void I2C_Stop1(void)
{
   SDA1_OUT();
   GPIO_ResetBits(I2C_PORT,PIN_SDA1);
   Delay_us(10);
   GPIO_SetBits(I2C_PORT,PIN_SCL1);
   Delay_us(10);
   GPIO_SetBits(I2C_PORT,PIN_SDA1);
   Delay_us(10);
    //GPIO_ResetBits(I2C_PORT, PIN_SCL1);//SCL1=0;
   //Delay_us(10);
}

u8 I2C_Wait_Ack1(void)
{
   u8 ucErrTime=0;
   SDA1_IN();      //SDA1?????
   GPIO_SetBits(I2C_PORT,PIN_SDA1);
   Delay_us(5);
   GPIO_SetBits(I2C_PORT,PIN_SCL1);
   Delay_us(5);
   while(GPIO_ReadInputDataBit(I2C_PORT,PIN_SDA1))
   {
      ucErrTime++;
      if(ucErrTime>250)
      {
         I2C_Stop1();
         return 1;
      }
   }
   GPIO_ResetBits(I2C_PORT, PIN_SCL1);
   Delay_us(5);
   return 0;
}

void I2C_Ack1(void)
{
   //GPIO_ResetBits(I2C_PORT,PIN_SCL1);
   SDA1_OUT();
   GPIO_ResetBits(I2C_PORT,PIN_SDA1);
   Delay_us(5);
   GPIO_SetBits(I2C_PORT,PIN_SCL1);
   Delay_us(5);
   GPIO_ResetBits(I2C_PORT,PIN_SCL1);
}

void I2C_NAck1(void)
{
   GPIO_ResetBits(I2C_PORT,PIN_SCL1);
   SDA1_OUT();
   GPIO_SetBits(I2C_PORT,PIN_SDA1);
   Delay_us(5);
   GPIO_SetBits(I2C_PORT,PIN_SCL1);
   Delay_us(5);
   GPIO_ResetBits(I2C_PORT,PIN_SCL1);
}

void I2C_Send_Byte1(u8 txd)
{
    u8 t;
    SDA1_OUT();
    GPIO_ResetBits(I2C_PORT,PIN_SCL1);//??????????
    for(t=0;t<8;t++)
    {
        if((txd&0x80)==0x80)
          {GPIO_SetBits(I2C_PORT, PIN_SDA1);}//SDA1=1,? 1
        else
          {GPIO_ResetBits(I2C_PORT, PIN_SDA1);}//SDA1=0,? 0
        txd<<=1;
        Delay_us(5);   //?TEA5767??????????
        GPIO_SetBits(I2C_PORT,PIN_SCL1);
        Delay_us(5);
        GPIO_ResetBits(I2C_PORT,PIN_SCL1);
        Delay_us(5);
    }
}

u8 I2C_Read_Byte1(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA1_IN();//SDA1?????
    GPIO_SetBits(I2C_PORT,PIN_SDA1);
    for(i=0;i<8;i++ )
    {
        GPIO_ResetBits(I2C_PORT,PIN_SCL1);
        Delay_us(5);
        GPIO_SetBits(I2C_PORT,PIN_SCL1);
        Delay_us(5);
        receive<<=1;
        if(GPIO_ReadInputDataBit(I2C_PORT,PIN_SDA1))
        {
            receive++;
        }
        Delay_us(5);
    }
    if (!ack)
        I2C_NAck1();//??nACK
    else
        I2C_Ack1(); //??ACK
    return receive;
}

void SingleWrite1(u8 daddr,u8 addr,u8 data)
{
    I2C_Start1();
    I2C_Send_Byte1(daddr);
    I2C_Wait_Ack1();
    I2C_Send_Byte1(addr);
    I2C_Wait_Ack1();
    I2C_Send_Byte1(data);
    I2C_Wait_Ack1();
    I2C_Stop1();
}

u8 SingleRead1(u8 daddr,u8 addr)
{
    u8 data;
    I2C_Start1();
    I2C_Send_Byte1(daddr);
    I2C_Wait_Ack1();
    I2C_Send_Byte1(addr);
    I2C_Wait_Ack1();
    I2C_Start1();
    I2C_Send_Byte1(daddr+1);
    I2C_Wait_Ack1();//wait or not?????
    data=I2C_Read_Byte1(0);//no ack
    I2C_Stop1();
    return data;
}

void SDA1_IN(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;    //????
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStruct.GPIO_Pin = PIN_SDA1;
   GPIO_Init(I2C_PORT, &GPIO_InitStruct );
}

void SDA1_OUT(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStruct.GPIO_Pin = PIN_SDA1;
   GPIO_Init(I2C_PORT, &GPIO_InitStruct );
}

void Init_MPU2(void)
{
    SingleWrite2(MPUADDR,REG_ACC_CONFIG,0x06);//Begin the data record
    SingleWrite2(MPUADDR,REG_GYR_CONFIG,0x06);
    SingleWrite2(MPUADDR,REG_GYR_RANGE,0x10);
    SingleWrite2(MPUADDR,REG_ACC_RANGE,0x08);
    SingleWrite2(MPUADDR,REG_MAG_CONFIG,0x02);
    SingleWrite2(MPUADDR,REG_MAG_MODE,0x16);
}

void I2C_Initi2(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE ); //This must be changed when change the port of the I2C interface
   GPIO_InitStructure.GPIO_Pin = PIN_SCL2 | PIN_SDA2;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;       //????
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

void I2C_Start2(void)
{
   SDA2_OUT();
   GPIO_SetBits(I2C_PORT,PIN_SDA2);  //SDA2 =2
   Delay_us(10);
   GPIO_SetBits(I2C_PORT,PIN_SCL2);  //SCL2=2
   Delay_us(10);
   GPIO_ResetBits(I2C_PORT,PIN_SDA2);//SDA2=0
   Delay_us(10);
   GPIO_ResetBits(I2C_PORT,PIN_SCL2);//SCL2=0
   Delay_us(10);
}

void I2C_Stop2(void)
{
   SDA2_OUT();
   GPIO_ResetBits(I2C_PORT,PIN_SDA2);
   Delay_us(10);
   GPIO_SetBits(I2C_PORT,PIN_SCL2);
   Delay_us(10);
   GPIO_SetBits(I2C_PORT,PIN_SDA2);
   Delay_us(10);
}

u8 I2C_Wait_Ack2(void)
{
   u8 ucErrTime=0;
   SDA2_IN();      //SDA2?????
   GPIO_SetBits(I2C_PORT,PIN_SDA2);
   Delay_us(5);
   GPIO_SetBits(I2C_PORT,PIN_SCL2);
   Delay_us(5);
   while(GPIO_ReadInputDataBit(I2C_PORT,PIN_SDA2))
   {
      ucErrTime++;
      if(ucErrTime>250)
      {
         I2C_Stop2();
         return 1;
      }
   }
   GPIO_ResetBits(I2C_PORT, PIN_SCL2);
   Delay_us(5);
   return 0;
}

void I2C_Ack2(void)
{
   //GPIO_ResetBits(I2C_PORT,PIN_SCL2);
   SDA2_OUT();
   GPIO_ResetBits(I2C_PORT,PIN_SDA2);
   Delay_us(5);
   GPIO_SetBits(I2C_PORT,PIN_SCL2);
   Delay_us(5);
   GPIO_ResetBits(I2C_PORT,PIN_SCL2);
}

void I2C_NAck2(void)
{
   GPIO_ResetBits(I2C_PORT,PIN_SCL2);
   SDA2_OUT();
   GPIO_SetBits(I2C_PORT,PIN_SDA2);
   Delay_us(5);
   GPIO_SetBits(I2C_PORT,PIN_SCL2);
   Delay_us(5);
   GPIO_ResetBits(I2C_PORT,PIN_SCL2);
}

void I2C_Send_Byte2(u8 txd)
{
    u8 t;
    SDA2_OUT();
    GPIO_ResetBits(I2C_PORT,PIN_SCL2);//??????????
    for(t=0;t<8;t++)
    {
        if((txd&0x80)==0x80)
          {GPIO_SetBits(I2C_PORT, PIN_SDA2);}//SDA2=2,? 2
        else
          {GPIO_ResetBits(I2C_PORT, PIN_SDA2);}//SDA2=0,? 0
        txd<<=1;
        Delay_us(5);   //?TEA5767??????????
        GPIO_SetBits(I2C_PORT,PIN_SCL2);
        Delay_us(5);
        GPIO_ResetBits(I2C_PORT,PIN_SCL2);
        Delay_us(5);
    }
}

u8 I2C_Read_Byte2(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA2_IN();//SDA2?????
    GPIO_SetBits(I2C_PORT,PIN_SDA2);
    for(i=0;i<8;i++ )
    {
        GPIO_ResetBits(I2C_PORT,PIN_SCL2);
        Delay_us(5);
        GPIO_SetBits(I2C_PORT,PIN_SCL2);
        Delay_us(5);
        receive<<=1;
        if(GPIO_ReadInputDataBit(I2C_PORT,PIN_SDA2))
        {
            receive++;
        }
        Delay_us(5);
    }
    if (!ack)
        I2C_NAck2();//??nACK
    else
        I2C_Ack2(); //??ACK
    return receive;
}

void SingleWrite2(u8 daddr,u8 addr,u8 data)
{
    I2C_Start2();
    I2C_Send_Byte2(daddr);
    I2C_Wait_Ack2();
    I2C_Send_Byte2(addr);
    I2C_Wait_Ack2();
    I2C_Send_Byte2(data);
    I2C_Wait_Ack2();
    I2C_Stop2();
}

u8 SingleRead2(u8 daddr,u8 addr)
{
    u8 data;
    I2C_Start2();
    I2C_Send_Byte2(daddr);
    I2C_Wait_Ack2();
    I2C_Send_Byte2(addr);
    I2C_Wait_Ack2();
    I2C_Start2();
    I2C_Send_Byte2(daddr+1);
    I2C_Wait_Ack2();//wait or not?????
    data=I2C_Read_Byte2(0);//no ack
    I2C_Stop2();
    return data;
}

void SDA2_IN(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;    //????
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStruct.GPIO_Pin = PIN_SDA2;
   GPIO_Init(I2C_PORT, &GPIO_InitStruct );
}

void SDA2_OUT(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStruct.GPIO_Pin = PIN_SDA2;
   GPIO_Init(I2C_PORT, &GPIO_InitStruct );
}
