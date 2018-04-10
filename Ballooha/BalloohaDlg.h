
// BalloohaDlg.h : ͷ�ļ�
//

#pragma once

#include "Resource.h"

class CBall;

// CBalloohaDlg �Ի���
class CBalloohaDlg : public CDialogEx
{
// ����
public:
	CBalloohaDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CBalloohaDlg();	// ��������

// �Ի�������
	enum { IDD = IDD_BALLOOHA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	std::list<CBall> m_lstBalls;	// ������
	CCriticalSection m_iCS;	// �ٽ�������
	std::list<CBall>::iterator m_itrLast;	// ���ɵ���һ����ĵ�����
	CDC m_iMemDC;	// ����˫�����е��ڴ��豸������
	CBitmap m_iMemBit;	// ����˫�����е�λͼ

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void DrawBalls();	// ������
	void InitDB();	// ˫�����ʼ��
};

// �̺߳�������
UINT BallThreadFunc(LPVOID param);