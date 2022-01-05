#include "gpio.h"

void GPIO_COM_Init(void)
{
	GPIO_InitTypeDef 					GPIO_InitStructure;  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	LED_OFF;
}

/**************************************************************************
�������ܣ�LED��˸
��ڲ�������˸Ƶ��
����  ֵ����
**************************************************************************/
void LED_Blink(u8 cnt)
{
	u8 i;
	
	for (i=0;i<cnt;i++)
	{
		LED_ON;
		delay_ms(500);
		LED_OFF;
		delay_ms(500);
	}
}

