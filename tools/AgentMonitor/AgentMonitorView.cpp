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
 

#include "stdafx.h"
#include "AgentMonitor.h"

#include "AgentMonitorDoc.h"
#include "AgentMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAgentMonitorView

IMPLEMENT_DYNCREATE(CAgentMonitorView, CView)

BEGIN_MESSAGE_MAP(CAgentMonitorView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAgentMonitorView ����/����

CAgentMonitorView::CAgentMonitorView()
{
	// TODO: �ڴ˴���ӹ������

}

CAgentMonitorView::~CAgentMonitorView()
{
}

BOOL CAgentMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// CAgentMonitorView ����

void CAgentMonitorView::OnDraw(CDC* /*pDC*/)
{
	CAgentMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CAgentMonitorView ��ӡ

BOOL CAgentMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CAgentMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡǰ��Ӷ���ĳ�ʼ��
}

void CAgentMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡ������������
}


// CAgentMonitorView ���

#ifdef _DEBUG
void CAgentMonitorView::AssertValid() const
{
	CView::AssertValid();
}

void CAgentMonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAgentMonitorDoc* CAgentMonitorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAgentMonitorDoc)));
	return (CAgentMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CAgentMonitorView ��Ϣ�������
