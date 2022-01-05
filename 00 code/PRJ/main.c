/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2021,һ��ѧ�Կ�
 *     All rights reserved.
 *     �������ۣ�QQȺ877041899
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����һ��ѧ�Զ����У�δ����������������ҵ��;��
 *		 ��Ȩ���У�����ؾ����޸�����ʱ���뱣��һ��ѧ�Կصİ�Ȩ������
 *
 * @file       main.c
 * @brief      ������
 * @author     SuJin
 * @version    v1.0
 * @date       2021-08-16
 */
#include "include.h"

int main(void)
{
	//1�����ж����ȼ�����Ϊ2����λ��ռ���ȼ�����λ��Ӧ���ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//2�δ��ʱ����ʼ��
	DELAY_Init();
	//3������Ƹ��ֲ�����ʼ��
	PARAM_Init();
	//4��ͨIO�ܽų�ʼ��,LED DEBUG_PIN
	GPIO_COM_Init();
	//5��ʼ��PWMƵ��Ϊ18KHz	
	TIM1_PWM_Init(18);
	//6ADC��ʼ��	
	ADC_DMA_Init();
	//7PPM����λ����ʱ��Ϊ1΢�룬����65535΢�루65.535.���룩��������ж�
	TIM2_CAP_Init(0xFFFF,71);	
	//8PID����ʼ����ʱ��3������Ϊ99->10ms,	199->20ms,499->50ms,999->100ms	
	TIM3_Init(99,7199);	
	//9����1��ʼ��	
	USART1_Init(115200);
	//10���MOS�Ƿ��·	
	MOSSHORT_Check();	
	//11���MOS�ܷ�������ͨ	
	MOSOPEN_Check();		
	//12ADC����ƫ�ú͵�������ȡ	
	OFFSET2BAT_Read();	
	//13�����������	
	MOTOR_Start();	
  while(1)
	{
		
//		OPEN_Check();		
		u8 i=0;
		u16 t[6]={0};
	u16 I_BUS[5],IBUFF=0;
	
	for(i=0;i<5;i++)
	{
		I_BUS[i]=ADC_Data[BIT_I]-OFFSET_I;
		delay_ms(10);
	}
	IBUFF = (I_BUS[0]+I_BUS[1]+I_BUS[2]+I_BUS[3]+I_BUS[4])/5.0;
		
		t[0] = MC.SPEED>>8;
		t[1] = MC.SPEED;
		t[2] = MC.DUTY>>8;
		t[3] = MC.DUTY;
		t[4] = IBUFF>>8;
		t[5] = IBUFF;
		
		for (i=0; i<6; i++)
		{
			USART_SendData(USART1 ,t[i]); //���͵�����ֵ
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ
		}
		delay_ms(50);
	}
}
