/*
*	CBall.cpp: Դ�ļ�
*	CBall���ʵ��
*/

#include "stdafx.h"
#include "CBall.h"
#include "BalloohaDlg.h"
#include <cmath>
#include <time.h>

CBall::CBall(int x, int y) 
: m_nX(x), m_nY(y), m_bErase(false)
{
	// ���캯��
	// ָ������뾶
	srand(time(0));
	m_nR = rand() % (MAX_BALL_INIT_SIZE-MIN_BALL_INIT_SIZE) + MIN_BALL_INIT_SIZE;

	// �뾶ԽС���ƶ�Խ�죨�����ƶ����Խ�̣�
	m_uElapse = INIT_MOVE_ELAPSE + m_nR;
	// ���ָ��һ����ɫ
	m_uColor = rand() % 3;
	switch (m_uColor)
	{
	case 0:
		m_uColor = COLOR_RED;
		break;
	case 1:
		m_uColor = COLOR_BLUE;
		break;
	case 2:
		m_uColor = COLOR_GREEN;
		break;
	default:
		;
	}
}

void CBall::Move()
{
	// ����ƶ�
	// ʹ�ü����꣬������������˶�
	double dAngle = 0;
	int nStepX = 0, nStepY = 0;	// X��Y���򲽳�

	Sleep(m_uElapse);	// ��ʱ

	dAngle = (double)((double)rand() / RAND_MAX) * 2 * 3.14;
	nStepX = (int)STEP_SIZE*sin(dAngle);
	nStepY = (int)STEP_SIZE*cos(dAngle);

	if ((m_nX + nStepX - m_nR)<LEFT_BORDER || (m_nX + nStepX + m_nR)>RIGHT_BORDER || \
		(m_nY + nStepY - m_nR)<UPPER_BORDER || (m_nY + nStepY + m_nR)>LOWER_BORDER)
		// Խ���ж�
		// �˴��������һ������������Ϊ�������Կ��������ˣ�
		return;

	// �޸�����
	m_nX = m_nX + nStepX;
	m_nY = m_nY + nStepY;
}

int CBall::Detect(std::list<CBall>& balls)
{
	// �����������ĽӴ����
	// �����Է���-1
	double dDist = 0;

	for (auto itr = balls.begin(); itr != balls.end(); itr++)
	{
		dDist = sqrt(double((itr->m_nX - m_nX)*(itr->m_nX - m_nX) + (itr->m_nY - m_nY)*(itr->m_nY - m_nY)));	// ����Բ�ľ�
		if (dDist <= abs(itr->m_nR - m_nR))
		{
			// ��Բ�ľ�С�ڵ�����Բ�뾶��ʱ
			if (m_nR == itr->m_nR)
			{
				// �뾶��ͬʱ������Ӧ
				continue;
			}
			else if (m_nR > itr->m_nR)
			{
				// �뾶���ڶԷ����Ե���
				// �ܳԶ����
				// �˴��򻯣������������Ե������ֻ����һ��
				Eat(*itr);
			}
			else
			{
				// �뾶�ȶԷ�С������
				// ֻ�ܱ�һ�����
				if(itr->Eat(*this))
					return -1;
			}
		}
	}

	return 0;
}

void CBall::Draw(CDC &dc)
{
	// ������Բ��
	CBrush iBrush, *pOldBrush;	// ��ˢ
	CPen iPen, *pOldPen;	// ����
	CRgn iRgn;

	iBrush.CreateSolidBrush(m_uColor);
	iPen.CreatePen(PS_SOLID, 0, m_uColor);	// �߿�Ϊ0����ȥ������
	pOldBrush = dc.SelectObject(&iBrush);
	pOldPen = dc.SelectObject(&iPen);

	dc.Ellipse(m_nX - m_nR, m_nY - m_nR, m_nX + m_nR, m_nY + m_nR);	// ��Բ

	dc.SelectObject(pOldBrush);	// ���ؾɻ�ˢ
	dc.SelectObject(pOldPen);	// �ɻ���

	iBrush.DeleteObject();
	iRgn.DeleteObject();
}

void CBall::Grow(unsigned int n)
{
	// ��ĳɳ�
	while (n--)
	{
		m_nR += GROW_SPEED;
		++m_uElapse;
	}

	// �������ߴ������
	if (m_nR >= MAX_BALL_SIZE)
	{
		m_nR = MAX_BALL_SIZE;
		m_uElapse = MAX_BALL_SIZE + INIT_MOVE_ELAPSE;
	}

	// �߽紦��λ������
	if ((m_nX - m_nR) < LEFT_BORDER)
	{
		// ��
		m_nX = LEFT_BORDER + m_nR;
	}
	else if ((m_nX + m_nR) > RIGHT_BORDER)
	{
		// ��
		m_nX = RIGHT_BORDER - m_nR;
	}
	if ((m_nY - m_nR) < UPPER_BORDER)
	{
		// ��
		m_nY = UPPER_BORDER + m_nR;
	}
	else if ((m_nY + m_nR) > LOWER_BORDER)
	{
		// ��
		m_nY = LOWER_BORDER - m_nR;
	}
}

bool CBall::Eat(CBall& prey)
{
	// ��Ĳ�ʳ����
	// �����Ƿ�ʳ�ɹ�
	// �Ѿ��ﵽ���ߴ��������ʳ
	if (m_nR >= MAX_BALL_SIZE)
		return false;

	// �ð뾶����ȼ�����һ������Եõ�����ȼ���ͬ�ĳɳ���
	Grow(prey.m_nR);
	prey.m_bErase = true;	// ��־����ʳ����������

	return true;
}