// SIPPhone.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������
#include "phonelib.h"


// CSIPPhoneApp:
// �йش����ʵ�֣������ SIPPhone.cpp
//

class CSIPPhoneApp : public CWinApp
{
public:
	CSIPPhoneApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSIPPhoneApp theApp;
