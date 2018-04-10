/*
*	CBall.cpp: 源文件
*	CBall类的实现
*/

#include "stdafx.h"
#include "CBall.h"
#include "BalloohaDlg.h"
#include <cmath>
#include <time.h>

CBall::CBall(int x, int y) 
: m_nX(x), m_nY(y), m_bErase(false)
{
	// 构造函数
	// 指定随机半径
	srand(time(0));
	m_nR = rand() % (MAX_BALL_INIT_SIZE-MIN_BALL_INIT_SIZE) + MIN_BALL_INIT_SIZE;

	// 半径越小，移动越快（两次移动间隔越短）
	m_uElapse = INIT_MOVE_ELAPSE + m_nR;
	// 随机指定一种颜色
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
	// 球的移动
	// 使用极坐标，可向各个方向运动
	double dAngle = 0;
	int nStepX = 0, nStepY = 0;	// X、Y方向步长

	Sleep(m_uElapse);	// 延时

	dAngle = (double)((double)rand() / RAND_MAX) * 2 * 3.14;
	nStepX = (int)STEP_SIZE*sin(dAngle);
	nStepY = (int)STEP_SIZE*cos(dAngle);

	if ((m_nX + nStepX - m_nR)<LEFT_BORDER || (m_nX + nStepX + m_nR)>RIGHT_BORDER || \
		(m_nY + nStepY - m_nR)<UPPER_BORDER || (m_nY + nStepY + m_nR)>LOWER_BORDER)
		// 越界判断
		// 此处可能造成一定卡顿现象（因为不动所以看起来像卡了）
		return;

	// 修改坐标
	m_nX = m_nX + nStepX;
	m_nY = m_nY + nStepY;
}

int CBall::Detect(std::list<CBall>& balls)
{
	// 检测与其它球的接触情况
	// 若被吃返回-1
	double dDist = 0;

	for (auto itr = balls.begin(); itr != balls.end(); itr++)
	{
		dDist = sqrt(double((itr->m_nX - m_nX)*(itr->m_nX - m_nX) + (itr->m_nY - m_nY)*(itr->m_nY - m_nY)));	// 计算圆心距
		if (dDist <= abs(itr->m_nR - m_nR))
		{
			// 当圆心距小于等于两圆半径差时
			if (m_nR == itr->m_nR)
			{
				// 半径相同时，不反应
				continue;
			}
			else if (m_nR > itr->m_nR)
			{
				// 半径大于对方，吃掉它
				// 能吃多个球
				// 此处简化，不考虑连环吃的情况，只算作一个
				Eat(*itr);
			}
			else
			{
				// 半径比对方小，被吃
				// 只能被一个球吃
				if(itr->Eat(*this))
					return -1;
			}
		}
	}

	return 0;
}

void CBall::Draw(CDC &dc)
{
	// 绘制球（圆）
	CBrush iBrush, *pOldBrush;	// 画刷
	CPen iPen, *pOldPen;	// 画笔
	CRgn iRgn;

	iBrush.CreateSolidBrush(m_uColor);
	iPen.CreatePen(PS_SOLID, 0, m_uColor);	// 线宽为0，即去掉轮廓
	pOldBrush = dc.SelectObject(&iBrush);
	pOldPen = dc.SelectObject(&iPen);

	dc.Ellipse(m_nX - m_nR, m_nY - m_nR, m_nX + m_nR, m_nY + m_nR);	// 画圆

	dc.SelectObject(pOldBrush);	// 换回旧画刷
	dc.SelectObject(pOldPen);	// 旧画笔

	iBrush.DeleteObject();
	iRgn.DeleteObject();
}

void CBall::Grow(unsigned int n)
{
	// 球的成长
	while (n--)
	{
		m_nR += GROW_SPEED;
		++m_uElapse;
	}

	// 超过最大尺寸的修正
	if (m_nR >= MAX_BALL_SIZE)
	{
		m_nR = MAX_BALL_SIZE;
		m_uElapse = MAX_BALL_SIZE + INIT_MOVE_ELAPSE;
	}

	// 边界处的位置修正
	if ((m_nX - m_nR) < LEFT_BORDER)
	{
		// 左
		m_nX = LEFT_BORDER + m_nR;
	}
	else if ((m_nX + m_nR) > RIGHT_BORDER)
	{
		// 右
		m_nX = RIGHT_BORDER - m_nR;
	}
	if ((m_nY - m_nR) < UPPER_BORDER)
	{
		// 上
		m_nY = UPPER_BORDER + m_nR;
	}
	else if ((m_nY + m_nR) > LOWER_BORDER)
	{
		// 下
		m_nY = LOWER_BORDER - m_nR;
	}
}

bool CBall::Eat(CBall& prey)
{
	// 球的捕食动作
	// 返回是否捕食成功
	// 已经达到最大尺寸的球不允许捕食
	if (m_nR >= MAX_BALL_SIZE)
		return false;

	// 用半径代表等级，吃一个球可以得到和其等级相同的成长度
	Grow(prey.m_nR);
	prey.m_bErase = true;	// 标志被捕食者生命结束

	return true;
}