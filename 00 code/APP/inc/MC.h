#ifndef __MC_H__
#define __MC_H__

//ͷ�ļ�
#include "include.h"

//�궨��
#define MOSAP_PWM(duty) 	TIM_SetCompare1(TIM1,duty);
#define MOSAN_ON					GPIO_SetBits(GPIOB, GPIO_Pin_15);
#define MOSBP_PWM(duty)		TIM_SetCompare2(TIM1,duty);
#define MOSBN_ON					GPIO_SetBits(GPIOB, GPIO_Pin_14);
#define MOSCP_PWM(duty)		TIM_SetCompare3(TIM1,duty);
#define MOSCN_ON					GPIO_SetBits(GPIOB, GPIO_Pin_13);
#define MOSAP_OFF 				TIM_SetCompare1(TIM1,0);
#define MOSAN_OFF 				GPIO_ResetBits(GPIOB, GPIO_Pin_15);
#define MOSBP_OFF					TIM_SetCompare2(TIM1,0);
#define MOSBN_OFF					GPIO_ResetBits(GPIOB, GPIO_Pin_14);
#define MOSCP_OFF					TIM_SetCompare3(TIM1,0);
#define MOSCN_OFF					GPIO_ResetBits(GPIOB, GPIO_Pin_13);

#define ALLMOS_OFF \
											do{													\
													TIM_SetCompare1(TIM1,0);\
													TIM_SetCompare2(TIM1,0);\
													TIM_SetCompare3(TIM1,0);\
													GPIO_ResetBits(GPIOB, GPIO_Pin_13);\
													GPIO_ResetBits(GPIOB, GPIO_Pin_14);\
													GPIO_ResetBits(GPIOB, GPIO_Pin_15);\
											}while(0);//�ر�����MOS
											
#define  	MC_DEFAULTS   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} 	// ��ʼ������
#define 	CLOSED_LOOP 	1							//����ջ���־
#define 	POLES 				7							//����������������Լ���ת����Ӱ��

#define FirstOrder_LPF(OLD, NEW, COE) do{NEW = (1-COE)*NEW + COE*OLD;}while(0); //һ�׵�ͨ�˲�
#define LOOP 	0 	//0����������1���ٶȻ�+��������2������
#define OUP   0		//��Ƿѹ����
#define OCP   0		//��������
#define OPP 	0		//1������ȱ�ౣ����0���ر�ȱ�ౣ��
#define MSLP 	0		//1��������ת������0���رն�ת����

//��������										
typedef struct {
	u8		CCW;      //1����ת��0������
	u8		PHA_CNT;  //�������
	u8		STATE;		//���״̬
	u8		ZCD_CNT;	//�����������
	u8		BAT;			//������࣬1����2S��2����3S��3����4S
	u8		FAULT;		//���ϼ�¼��//1-6��ʾA+,B+,C+,A-,B-,C-�Ķ�·���ϣ�7-12��ʾA+,B+,C+,A-,B-,C-���ܿ�ͨ���ϣ�13-17:��ѹ��Ƿѹ����������ת��ȱ��,18:��������
	u16		DUTY;     //ռ�ձ�
	u16   DELAY30;	//��ʱ30�ȼ���
	u16		CYCLES;		//���ת����Ȧ������
	u16		SPEED;		//���ת��
	u16   AI;				//A�����
	u16		BI;				//B�����
	u16		CI;				//C�����
	u16		OVER_VOL;	//��ѹֵ
	u16		UNDER_VOL;//Ƿѹֵ
	float ADJ30;		//��ʱ30�ȵ���ϵ��0.7��������ȫ��ʱǰ30���߹���ʱ�䣬��Ҫ�ʵ���С,��С�ή�͵��Ч�ʣ���������������䵼�µ������
}Motor_Control;

extern Motor_Control MC;

//��������
void PARAM_Init(void);
void MOTOR_Start(void);
void phase_change(void);
void MOS_ABON(void);
void MOS_ACON(void);
void MOS_BCON(void);
void MOS_BAON(void);
void MOS_CAON(void);
void MOS_CBON(void);


void ZCD_A_UP(void);
void ZCD_B_UP(void);
void ZCD_C_UP(void);
void ZCD_A_DOWN(void);
void ZCD_B_DOWN(void);
void ZCD_C_DOWN(void);

#endif
