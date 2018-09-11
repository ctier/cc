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

#pragma once

#include "stdafx.h"

typedef unsigned int UNIT;

class CButtonEx : public CButton
{
private:
	CBrush m_BrushBk;
	// ��ť��״̬
	BOOL m_bOver;			// ���λ�ڰ�ť֮��ʱ��ֵΪtrue����֮Ϊflase
	BOOL m_bTracking;		// ����갴��û���ͷ�ʱ��ֵΪtrue
	BOOL m_bSelected;		// ��ť�������Ǹ�ֵΪtrue
	BOOL m_bIsDefault;
	BOOL m_nTypeStyle;
	int  m_btPX;			// ��ť�ĺ�����
	int  m_btPY;			// ��ť��������
	int  m_nIDBmpDefault;
	int  m_nIDBmpHover;

public:
	CButtonEx();
	virtual ~CButtonEx();

private:
	void TransparentBlt(HDC hdcDest,		// Ŀ��DC
					 int nXOriginDest,		// Ŀ��Xƫ��
					 int nYOriginDest,		// Ŀ��Yƫ��
					 int nWidthDest,		// Ŀ����
					 int nHeightDest,		// Ŀ��߶�
					 HDC hdcSrc,			// ԴDC
					 int nXOriginSrc,		// ԴX���
					 int nYOriginSrc,		// ԴY���
					 int nWidthSrc,			// Դ���
					 int nHeightSrc,		// Դ�߶�
					 UINT crTransparent );

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawButton(CDC *pDC, CBitmap &bitmap, CRect *rect);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnClicked();
	afx_msg UINT OnGetDlgCode();
	LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);

public:
	void SetBitMap(int nIDBmpDefault, int nIDBmpHover);
	void SetButtonPos(int bx, int by);

};
