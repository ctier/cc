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

// CCAgentBarPropPage.h : CCCAgentBarPropPage ����ҳ���������
// CCCAgentBarPropPage : �й�ʵ�ֵ���Ϣ������� CCAgentBarPropPage.cpp��
class CCCAgentBarPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCCAgentBarPropPage)
	DECLARE_OLECREATE_EX(CCCAgentBarPropPage)

// ���캯��
public:
	CCCAgentBarPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_CCAGENTBAR };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};
