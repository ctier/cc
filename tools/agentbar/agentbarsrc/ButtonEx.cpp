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

#include "ButtonEx.h"
#include "ImgMgr.h"

CButtonEx::CButtonEx()
{
	m_bIsDefault = FALSE;
	m_bOver = m_bSelected = m_bTracking = FALSE;
	m_BrushBk.CreateSolidBrush(RGB(240,240,240));
}

CButtonEx::~CButtonEx()
{
}

void CButtonEx::TransparentBlt(HDC hdcDest,		// Ŀ��DC
					 int nXOriginDest,			// Ŀ��Xƫ��
					 int nYOriginDest,			// Ŀ��Yƫ��
					 int nWidthDest,			// Ŀ����
					 int nHeightDest,			// Ŀ��߶�
					 HDC hdcSrc,				// ԴDC
					 int nXOriginSrc,			// ԴX���
					 int nYOriginSrc,			// ԴY���
					 int nWidthSrc,				// Դ���
					 int nHeightSrc,			// Դ�߶�
					 UINT crTransparent)
{
	HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	// ��������λͼ
	HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);			// ������ɫ����λͼ
	HDC		hImageDC = CreateCompatibleDC(hdcDest);
	HDC		hMaskDC = CreateCompatibleDC(hdcDest);
	hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
	hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);

	// ��ԴDC�е�λͼ��������ʱDC��
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	else
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, 
		hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);

	// ����͸��ɫ
	SetBkColor(hImageDC, crTransparent);

	// ����͸������Ϊ��ɫ����������Ϊ��ɫ������λͼ
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);

	// ����͸������Ϊ��ɫ���������򱣳ֲ����λͼ
	SetBkColor(hImageDC, RGB(0,0,0));
	SetTextColor(hImageDC, RGB(255,255,255));
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// ͸�����ֱ�����Ļ���䣬�������ֱ�ɺ�ɫ
	SetBkColor(hdcDest,RGB(0xff,0xff,0xff));
	SetTextColor(hdcDest,RGB(0,0,0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// "��"����,��������Ч��
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);

	// ��ԭDC��Ĭ��״̬
	SelectObject(hImageDC, hOldImageBMP);
	DeleteDC(hImageDC);
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteDC(hMaskDC);
	DeleteObject(hImageBMP);
	DeleteObject(hMaskBMP);
}

// ���Owner Draw����
void CButtonEx::PreSubclassWindow()
{
	// �޸İ�ť�Ի�����
	ModifyStyle(0, BS_OWNERDRAW);
	CButton::PreSubclassWindow();

	if (m_nTypeStyle == BS_DEFPUSHBUTTON)
	{
		m_nTypeStyle = BS_PUSHBUTTON;
	}
}

void CButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// ��lpDrawItemStruct��ȡ�ؼ��������Ϣ
	CBitmap *pBit = CImgMgr::Instance().Get(m_bOver ? m_nIDBmpHover : m_nIDBmpDefault);
	if(pBit == NULL)
		return;

	CRect rect = lpDrawItemStruct->rcItem;
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);			// ��ð�ť��DC
	int nSaveDC = pDC->SaveDC();

	BITMAP bit;
	pBit->GetBitmap(&bit);
	rect.left = 0;
	rect.right = bit.bmWidth;
	rect.top = 0;
	rect.bottom = bit.bmHeight;
	MoveWindow(m_btPX, m_btPY, rect.Width(), rect.Height());	// ���ð�ť�Ĵ�СΪλͼ�Ĵ�С
	DrawButton(pDC, *pBit, &rect);
	pDC->RestoreDC(nSaveDC);									// �ָ���ťDCΪ��ʼ״̬
}

// �ڰ�ť�ϻ���͸����λͼ
void CButtonEx::DrawButton(CDC *pDC, CBitmap &bitmap, CRect *rect)
{
	COLORREF crMask = RGB(255,255,255);
	BITMAP bm;
	bitmap.GetBitmap(&bm);
	int nWidth = bm.bmWidth;
	int nHeight = bm.bmHeight;
	CDC ImageDC;
	ImageDC.CreateCompatibleDC(pDC);																						// ����һ���밴ťDC�����λͼDC
	CBitmap *pOldImageBMP = ImageDC.SelectObject(&bitmap);																	// ��λͼѡ��λͼDC
	TransparentBlt(pDC->m_hDC,0, 0, rect->Width(), rect->Height(),ImageDC.m_hDC,0,0,rect->Width(), rect->Height(),crMask);	// ����͸��λͼ
	ImageDC.SelectObject(pOldImageBMP);																						// �ָ�λͼDC�ĳ�ʼ״̬
}

BEGIN_MESSAGE_MAP(CButtonEx, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_ERASEBKGND()
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_WM_GETDLGCODE()
	ON_MESSAGE(BM_SETSTYLE, OnSetStyle)
END_MESSAGE_MAP()

void CButtonEx::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		// ����һ�����봰��ʱ�� ����һ��WM_MOUSELEAVE ��Ϣ
		TRACKMOUSEEVENT tme;

		tme.cbSize = sizeof(tme);

		tme.hwndTrack = m_hWnd;

		tme.dwFlags = TME_LEAVE;

		_TrackMouseEvent(&tme);
		m_bTracking = TRUE;
		if (!m_bOver)
		{
			m_bOver = TRUE;
			Invalidate(FALSE);
		}
	}
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CButton::OnMouseMove(nFlags, point);
}

// ����뿪��Ϣ������
LRESULT CButtonEx::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = false;
	m_bOver = FALSE;

	Invalidate(false);

	return TRUE;
}

// �Ի水ť��ɫ
BOOL CButtonEx::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CBrush * pOldBrush = pDC->SelectObject(&m_BrushBk);
	pDC->FillRect(&rect,&m_BrushBk);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}

BOOL CButtonEx::OnClicked() 
{
	m_bSelected = true;
	return FALSE;
}

// ����Ĭ��״̬���
UINT CButtonEx::OnGetDlgCode()
{
	UINT nCode = CButton::OnGetDlgCode();
	nCode |= (1 ? DLGC_DEFPUSHBUTTON : DLGC_UNDEFPUSHBUTTON);

	return nCode;
}

LRESULT CButtonEx::OnSetStyle(WPARAM wParam, LPARAM lParam)
{
	UINT nNewType = (UNIT)(wParam & BS_TYPEMASK);

	// ����Ĭ��״̬���
	if(nNewType == BS_DEFPUSHBUTTON)
	{
		m_bIsDefault = TRUE;
	}
	else if(nNewType == BS_PUSHBUTTON)
	{
		m_bIsDefault = FALSE;
	}

	return DefWindowProc(BM_SETSTYLE, (wParam & ~BS_TYPEMASK) | BS_OWNERDRAW, lParam);
}

// ���ð�ť��Ĭ��λͼ������Ƶ���ť�ϵ�λͼ
void CButtonEx::SetBitMap(int nIDBmpDefault, int nIDBmpHover)
{
	m_nIDBmpDefault = nIDBmpDefault;
	m_nIDBmpHover = nIDBmpHover;
}

// ���ð�ť�ڸ����ڵ�����λ��
void CButtonEx::SetButtonPos(int bx, int by)
{
	m_btPX = bx;
	m_btPY = by;
}
