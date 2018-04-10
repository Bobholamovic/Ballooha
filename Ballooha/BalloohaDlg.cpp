
// BalloohaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ballooha.h"
#include "BalloohaDlg.h"
#include "afxdialogex.h"
#include "CBall.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CBalloohaDlg �Ի���



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


// CBalloohaDlg ��Ϣ�������

BOOL CBalloohaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MAXIMIZE);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	// ��ʼ��˫����
	InitDB();
	// �򿪶�ʱ������ʼˢ����Ļ
	SetTimer(REFRESH_TIMER_ID, REFRESH_RATE, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBalloohaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBalloohaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT BallThreadFunc(LPVOID param)
{
	// �����̺߳���
	CBalloohaDlg* pDlg = reinterpret_cast<CBalloohaDlg*>(param);	// ��ȡ�Ի������ָ��
	auto itr = pDlg->m_itrLast;
	CBall& iBall = *itr;

	while (1)
	{
		// ���Բ��
		// ���ٽ���ͬ����ֹ���ֵ���������
		pDlg->m_iCS.Lock();	// �����ٽ���
		// ���ö�·����
		if (iBall.GetEraseFlag() || iBall.Detect(pDlg->m_lstBalls) == -1)
		{
			// ����������������˳��߳�
			pDlg->m_lstBalls.erase(itr);
			pDlg->m_iCS.Unlock();
			return 0;
		}
		pDlg->m_iCS.Unlock();	// �뿪�ٽ���

		// ���ƶ�
		iBall.Move();
	}

	return 1;
}

void CBalloohaDlg::DrawBalls()
{
	CDC* pDC = GetDC();

	// ���λͼ
	m_iMemDC.FillSolidRect(LEFT_BORDER, UPPER_BORDER, RIGHT_BORDER - LEFT_BORDER, LOWER_BORDER - UPPER_BORDER, COLOR_WHITE);

	// ��ͼ
	m_iCS.Lock();
	for (auto itr = m_lstBalls.begin(); itr != m_lstBalls.end(); itr++)
		itr->Draw(m_iMemDC);
	m_iCS.Unlock();

	// ���ڴ���ͼ��������Ļ
	pDC->BitBlt(LEFT_BORDER, UPPER_BORDER, RIGHT_BORDER - LEFT_BORDER, LOWER_BORDER - UPPER_BORDER, &m_iMemDC, LEFT_BORDER, UPPER_BORDER, SRCCOPY);

	// �ͷ��ڴ�
	ReleaseDC(pDC);
}

void CBalloohaDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// ��굥���������Ӧ
	// �豸����ת��Ϊ�߼�����
	CClientDC iDC(this);
	iDC.DPtoLP(&point);

	// Խ���ж�
	if ((point.x - MAX_BALL_INIT_SIZE) < LEFT_BORDER || (point.x + MAX_BALL_INIT_SIZE) > RIGHT_BORDER || \
		(point.y - MAX_BALL_INIT_SIZE) < UPPER_BORDER || (point.y + MAX_BALL_INIT_SIZE) > LOWER_BORDER)
		return;
	// �Ƿ񳬹�������
	if (m_lstBalls.size() >= MAX_BALL_NUM)
		return;
	// �½������Բ��λ���������
	m_lstBalls.push_back(CBall(point.x, point.y));
	m_itrLast = --m_lstBalls.end();

	// �����߳�
	AfxBeginThread(BallThreadFunc, (LPVOID) this);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CBalloohaDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnClose();
}

CBalloohaDlg::~CBalloohaDlg()
{
	// ��������
	// ��֤�����߳��˳�
	// ����������ã������ǵ��������ö���ռ䱣������������д
	for (auto itr = m_lstBalls.begin(); itr != m_lstBalls.end(); itr++)
		itr->SetEraseFlag();
	Sleep(1000);
}

void CBalloohaDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

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

	m_iMemDC.CreateCompatibleDC(pDC);	// ����һ���뵱ǰ��Ļ���ݵ��ڴ��豸������
	// �˴��õĿ������豸���ꣿ
	// ֻ�ðѿͻ������Ͻǿ�ʼ����һ��ٹ�ȥ��
	m_iMemBit.CreateCompatibleBitmap(pDC, RIGHT_BORDER, LOWER_BORDER);

	m_iMemDC.SelectObject(&m_iMemBit);

	ReleaseDC(pDC);
}