#ifndef  I2C_H  
#define  I2C_H

#include "stm32f10x.h"   


//Global address
#define MPUADDR             0xD0
#define MAGADDR             0x18
#define REG_ACC_CONFIG      0x1D
#define REG_GYR_CONFIG      0x1A
#define REG_GYR_RANGE       0x1B
#define REG_ACC_RANGE       0x1C
#define REG_MAG_CONFIG      0x37
#define REG_MAG_MODE        0x0A
#define REG_PWR_MNG         0x6B
#define SMPLRT_DIV          0x19

//Register address for acceleromter
#define REG_DATA_GX_LOW     0x3C  
#define REG_DATA_GX_HIGH    0x3B
#define REG_DATA_GY_LOW     0x3E
#define REG_DATA_GY_HIGH    0x3D
#define REG_DATA_GZ_LOW     0x40
#define REG_DATA_GZ_HIGH    0x3F

//Register address for gyroscope
#define REG_DATA_WX_LOW     0x44 
#define REG_DATA_WX_HIGH    0x43
#define REG_DATA_WY_LOW     0x46
#define REG_DATA_WY_HIGH    0x45
#define REG_DATA_WZ_LOW     0x48
#define REG_DATA_WZ_HIGH    0x47

//Register address for Magnetometer
#define REG_DATA_BX_LOW     0x03
#define REG_DATA_BX_HIGH    0x04
#define REG_DATA_BY_LOW     0X05
#define REG_DATA_BY_HIGH    0X06
#define REG_DATA_BZ_LOW     0X07
#define REG_DATA_BZ_HIGH    0X08


//Pin map for communication
#define PIN_SCL    GPIO_Pin_5  
#define PIN_SDA    GPIO_Pin_4
#define PIN_SCL1   GPIO_Pin_3  
#define PIN_SDA1   GPIO_Pin_2
#define PIN_SCL2   GPIO_Pin_1  
#define PIN_SDA2   GPIO_Pin_0  
#define I2C_PORT   GPIOE  


void Init_MPU(void); 
void Init_MPU1(void);
void Init_MPU2(void);


void SDA_IN(void);  
void SDA1_IN(void);  
void SDA2_IN(void);  

void SDA_OUT(void);
void SDA1_OUT(void);
void SDA2_OUT(void);

void I2C_Initi(void);                      
void I2C_Initi1(void);                 
void I2C_Initi2(void);                  

void I2C_Start(void);       
void I2C_Start1(void);  
void I2C_Start2(void);  

void I2C_Stop(void);     
void I2C_Stop1(void);
void I2C_Stop2(void);

void I2C_Send_Byte(u8 txd); 
void I2C_Send_Byte1(u8 txd);
void I2C_Send_Byte2(u8 txd);

u8 I2C_Read_Byte(unsigned char ack);   
u8 I2C_Read_Byte1(unsigned char ack);   
u8 I2C_Read_Byte2(unsigned char ack);   

u8 I2C_Wait_Ack(void); 
u8 I2C_Wait_Ack1(void);
u8 I2C_Wait_Ack2(void);

void I2C_Ack(void);
void I2C_Ack1(void);
void I2C_Ack2(void);

void I2C_NAck(void);      
void I2C_NAck1(void);   
void I2C_NAck2(void);   

void SingleWrite(u8 daddr,u8 addr,u8 data); 
void SingleWrite1(u8 daddr,u8 addr,u8 data); 
void SingleWrite2(u8 daddr,u8 addr,u8 data); 

u8 SingleRead(u8 daddr,u8 addr);     
u8 SingleRead1(u8 daddr,u8 addr);  
u8 SingleRead2(u8 daddr,u8 addr);  

#endif 
