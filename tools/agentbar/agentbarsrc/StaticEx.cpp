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

#include "StaticEx.h"

CStaticEx::CStaticEx()
{
}

CStaticEx::~CStaticEx()
{
}

void CStaticEx::PreSubclassWindow() 
{
	// �޸ķ��Ϊ�Ի�
	ModifyStyle(0, BS_OWNERDRAW);
	CStatic::PreSubclassWindow();
}

BEGIN_MESSAGE_MAP(CStaticEx, CStatic)
ON_WM_ERASEBKGND()
ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CStaticEx::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

void CStaticEx::OnPaint() 
{
    CPaintDC dc(this); 
        
	m_font.CreatePointFont(m_Fontsize, m_FontFacename);	// �����Զ��������
	CFont *pOldFont = dc.SelectObject(&m_font);
	CSize sz = dc.GetTextExtent(m_WindowText);			// ������ֵĳ����Ա�ı�Static�ؼ��Ĵ�С
	m_brush.CreateSolidBrush(m_bkColor);				// ����������ˢ
	CRect rect;
	rect.left = m_nptx;
	rect.top  = m_npty;
	rect.right = rect.left + m_nLen;
	rect.bottom = rect.top + sz.cy;
	MoveWindow(&rect);
	GetClientRect(&rect);
	CBrush * pOldBrush = dc.SelectObject(&m_brush);
	dc.FillRect(&rect,&m_brush);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_textColor);

	int n = m_nLen - sz.cx;
	n >>= 1;
	dc.TextOut(rect.left + n, rect.top, m_WindowText);	// ������ʾ

	dc.SelectObject(pOldFont);							// ��ԭDCΪĬ��״̬
	dc.SelectObject(pOldBrush);
	m_brush.DeleteObject();								// ɾ����Դ����
	m_font.DeleteObject();
}

void CStaticEx::SetFont(int nSize, const char *strFaceName)
{
	// ��������
    m_Fontsize = nSize;
	m_FontFacename = strFaceName;
}

void CStaticEx::SetColor(COLORREF bkColor, COLORREF textColor)
{
	// �������ֵı���ɫ��ǰ��ɫ
    m_bkColor = bkColor; 
	m_textColor = textColor;
}

void CStaticEx::SetWindowpos(int nx, int ny)
{
    m_nptx = nx;
	m_npty = ny;
}

void CStaticEx::SetStaticexText(const char *strtext)
{
	m_WindowText = strtext;
}

CString &CStaticEx::GetStaticexText()
{
    return m_WindowText;
}

void CStaticEx::SetWidth(int len)
{
	m_nLen = len;
}
