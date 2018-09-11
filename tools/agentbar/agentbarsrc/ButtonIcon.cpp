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

#include "ButtonIcon.h"

CButtonIcon::CButtonIcon() : m_iconIndex(0)
{
}

CButtonIcon::~CButtonIcon()
{
}

void CButtonIcon::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// ��ȡͼ�꣬ �˴����ͼ���index�����ڣ�����Щ�����ϳ��˷��ؿյ�HICON�⣬���ᱨ��
	HICON h = AfxGetApp()->LoadIcon(m_iconIndex);
	// IDI_ASTERISK-��Ϣ��IDI_EXCLAMATION-���棬IDI_QUESTION-�ʺţ�IDI_HAND-����IDI_APPLICATION-Ӧ�ó���ͼ��
	if(!h)
		h = AfxGetApp()->LoadStandardIcon(IDI_HAND);
	// ���DC��Դ
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	// ����DC
	int oldDC = pDC->SaveDC();
	// ����ͼ�갴ť
	pDC->DrawIcon(0, 0, h);
	// ��ԭDC
	pDC->RestoreDC(oldDC);
}

void CButtonIcon::PreSubclassWindow() 
{
	// �޸ķ��Ϊ�Ի�
	ModifyStyle(0, BS_OWNERDRAW);
    
	CButton::PreSubclassWindow();
}

void CButtonIcon::SetIcon(int index)
{
    m_iconIndex = index;
}
