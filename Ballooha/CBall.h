/*
 *	CBall.h: ͷ�ļ�
 *	CBall��Ľӿ�
 */

#pragma once

class CBalloohaDlg;

class CBall
{
private:
	unsigned int m_uColor;	// ��ɫ

	// Բ������
	int m_nX;
	int m_nY;

	int m_nR;	// �뾶
	bool m_bErase;	// ָ�������Ƿ����
	unsigned int m_uElapse;	// ˢ��ʱ��������λΪms

private:
	void Grow(unsigned int);	// �ɳ�
	bool Eat(CBall&);	// ����

public:
	CBall(int, int);	// ���캯��
	~CBall() {}	// ��������

	void Move();	// �ƶ�
	void Draw(CDC&);	// ��ͼ
	int Detect(std::list<CBall>&);	// ���������Բ�Ӵ����

	inline const unsigned int GetElapse() const
	{
		// ��ȡˢ��ʱ����
		return m_uElapse;
	}

	inline bool GetEraseFlag() const
	{
		// ��ȡ���״̬
		return m_bErase;
	}

	inline void SetEraseFlag()
	{
		// �趨���״̬
		m_bErase = true;
	}

};

