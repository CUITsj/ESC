/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/******************************************************************************/
/*����Ϊ�û���д�������жϷ�����                                              */
/******************************************************************************/

/***************PPM******************/
static u8 Rise_flag = 0;
static u16 Hightime_us = 0;
u16 PPM_Duty = 0,DELAY_CTL=0,START_FLAG=0;//PPM���ŷ�Χ0-2000
/***************PPM******************/

//motorcontrol
void TIM1_UP_IRQHandler(void)
{
	static u8 ED360_CNT=0;
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseStructure;
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//		if(MC.DUTY>=1000)//duty>=50%,���������ж�
//		{
//			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned2;//PWM�������ģʽ�����������ж�
//			TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);			
//		}
//		else if(MC.DUTY<1000)//duty<=50%,���������ж�
//		{
//			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;//PWM�������ģʽ�����������ж�
//			TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);			
//		}
		//������ƴ���
		if(MC.STATE==1)//����Ѿ������ɹ�����ʼ�����Ӧ���綯��
		{
			DEBUG_PIN_HIGH;
			if(MC.CCW)//��ת
				{
					switch(MC.PHA_CNT)
					{
						#if OPP 	//����ȱ�ౣ��
						//AB�ർͨ�����C��BEMF
						case 0: ZCD_C_DOWN(); MC.AI=(ADC_Data[BIT_I]-OFFSET_I); break;
						case 1: ZCD_B_UP(); 	MC.AI=(ADC_Data[BIT_I]-OFFSET_I); break;
						case 2: ZCD_A_DOWN(); MC.BI=(ADC_Data[BIT_I]-OFFSET_I); break;
						case 3: ZCD_C_UP(); 	MC.BI=(ADC_Data[BIT_I]-OFFSET_I); break;
						case 4: ZCD_B_DOWN(); MC.CI=(ADC_Data[BIT_I]-OFFSET_I); break;
						case 5: ZCD_A_UP(); 	MC.CI=(ADC_Data[BIT_I]-OFFSET_I); break;
						default: break;
						#else 
						//AB�ർͨ�����C��BEMF
						case 0: ZCD_C_DOWN(); break;
						case 1: ZCD_B_UP(); 	break;
						case 2: ZCD_A_DOWN(); break;
						case 3: ZCD_C_UP(); 	break;
						case 4: ZCD_B_DOWN(); break;
						case 5: ZCD_A_UP(); 	break;
						default: break;
						#endif
					}
				}
				else//��ת
				{
					switch(MC.PHA_CNT)
					{
						#if OPP		//����ȱ�ౣ��
						case 5: ZCD_C_UP(); 	MC.AI=(ADC_Data[BIT_I]-OFFSET_I); break;
						case 4: ZCD_B_DOWN(); MC.AI=(ADC_Data[BIT_I]-OFFSET_I); break;
						case 3: ZCD_A_UP(); 	MC.BI=(ADC_Data[BIT_I]-OFFSET_I); break;
						case 2: ZCD_C_DOWN(); MC.BI=(ADC_Data[BIT_I]-OFFSET_I); break;
						case 1: ZCD_B_UP(); 	MC.CI=(ADC_Data[BIT_I]-OFFSET_I); break;
						case 0: ZCD_A_DOWN(); MC.CI=(ADC_Data[BIT_I]-OFFSET_I); break;
						default: break;
						#else
						case 5: ZCD_C_UP(); 	break;
						case 4: ZCD_B_DOWN(); break;
						case 3: ZCD_A_UP(); 	break;
						case 2: ZCD_C_DOWN(); break;
						case 1: ZCD_B_UP(); 	break;
						case 0: ZCD_A_DOWN(); break;
						default: break;
						#endif
					}
				}
		}
		if(MC.STATE==2)//��ʼ��ʱȻ����
		{
			DEBUG_PIN_LOW;	
			MC.DELAY30--;
			if (MC.DELAY30 == 0)
			{
				MC.PHA_CNT++;
				if(MC.PHA_CNT == 6)
				{
					ED360_CNT++;//ת��360�ȵ�Ƕȼ���
					if(ED360_CNT==POLES)//��е�Ƕ�ת��360��
					{
						ED360_CNT=0;
						MC.CYCLES++;
					}
					MC.PHA_CNT = 0;
				}
				phase_change();
				MC.STATE=1;
			}
		}
	}
}

