// CCAgentBarTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CCCAgentBarTestApp:
// �йش����ʵ�֣������ CCAgentBarTest.cpp
//

class CCCAgentBarTestApp : public CWinApp
{
public:
	CCCAgentBarTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCCAgentBarTestApp theApp;
