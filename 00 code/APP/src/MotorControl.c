#include "MC.h"

Motor_Control		MC=MC_DEFAULTS;

//������Ƹ��ֲ�����ʼ��
void PARAM_Init(void)
{
	MC.CCW=0;      	//1����ת��0������
	MC.PHA_CNT=0; 	//�������
	MC.STATE=0;			//���״̬
	MC.ZCD_CNT=0;		//�����������
	MC.BAT=0;				//������࣬1����2S��2����3S��3����4S
	MC.FAULT=0;			//���ϼ�¼��//1-6��ʾA+,B+,C+,A-,B-,C-�Ķ�·���ϣ�7-12��ʾA+,B+,C+,A-,B-,C-���ܿ�ͨ���ϣ�13-17:��ѹ��Ƿѹ����������ת��ȱ��,18:��������
	MC.DUTY=0;     	//ռ�ձ�
	MC.DELAY30=0;		//��ʱ30�ȼ���
	MC.CYCLES=0;		//���ת����Ȧ������
	MC.SPEED=0;			//���ת��
	MC.AI=0;				//A�����
	MC.BI=0;				//B�����
	MC.CI=0;				//C�����
	MC.OVER_VOL=0;	//��ѹֵ
	MC.UNDER_VOL=0;	//Ƿѹֵ
	MC.ADJ30=0.7;		//��ʱ30�ȵ���ϵ��0.7��������ȫ��ʱǰ30���߹���ʱ�䣬��Ҫ�ʵ���С,��С�ή�͵��Ч�ʣ���������������䵼�µ������
}
//�����������
void MOTOR_Start(void)
{
	u16 step_length=300;//��ʼ���ಽ��Ϊ300���̶�����
	u16 i;
	
	MC.DUTY=110;//Ԥ��λռ�ձȣ�����100��������120��110���߶�������
	MOS_ABON();
	delay_ms(50);//Ԥ��λʱ��50����ȽϺ���
	while(1)
	{
		for(i=0;i<step_length;i++)
		{
			delay_us(100);//��ʱ100us���̶�����
		}
		step_length -= step_length/30+1;//������������ѿ��ԼӴ������������30�ȽϺ���

		if(step_length < 25)
		{
			if(CLOSED_LOOP)
			{
				MC.STATE=1;//���������һ��״̬
				MC.DUTY=80;//�����ʼ����ռ�ձ�
				break;
			}
			else
			{	
				step_length = 25;   //����ǿ�ƻ���һֱ�����϶������ת
			}				
		}
		MC.PHA_CNT++;//ǿ�ƻ��࣬�϶����
		if(MC.PHA_CNT == 6)
		{
			MC.PHA_CNT = 0;
		}
		phase_change();//�������ຯ��
	}
}

//A������⺯��,A��BEMF�ɵͱ��
void ZCD_A_UP(void)
{
	//��ʱ30�ȼ���
	MC.DELAY30++;
	if(ADC_Data[BIT_A]>ADC_Data[BIT_N])//��⵽����
	{
		MC.ZCD_CNT++;
		if(MC.ZCD_CNT==8)//��⵽3�ι����ź�
		{
			MC.ZCD_CNT=0;
			MC.DELAY30=MC.DELAY30*MC.ADJ30;
			MC.STATE=2;
		}
	}
}

//B������⺯��,B��BEMF�ɵͱ��
void ZCD_B_UP(void)
{
	//��ʱ30�ȼ���
	MC.DELAY30++;
	if(ADC_Data[BIT_B]>ADC_Data[BIT_N])//��⵽����
	{
		MC.ZCD_CNT++;
		if(MC.ZCD_CNT==8)//��⵽3�ι����ź�
		{
			MC.ZCD_CNT=0;
			MC.DELAY30=MC.DELAY30*MC.ADJ30;
			MC.STATE=2;
		}
	}
}

//C������⺯��,C��BEMF�ɵͱ��
void ZCD_C_UP(void)
{
	//��ʱ30�ȼ���
	MC.DELAY30++;
	if(ADC_Data[BIT_C]>ADC_Data[BIT_N])//��⵽����
	{
		MC.ZCD_CNT++;
		if(MC.ZCD_CNT==8)//��⵽3�ι����ź�
		{
			MC.ZCD_CNT=0;
			MC.DELAY30=MC.DELAY30*MC.ADJ30;
			MC.STATE=2;
		}
	}
}