//PPM
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_CC2) != RESET)//������������
	{
		if(Rise_flag == 1)//�ϴβ��������أ����Ϊ�½���
		{
			Rise_flag=0;//�����ر�־����
			Hightime_us=TIM_GetCapture2(TIM2);//��¼3ͨ������ֵ
			//����PPM��������ռ�ձ�
			if(Hightime_us<960)
			{
				PPM_Duty=80;
			}
			else if (Hightime_us>2100)
			{
				PPM_Duty=2000;
			}
			else
			{
				PPM_Duty=Hightime_us-900;
			}
			if (PPM_Duty > 500)
			{
				PPM_Duty=500;
			}
			if (DELAY_CTL == 1)
			{
				MC.DUTY = PPM_Duty;
			}
			else
			{
				START_FLAG = 1;
			}
			if (MC.DUTY <= 120)
			{
				MC.ADJ30=0.7;
			}
			else if (MC.DUTY > 120 && MC.DUTY<=130)
			{
				MC.ADJ30=0.6;
			}
			else if (MC.DUTY>130 && MC.DUTY <=140)
			{
				MC.ADJ30=0.5;
			}
			else if (MC.DUTY>140 && MC.DUTY <=150)
			{
				MC.ADJ30=0.4;
			}
			else if (MC.DUTY>150 && MC.DUTY <=160)
			{
				MC.ADJ30=0.3;
			}
			else if (MC.DUTY>160 && MC.DUTY <=170)
			{
				MC.ADJ30=0.3;
			}
			else if (MC.DUTY>170 && MC.DUTY <=200)
			{
				MC.ADJ30=0.2;
			}
			else
			{
				MC.ADJ30=0.2;
			}
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising);//����Ϊ�����ز���
		}
		else//��һ�β���������
		{
			Rise_flag=1;//�����ر�־��λ
			Hightime_us=0;//�ߵ�ƽ����ʱ�����
			TIM_SetCounter(TIM2,0);//��������ֵ����Ϊ0
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);//����Ϊ�½��ز���
		}
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update|TIM_IT_CC2);//���жϱ�־
}

