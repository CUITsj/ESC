#ifndef __ADC_H__
#define __ADC_H__

#include "include.h"

//����ADC_Data�����Ѱλ�꣬����Ӳ���ı�ADC�����ܽź��޸ĳ���
#define 	BIT_A   	3
#define 	BIT_B			1
#define 	BIT_C			0
#define		BIT_N			2
#define 	BIT_I			4
#define 	BIT_U			5
extern volatile uint16_t ADC_Data[6];    //�洢6��ADC���ݵı���,0:C���ѹ|1:B���ѹ|2:���Ե��ѹ|3:A���ѹ|4:ĸ�ߵ���|5:ĸ�ߵ�ѹ

void ADC_DMA_Init(void);

#endif 
