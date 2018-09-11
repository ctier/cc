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

#define MY_BS_TYPEMASK SS_TYPEMASK

class CButtonXP : public CButton
{
private:
	// ��ť����߿�
	CPen m_BoundryPen;

	// ���ָ�����ڰ�ť֮��ʱ��ť���ڱ߿�
	CPen m_InsideBoundryPenLeft;
	CPen m_InsideBoundryPenRight;
	CPen m_InsideBoundryPenTop;
	CPen m_InsideBoundryPenBottom;

	// ��ť��ý���ʱ��ť���ڱ߿�
	CPen m_InsideBoundryPenLeftSel;
	CPen m_InsideBoundryPenRightSel;
	CPen m_InsideBoundryPenTopSel;
	CPen m_InsideBoundryPenBottomSel;

	// ��ť�Ļ�ˢ
	CBrush m_FillActive;
	CBrush m_FillInactive;

	// ��ť��״̬
	BOOL m_bOver;		// ���λ�ڰ�ť֮��ʱ��ֵΪtrue����֮Ϊflase
	BOOL m_bTracking;	// ����갴��û�G�ͷ�ʱ��ֵΪtrue
	BOOL m_bSelected;	// ��ť�������Ǹ�ֵΪtrue
	BOOL m_bFocus;		// ��ťΪ��ǰ��������ʱ��ֵΪtrue
	BOOL m_bIsDefault;
	BOOL m_nTypeStyle;

public:
	CButtonXP();
	virtual ~CButtonXP();

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DoGradientFill(CDC *pDC, CRect* rect);
	virtual void DrawInsideBorder(CDC *pDC, CRect* rect);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg BOOL OnClicked();
    afx_msg UINT OnGetDlgCode();
	LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);
};
