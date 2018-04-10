
// BalloohaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Ballooha.h"
#include "BalloohaDlg.h"
#include "afxdialogex.h"
#include "CBall.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBalloohaDlg 对话框



CBalloohaDlg::CBalloohaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBalloohaDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBalloohaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBalloohaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CBalloohaDlg 消息处理程序

BOOL CBalloohaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MAXIMIZE);

	// TODO:  在此添加额外的初始化代码
	// 初始化双缓冲
	InitDB();
	// 打开定时器，开始刷新屏幕
	SetTimer(REFRESH_TIMER_ID, REFRESH_RATE, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBalloohaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBalloohaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBalloohaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT BallThreadFunc(LPVOID param)
{
	// 球工作线程函数
	CBalloohaDlg* pDlg = reinterpret_cast<CBalloohaDlg*>(param);	// 获取对话框对象指针
	auto itr = pDlg->m_itrLast;
	CBall& iBall = *itr;

	while (1)
	{
		// 检测圆心
		// 用临界区同步防止出现迭代器错误
		pDlg->m_iCS.Lock();	// 进入临界区
		// 利用短路法则
		if (iBall.GetEraseFlag() || iBall.Detect(pDlg->m_lstBalls) == -1)
		{
			// 若被吃则清除该球并退出线程
			pDlg->m_lstBalls.erase(itr);
			pDlg->m_iCS.Unlock();
			return 0;
		}
		pDlg->m_iCS.Unlock();	// 离开临界区

		// 球移动
		iBall.Move();
	}

	return 1;
}

void CBalloohaDlg::DrawBalls()
{
	CDC* pDC = GetDC();

	// 清空位图
	m_iMemDC.FillSolidRect(LEFT_BORDER, UPPER_BORDER, RIGHT_BORDER - LEFT_BORDER, LOWER_BORDER - UPPER_BORDER, COLOR_WHITE);

	// 绘图
	m_iCS.Lock();
	for (auto itr = m_lstBalls.begin(); itr != m_lstBalls.end(); itr++)
		itr->Draw(m_iMemDC);
	m_iCS.Unlock();

	// 将内存中图拷贝到屏幕
	pDC->BitBlt(LEFT_BORDER, UPPER_BORDER, RIGHT_BORDER - LEFT_BORDER, LOWER_BORDER - UPPER_BORDER, &m_iMemDC, LEFT_BORDER, UPPER_BORDER, SRCCOPY);

	// 释放内存
	ReleaseDC(pDC);
}

void CBalloohaDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	// 鼠标单击左键后响应
	// 设备坐标转换为逻辑坐标
	CClientDC iDC(this);
	iDC.DPtoLP(&point);

	// 越界判断
	if ((point.x - MAX_BALL_INIT_SIZE) < LEFT_BORDER || (point.x + MAX_BALL_INIT_SIZE) > RIGHT_BORDER || \
		(point.y - MAX_BALL_INIT_SIZE) < UPPER_BORDER || (point.y + MAX_BALL_INIT_SIZE) > LOWER_BORDER)
		return;
	// 是否超过最大个数
	if (m_lstBalls.size() >= MAX_BALL_NUM)
		return;
	// 新建球对象，圆心位于鼠标点击处
	m_lstBalls.push_back(CBall(point.x, point.y));
	m_itrLast = --m_lstBalls.end();

	// 创建线程
	AfxBeginThread(BallThreadFunc, (LPVOID) this);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CBalloohaDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnClose();
}

CBalloohaDlg::~CBalloohaDlg()
{
	// 析构函数
	// 保证所有线程退出
	// 这个方法不好，但考虑到不想再用额外空间保存句柄，先这样写
	for (auto itr = m_lstBalls.begin(); itr != m_lstBalls.end(); itr++)
		itr->SetEraseFlag();
	Sleep(1000);
}

void CBalloohaDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	switch (nIDEvent)
	{
	case REFRESH_TIMER_ID:
		DrawBalls();
		break;
	default:
		;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CBalloohaDlg::InitDB()
{
	CDC* pDC = GetDC();

	m_iMemDC.CreateCompatibleDC(pDC);	// 创建一个与当前屏幕兼容的内存设备上下文
	// 此处用的可能是设备坐标？
	// 只好把客户区左上角开始整个一起抠过去了
	m_iMemBit.CreateCompatibleBitmap(pDC, RIGHT_BORDER, LOWER_BORDER);

	m_iMemDC.SelectObject(&m_iMemBit);

	ReleaseDC(pDC);
}