
// MyWireShark.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyWireSharkApp:
// �йش����ʵ�֣������ MyWireShark.cpp
//

class CMyWireSharkApp : public CWinApp
{
public:
	CMyWireSharkApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyWireSharkApp theApp;