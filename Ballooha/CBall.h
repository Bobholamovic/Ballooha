/*
 *	CBall.h: 头文件
 *	CBall类的接口
 */

#pragma once

class CBalloohaDlg;

class CBall
{
private:
	unsigned int m_uColor;	// 颜色

	// 圆心坐标
	int m_nX;
	int m_nY;

	int m_nR;	// 半径
	bool m_bErase;	// 指定该球是否被清除
	unsigned int m_uElapse;	// 刷新时间间隔，单位为ms

private:
	void Grow(unsigned int);	// 成长
	bool Eat(CBall&);	// 吃球

public:
	CBall(int, int);	// 构造函数
	~CBall() {}	// 析构函数

	void Move();	// 移动
	void Draw(CDC&);	// 绘图
	int Detect(std::list<CBall>&);	// 检测与其它圆接触情况

	inline const unsigned int GetElapse() const
	{
		// 获取刷新时间间隔
		return m_uElapse;
	}

	inline bool GetEraseFlag() const
	{
		// 获取清除状态
		return m_bErase;
	}

	inline void SetEraseFlag()
	{
		// 设定清除状态
		m_bErase = true;
	}

};

