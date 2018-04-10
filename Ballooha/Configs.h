/*
 *	Configs.h: ͷ�ļ�
 *	ָ��һЩ���ò���
*/

#pragma once

// ��
#define MAX_BALL_NUM 50	// ����������
#define MAX_BALL_SIZE 50	// ������ߴ磨�뾶��
#define MAX_BALL_INIT_SIZE 10	// �������ʼ�ߴ�
#define MIN_BALL_INIT_SIZE 3	// �����С��ʼ�ߴ�
#define GROW_SPEED 1	// �ɳ��ٶȣ�ÿ�γɳ���С��
#define INIT_MOVE_ELAPSE 100	//	��ʼ�ƶ����ʱ�䣨ms��
#define STEP_SIZE 50	// �ƶ�����

// �߽�λ��
#define LEFT_BORDER 100
#define RIGHT_BORDER 1200
#define UPPER_BORDER 100
#define LOWER_BORDER 600

// ��ɫ
#define COLOR_RED (RGB(255,0,0))
#define COLOR_BLUE (RGB(0,0,255))
#define COLOR_GREEN (RGB(0,255,0))
#define COLOR_WHITE (RGB(255, 255, 255))

// ��������
#define REFRESH_RATE 50	// ֡ˢ�����ʣ�ͼ�����ʱ��������λms��
#define REFRESH_TIMER_ID 0	// ˢ���ö�ʱ��ID��