
// BalloohaDlg.h : 头文件
//

#pragma once

#include "Resource.h"

class CBall;

// CBalloohaDlg 对话框
class CBalloohaDlg : public CDialogEx
{
// 构造
public:
	CBalloohaDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CBalloohaDlg();	// 析构函数

// 对话框数据
	enum { IDD = IDD_BALLOOHA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	std::list<CBall> m_lstBalls;	// 球链表
	CCriticalSection m_iCS;	// 临界区对象
	std::list<CBall>::iterator m_itrLast;	// 生成的上一个球的迭代器
	CDC m_iMemDC;	// 用作双缓冲中的内存设备上下文
	CBitmap m_iMemBit;	// 用作双缓冲中的位图

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void DrawBalls();	// 绘制球
	void InitDB();	// 双缓冲初始化
};

// 线程函数声明
UINT BallThreadFunc(LPVOID param);