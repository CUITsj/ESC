#include "timer.h"
//TIM1����PWM
//TIM2����PPM
//TIM3����PID


//pwm_freqΪPWM��Ƶ�ʵ�λΪKHz
void TIM1_PWM_Init(u16 pwm_freq)
{
	//������Ӧ�ṹ��
	NVIC_InitTypeDef 					NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef       	TIM_OCInitStructure;
	GPIO_InitTypeDef 					GPIO_InitStructure;  
	
	//1��ʹ��IO�Ͷ�ʱ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
	
	//�������ű۵���·PWM����//PWM_AH | PWM_BH | PWM_CH
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//���ÿ�©���
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//�������ű���·IO��//PWM_AL | PWM_BL | PWM_CL
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_OD;//��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
			
	//2����ʱ��������ʼ��
	//���㹫ʽ��PWM���� = ��(��װ��ֵ+1)*(Ԥ��Ƶϵ��+1)/ʱ��Ƶ�ʣ�*2;
 	TIM_DeInit(TIM1);//��TIM1�Ĵ���ֵ����ΪĬ��ֵ
	TIM_TimeBaseStructure.TIM_Period = 36000 / pwm_freq - 1;//��װ��ֵ
  TIM_TimeBaseStructure.TIM_Prescaler = 0;//Ԥ��Ƶϵ��
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;//PWM�������ģʽ�����������ж�             
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim                 
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //���ý����жϵĴ�����0ÿ�ζ�����     
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	//��ʼ������ͨ��������Ƚϲ���
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//����ʱ��ֵС�ڱȽ����趨ֵʱ��TIMX����Ŵ�ʱ�����Ч��ƽ��
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
  TIM_OCInitStructure.TIM_Pulse = 0;//���ô�װ�벶��ȽϼĴ���������ֵ��PWMռ�ձȣ�CCR��ʼֵΪ0
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High ; //�������:TIM����Ƚϼ���Ϊ�ߵ�ƽ
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//����ʱ����͵�ƽ
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);                         
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
  
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//CH1Ԥװ��ʹ��               
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);//CH2Ԥװ��ʹ��
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);//CH3Ԥװ��ʹ��
 
	//3��������ʱ���ж�
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);  
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);                            //���жϱ�־λ
  TIM_ITConfig(TIM1,TIM_IT_Update ,ENABLE);                              //���ж� 
	
	//����NVIC
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;//�����жϺ�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);
	
	//4��ʹ�ܶ�ʱ��
	TIM_Cmd(TIM1, ENABLE);//ʹ��TIM1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//ʹ��PWM
	
	ALLMOS_OFF;
}

//�ö�ʱ�����ڲ���PPM�ź�
void TIM2_CAP_Init(u16 arr,u16 psc)	
{
	GPIO_InitTypeDef 					GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_ICInitTypeDef 				TIM_ICInitStructure;  
 	NVIC_InitTypeDef 					NVIC_InitStructure;
	
	//����IO�Ͷ�ʱ��ʱ��Դ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIOB����ӳ��ʱ��
	
	//��ӳ��Ϊ��ͨIO
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); 
	//����ΪTIM2CH2
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);  
	
	//PB3��PPM���չܽ�
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB3 ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//��ʼ����ʱ��2
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM2_CH2���벶�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC2S=02 	ѡ������� IC2ӳ�䵽TI1��
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM_ICInitStructure.TIM_ICFilter = 0x00;//���������˲��� ���˲�
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update|TIM_IT_CC2);//���жϱ�־	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);//��������ж� ,�������ж�	
  TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��2
}


//��ʱ��ʱ����㹫ʽTout = ((��װ��ֵ+1)*(Ԥ��Ƶϵ��+1))/ʱ��Ƶ��;
//���磺1�붨ʱ����װ��ֵ=9999��Ԥ��Ƶϵ��=7199
//�ö�ʱ��ֻ����ʱ�ж�ʹ��
void TIM3_Init(u16 arr,u16 psc)	
{
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
 	NVIC_InitTypeDef 					NVIC_InitStructure;

	//1��ʹ�ܶ�ʱ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	
	//2����ʱ��������ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//3��������ʱ���ж�����NVIC
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//��������ж�	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	TIM_Cmd(TIM3,ENABLE); 	//ʹ�ܶ�ʱ��3	
}