//A������⺯��,A��BEMF�ɸ߱��
void ZCD_A_DOWN(void)
{
	//��ʱ30�ȼ���
	MC.DELAY30++;
	if(ADC_Data[BIT_A]<ADC_Data[BIT_N])//��⵽����
	{
		MC.ZCD_CNT++;
		if(MC.ZCD_CNT==8)//��⵽3�ι����ź�
		{
			MC.ZCD_CNT=0;
			MC.DELAY30=MC.DELAY30*MC.ADJ30;
			MC.STATE=2;
		}
	}
}

//B������⺯��,B��BEMF�ɸ߱��
void ZCD_B_DOWN(void)
{
	//��ʱ30�ȼ���
	MC.DELAY30++;
	if(ADC_Data[BIT_B]<ADC_Data[BIT_N])//��⵽����
	{
		MC.ZCD_CNT++;
		if(MC.ZCD_CNT==8)//��⵽3�ι����ź�
		{
			MC.ZCD_CNT=0;
			MC.DELAY30=MC.DELAY30*MC.ADJ30;
			MC.STATE=2;
		}
	}
}

//C������⺯��,C��BEMF�ɸ߱��
void ZCD_C_DOWN(void)
{
	//��ʱ30�ȼ���
	MC.DELAY30++;
	if(ADC_Data[BIT_C]<ADC_Data[BIT_N])//��⵽����
	{
		DEBUG_PIN_LOW;
		MC.ZCD_CNT++;
		if(MC.ZCD_CNT==8)//��⵽3�ι����ź�
		{
			MC.ZCD_CNT=0;
			MC.DELAY30=MC.DELAY30*MC.ADJ30;
			MC.STATE=2;
		}
	}
}

//�������ຯ��
void phase_change(void)
{
	if(MC.CCW)//��ת
	{
		switch(MC.PHA_CNT)
		{
			case 0: MOS_ABON(); break;
			case 1: MOS_ACON(); break;
			case 2: MOS_BCON(); break;
			case 3: MOS_BAON(); break;
			case 4: MOS_CAON(); break;
			case 5: MOS_CBON(); break;
			default: break;
		}
	}
	else//��ת
	{
		switch(MC.PHA_CNT)
		{
			case 5: MOS_ABON(); break;
			case 4: MOS_ACON(); break;
			case 3: MOS_BCON(); break;
			case 2: MOS_BAON(); break;
			case 1: MOS_CAON(); break;
			case 0: MOS_CBON(); break;
			default: break;
		}
	}
}

//AB
void MOS_ABON(void)
{
	MOSAP_PWM(MC.DUTY);
	MOSAN_OFF;
	MOSBP_OFF;
	MOSBN_ON;
	MOSCP_OFF;
	MOSCN_OFF;
}

//AC
void MOS_ACON(void)
{
	MOSAP_PWM(MC.DUTY);
	MOSAN_OFF;
	MOSBP_OFF;
	MOSBN_OFF;
	MOSCP_OFF;
	MOSCN_ON;
}

//BC
void MOS_BCON(void)
{
	MOSAP_OFF;
	MOSAN_OFF;
	MOSBP_PWM(MC.DUTY);
	MOSBN_OFF;
	MOSCP_OFF;
	MOSCN_ON;
}

//BA
void MOS_BAON(void)
{
	MOSAP_OFF;
	MOSAN_ON;
	MOSBP_PWM(MC.DUTY);
	MOSBN_OFF;
	MOSCP_OFF;
	MOSCN_OFF;
}

//CA
void MOS_CAON(void)
{
	MOSAP_OFF;
	MOSAN_ON;
	MOSBP_OFF;
	MOSBN_OFF;
	MOSCP_PWM(MC.DUTY);
	MOSCN_OFF;
}

//CB
void MOS_CBON(void)
{
	MOSAP_OFF;
	MOSAN_OFF;
	MOSBP_OFF;
	MOSBN_ON;
	MOSCP_PWM(MC.DUTY);
	MOSCN_OFF;
}

