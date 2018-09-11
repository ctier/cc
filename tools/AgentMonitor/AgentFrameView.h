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


// CAgentFrameView ���

class CAgentFrameView : public CFrameWnd
{
	DECLARE_DYNCREATE(CAgentFrameView)
protected:
	CAgentFrameView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CAgentFrameView();
	CSplitterWnd  m_wndSplitterH;	//splitter
	CSplitterWnd*  m_pWndSplitterV;
	CCreateContext* m_pContext;

protected:
	afx_msg LRESULT OnGetSysInfo(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnAgentInfo(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};