//PID��ת�ٲ���
void TIM3_IRQHandler(void)
{
	static u8 cnt_2s=0,cnt1s=0;
	u16	BUFF=0;//���ת�ٻ���
	
	#if (LOOP==0 || LOOP==1)
	s16 iErr=0;	//��ǰ�������
	s16 buff_duty=0; //�������
	static s16 sum_iErr=0;	//�ۼƵ������
	static s16 last_iErr=0;	//�ϴε������
	#endif
	#if (LOOP==1)
	s16 sErr=0;//��ǰ�ٶ����
	static s16 last1_sErr=0;//�ϴ��ٶ����
	static s16 last2_sErr=0;//���ϴ��ٶ����
	#endif
	
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//�ж��Ƿ���TIM3�ж�
	{	
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//���жϱ�־
		
		/*****************����ת��*****************/
		cnt_2s++;
		if(cnt_2s==200)//2��ʱ���ѵ�
		{
			cnt_2s=0;
			BUFF=MC.SPEED;
			MC.SPEED = MC.CYCLES*30;
			FirstOrder_LPF(BUFF,MC.SPEED,0.8);//�Բ�õ�ת�ٽ���һ�׵�ͨ�˲�
			MC.CYCLES=0;
		}
		//��ʱPPM����
		if (START_FLAG == 1)
		{
			START_FLAG = 0;
			cnt1s++;
		}
		if (cnt1s == 255)
		{
			cnt1s=0;
			DELAY_CTL = 1;
		}
		/*****************����ת��*****************/
		
//		
//		#if (LOOP == 0)//0��������
//		/*****************������*****************/
//			//1�����㵱ǰ���
//			iErr = PPM_Duty - (ADC_Data[BIT_I]-OFFSET_I);
//			
//			//2�������ۼ����޷�
//			sum_iErr += iErr;
//			if (sum_iErr > 10000)
//			{
//				sum_iErr = 10000;
//			}
//			else if (sum_iErr < -10000)
//			{
//				sum_iErr = -10000;
//			}
//			
//			//3��λ��ʽPID
//			buff_duty =  iErr * 100 + sum_iErr * 0.5 + (iErr - last_iErr) * 0;
//			
//			//4��PID����޷�
//			if (buff_duty>2000)
//			{
//				buff_duty = 2000;
//			}
//			else if (buff_duty < 0)
//			{
//				buff_duty = 0;
//			}
//			
//			//5�����
//			MC.DUTY = buff_duty;
//			
//			//6����¼��ǰ���
//			last_iErr = iErr;
//		/*****************������*****************/
//		
//		#elif (LOOP == 1)//1���ٶȻ���������
//		/*****************�ٶȻ�+������*****************/
//			//1�����㵱ǰ�ٶ����
//			sErr = PPM_Duty - MC.SPEED;
//			
//			//2������ʽPID
//			buff_duty += 1*(sErr-last1_sErr)+1*sErr+1*(sErr-2*last1_sErr+last2_sErr);
//			
//			//3����¼��ǰ���
//			last2_sErr = last1_sErr;
//			last1_sErr = sErr;
//			
//			//4�����㵱ǰ���
//			iErr = buff_duty - (ADC_Data[BIT_I]-OFFSET_I);
//			
//			//5�������ۼ����޷�
//			sum_iErr += iErr;
//			if (sum_iErr > 10000)
//			{
//				sum_iErr = 10000;
//			}
//			else if (sum_iErr < -10000)
//			{
//				sum_iErr = -10000;
//			}
//			
//			//6��λ��ʽPID
//			buff_duty =  iErr * 100 + sum_iErr * 0.5 + (iErr - last_iErr) * 0;
//			
//			//7��PID����޷�
//			if (buff_duty>2000)
//			{
//				buff_duty = 2000;
//			}
//			else if (buff_duty < 0)
//			{
//				buff_duty = 0;
//			}
//			
//			//8�����
//			MC.DUTY = buff_duty;
//			
//			//9����¼��ǰ���
//			last_iErr = iErr;
//		/*****************�ٶȻ�+������*****************/
//		#endif
		
		/*****************��Ƿѹ����*****************/
		#if OUP
		if(ADC_Data[BIT_U] > MC.OVER_VOL)//ĸ�ߵ�ѹ����13V
		{
			ALLMOS_OFF;
			MC.FAULT = 13;
			LED_Blink(3);
			while(1);
		}
		else if (ADC_Data[BIT_U] < MC.UNDER_VOL)//ĸ�ߵ�ѹС��11V
		{
			ALLMOS_OFF;
			MC.FAULT = 14;
			LED_Blink(4);
			while(1);
		}
		#endif
		#if OCP
		/*****************��������*****************/
		if (ADC_Data[BIT_I] > 2850)//����45A��������������,MOS�͵���50A��2212������19A����Ҫ�Ǳ���MOS
		{
			ALLMOS_OFF;
			MC.FAULT = 15;
			LED_Blink(5);
			while(1);
		}
		#endif
		#if MSLP
		/*****************��ת����*****************/
		if(MC.DUTY>321&&MC.SPEED<123&&(ADC_Data[BIT_I]-OFFSET_I)>321)//���������ٶ������ҵ��������ж�Ϊ��ת
		{
			ALLMOS_OFF;
			MC.FAULT = 16;
			LED_Blink(6);
			while(1);
		}
		#endif
		#if OPP
		/*****************ȱ�ౣ��*****************/
		//ĳ�����С����������һ�������һ�룬����Ϊ��ȱ�ֻ࣬�ܼ��ȱһ���������
		if(MC.AI<(MC.BI/2) || MC.AI<(MC.CI/2) || MC.BI<(MC.AI/2) || MC.BI<(MC.CI/2) || MC.CI<(MC.BI/2) || MC.CI<(MC.AI/2))
		{
			ALLMOS_OFF;
			MC.FAULT = 17;
			LED_Blink(7);
			while(1);
		}
		#endif
	}
}

//���ڽ����ж�
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //���յ�����
	{	  
		USART_ClearFlag(USART1, USART_FLAG_RXNE);     //����жϱ�־
	}
}

/******************************************************************************/
/*����Ϊ�û���д�������жϷ�����                                              */
/******************************************************************************/

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
