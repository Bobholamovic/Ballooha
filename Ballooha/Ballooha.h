
// Ballooha.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBalloohaApp: 
// �йش����ʵ�֣������ Ballooha.cpp
//

class CBalloohaApp : public CWinApp
{
public:
	CBalloohaApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBalloohaApp theApp;