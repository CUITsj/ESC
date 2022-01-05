#include "adc.h"

volatile uint16_t ADC_Data[6];    //�洢6��ADC���ݵı���,0:C���ѹ|1:B���ѹ|2:���Ե��ѹ|3:A���ѹ|4:ĸ�ߵ���|5:ĸ�ߵ�ѹ

void ADC_DMA_Init(void)
{
	//���������ṹ��
	ADC_InitTypeDef       ADC_InitStructure;	
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;
	
	//ʹ����Ӧʱ��Դ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	
	//����ADC��Ƶ����Ϊ6��Ƶ��72M/6=12M,��ΪADC������ʱ�Ӳ��ó���14MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  
	
	//����6��ADCͨ���Ĳ����ܽ�
	//C���ѹ|B���ѹ|���Ե��ѹ|A���ѹ|ĸ�ߵ���|ĸ�ߵ�ѹ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//ADC��ʼ��
	ADC_DeInit(ADC1);  //��λADC1
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC����ģʽ:ADC1��ADC2�����໥����
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//���ͨ��ת����Ҫʹ��ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC�����Ҷ���,0x0FFF
	ADC_InitStructure.ADC_NbrOfChannel = 6;//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);

	//����ADC����ͨ���Ĳ���˳��Ͳ���ʱ�䣬ʱ��Խ�����Խ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_7Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_7Cycles5);  
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_7Cycles5); 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_7Cycles5); 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_7Cycles5); 

	ADC_Cmd(ADC1, ENABLE);//ʹ��ָ����ADC1
	ADC_DMACmd(ADC1, ENABLE); //����ADC��DMA֧��

	//DMA��ʼ��
	DMA_DeInit(DMA1_Channel1);//��DMA1ͨ��1�ļĴ�����ΪĬ��ֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(ADC1->DR);//DMA��ȡ��ADC���ݼĴ�����ʼ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Data;//DMAд����ڴ���ʼ��ַ 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//����(�����赽�ڴ�)

	DMA_InitStructure.DMA_BufferSize = 6;//�������ݵĴ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ���̶�����6�������ĵ�ַ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݿ��Ϊ16λ��ADCһ�����ݵĿ��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�ڴ����ݿ��

	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMAѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//���ȼ�����
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//��ֹ�ڴ浽�ڴ�Ĵ���
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	ADC_ResetCalibration(ADC1);//ʹ�ܸ�λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ���λУ׼����	
	ADC_StartCalibration(ADC1);//����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));//�ȴ�У׼����
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //���������ʼת��	
}
