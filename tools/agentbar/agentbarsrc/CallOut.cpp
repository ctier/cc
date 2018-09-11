/*
 * Copyright 2002-2014 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      CC/LICENSE
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "CallOut.h"
#include "Info.h"

IMPLEMENT_DYNAMIC(CCallOut, CDialog)
CCallOut::CCallOut(CWnd* pParent /*=NULL*/)
	: CDialog(CCallOut::IDD, pParent), m_posX(0), m_posY(0), m_intAgentOrOut(0)
{
	m_BrushBK.CreateSolidBrush(RGB(250,250,250));
}

CCallOut::~CCallOut()
{
	m_BrushBK.DeleteObject();
}

void CCallOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_IN, m_rdoAgent);
	DDX_Control(pDX, IDC_RADIO_OUT, m_rdoOut);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_NUMBER, m_editNumber);
	DDX_Text(pDX, IDC_EDIT_NUMBER, m_strNumber);
}

BOOL CCallOut::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rect;
	GetWindowRect(&rect);
	int w = rect.Width();
	int h = rect.Height();
	rect.left = m_posX;
	rect.top = m_posY;
	rect.right = rect.left + w;
	rect.bottom = rect.top + h;
	MoveWindow(&rect);

	if(m_intAgentOrOut == 0)
	{
		m_rdoAgent.SetCheck(0);
		m_rdoOut.SetCheck(1);
	}
	else
	{
		m_rdoAgent.SetCheck(1);
		m_rdoOut.SetCheck(0);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BEGIN_MESSAGE_MAP(CCallOut, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CCallOut::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if(m_strNumber.Trim().IsEmpty())
	{
		CInfo info;
		info.SetInfo(IDI_ICON_ERROR, "��ϯ���Ż����ߺ��벻��Ϊ�գ�");
		info.DoModal();
		return;
	}

	int agentcheck = m_rdoAgent.GetCheck();
	int outcheck = m_rdoOut.GetCheck();

	if(agentcheck ==0 && outcheck == 1)
	{
		m_intAgentOrOut = 0;//����
	}
	else if(agentcheck == 1 && outcheck == 0)
	{
		m_intAgentOrOut = 1;//��ϯ
	}
	else
	{
		CInfo info;
		info.SetInfo(IDI_ICON_ERROR, "��ϯ��������Ҫ����ѡ��һ�֣�");
		info.DoModal();
		return;
	}
	//
	OnOK();
}

HBRUSH CCallOut::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	switch(pWnd->GetDlgCtrlID())
	{
	case IDC_RADIO_IN:
	case IDC_RADIO_OUT:
		pDC->SetBkColor(RGB(250,250,250));
		return m_BrushBK;
		break;
	default:
		break;
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

BOOL CCallOut::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	CBrush *p_OldBrush = NULL;

	p_OldBrush = pDC->SelectObject(&m_BrushBK);
	pDC->GetClipBox(&rect);
	pDC->FillRect(&rect, &m_BrushBK);
	pDC->SelectObject(p_OldBrush);

	p_OldBrush = NULL;

	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CCallOut::SetPosition(LONG x, LONG y)
{
	m_posX = x;
	m_posY = y;
}

int CCallOut::GetAgentOrOut()
{
	return m_intAgentOrOut;
}

CString &CCallOut::GetNumber()
{
	return m_strNumber;
}
