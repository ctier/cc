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
#include "CCAgentBar.h"
#include "CCAgentBarPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCCAgentBarPropPage, COlePropertyPage)

// ��Ϣӳ��
BEGIN_MESSAGE_MAP(CCCAgentBarPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// ��ʼ���๤���� guid
IMPLEMENT_OLECREATE_EX(CCCAgentBarPropPage, "CCAGENTBAR.CCAgentBarPropPage.1",
	0x35261f88, 0xccd6, 0x4c86, 0x89, 0xc1, 0xe6, 0x74, 0x7e, 0xf0, 0xb3, 0x36)

// CCCAgentBarPropPage::CCCAgentBarPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CCCAgentBarPropPage ��ϵͳע�����
BOOL CCCAgentBarPropPage::CCCAgentBarPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CCAGENTBAR_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

// CCCAgentBarPropPage::CCCAgentBarPropPage - ���캯��
CCCAgentBarPropPage::CCCAgentBarPropPage() :
	COlePropertyPage(IDD, IDS_CCAGENTBAR_PPG_CAPTION)
{
}

// CCCAgentBarPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����
void CCCAgentBarPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CCCAgentBarPropPage ��Ϣ�������
