
// RestfulAgentTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRestfulAgentTestApp:
// �йش����ʵ�֣������ RestfulAgentTest.cpp
//

class CRestfulAgentTestApp : public CWinApp
{
public:
	CRestfulAgentTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRestfulAgentTestApp theApp;