#ifndef _MAIN_H
#define _MAIN_H

//global variants

char str[256];

double gx1,gy1,gz1;
double gx2,gy2,gz2;
double wx1,wy1,wz1;
double wx2,wy2,wz2;
double bx1,by1,bz1;
double bx2,by2,bz2;

int16_t gx1l,gy1l,gz1l,gx2l,gy2l,gz2l;
int16_t gx1h,gy1h,gz1h,gx2h,gy2h,gz2h;

int16_t wx1l,wy1l,wz1l,wx2l,wy2l,wz2l;
int16_t wx1h,wy1h,wz1h,wx2h,wy2h,wz2h;

int16_t bx1l,by1l,bz1l,bx2l,by2l,bz2l;
int16_t bx1h,by1h,bz1h,bx2h,by2h,bz2h;

//functions

void Sys_Init(void);

void MPU_Measure(void);
void MPU_Calc(void);

#endif
