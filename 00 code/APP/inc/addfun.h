#ifndef __ADDFUN_H__
#define __ADDFUN_H__

//ͷ�ļ�
#include "include.h"

//�궨��
#define 	CHECK_DUTY 		200			//����õ�ռ�ձ�Ϊ10%
#define 	SHORT_CURRENT 21845  		//MOS��·���������ֵ
#define   POPEN_VOL			0					//���ű�MOS��ͨ��ѹ�����ֵ
#define 	NOPEN_VOL 		4095			//���ű�MOS��ͨ��ѹ�����ֵ


//��������
extern u16 OFFSET_A,OFFSET_B,OFFSET_C,OFFSET_N,OFFSET_I;

//��������
void MOSSHORT_Check(void);
void MOSOPEN_Check(void);
void OFFSET2BAT_Read(void);

#endif
