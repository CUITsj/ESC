#include "SysTick.h"
//72M�����������ʱ�����ʱ1.864135��
//ȫ�ֱ������� static �ؼ��֣�����������ͱ���˵�ǰ�ļ����������ļ��о���Ч�ˡ�
static u8  cnt_us=0;							//SysTick����һ΢��ļ���ֵ			   
static u16 cnt_ms=0;							//SysTick����һ����ļ���ֵ			

//��ʼ���ӳٺ���
//HCLKʱ�Ӽ�ΪAHBʱ��
//SystemCoreClock:ϵͳʱ��72M
void DELAY_Init()
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ��AHBʱ�ӵ�8��Ƶ��9MHz��ΪSysTickʱ�ӣ�
	cnt_us=SystemCoreClock/8000000;				//9M����9��Ϊһ΢��
	cnt_ms=(u16)cnt_us*1000;				//9M����9000��Ϊһ����
}

//��ʱnus
//nusΪҪ��ʱ��us��.	
//��72M������,nus<=1864135 
void delay_us(u32 nus)
{		
	u32 temp;	
	SysTick->LOAD=nus*cnt_us; 					//ʱ�����	  		 
	SysTick->VAL=0x00;        					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0x00;      					 //��ռ�����	 
}

//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		 
	SysTick->LOAD=(u32)nms*cnt_ms;				//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;							//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0x00;       					//��ռ�����	  	    
} 


