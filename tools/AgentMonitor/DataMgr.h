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
#include <afxtempl.h>
#include "acdcommon.h"
#include "MainFormView.h"

typedef struct //��ϯ��Ϣ
{
	char agentId[64];
	char agentDn[64];
	int currStatus;
	char loginTime[64];
	char loginDurx[64];
	char currStatusTime[64];
	char currStatusDurx[64];
	char origCaller[128];
	char origCallee[128];
	char statusStr[256];
	int	privQueueNum;
	char clientIpAddr[128];
} AgentInfo;

typedef struct //���ܶ�����Ϣ
{
	char callid[128];
	char startTime[64];
	char waitDurx[64];
	char origCaller[128];
	char origCallee[128];

} SkillQueueInfo;


typedef struct //˽�ж�����Ϣ
{
	char callid[128];
	char startTime[64];
	char waitDurx[64];
	char origCaller[128];
	char origCallee[128];

} PrivQueueInfo;

struct SysInfoT //ϵͳͳ������
{
    int totalAgents;
    int totalCalls;
    int ivrCalls;
    int agentCalls;
    int queueCalls;
    int privQueueCalls;
};

typedef CMap<CString,LPCTSTR,int,int> Agents; //��ϯMap����ϯ�ţ�ĳ��������
typedef CMap<CString,LPCTSTR,AgentInfo*,AgentInfo*> AgentMap; //��ϯ��ϢMap����ϯ�ţ���ϯ��Ϣ��
typedef CMap<CString,LPCTSTR,Agents*,Agents*>	SkillAgentsMap; //������ϯMap �����ܣ���ϯMap��,����ÿ�����ܵ���ϯ
typedef CMap<CString,LPCTSTR,CString,LPCTSTR>	SkillQueueMap; //���ܶ���Map �����ܣ�����String��������ÿ�����ܵĶ����ִ�
#define MAKELONGLONG(a, b) ((__int64)(((unsigned __int64)(a) & 0xffffffff) | ((unsigned __int64)(b) & 0xffffffff) << 32))
#define LOLONG(a) ((__int32)((unsigned __int64)(a) & 0xffffffff))
#define HILONG(a) ((__int32)((unsigned __int64)(a) >> 32))

#define WM_SKILLCHANGED WM_USER+9900 //�û�ѡ����ĳ������
#define WM_DATACHANGED WM_USER+9901 //��̨�̻߳�ȡ�����ݣ���Ҫ�������
#define WM_GETAGENTSDATA WM_USER+9902 // deprecated
#define WM_DISPLAYSYSINFO WM_USER+9903 //��Ҫ��ʾϵͳͳ����Ϣ
#define WM_DISPLAYAGENTINFO WM_USER+9904 //��Ҫ��ʾ������Ϣ
#define WM_SKILLGROUPCHANGED WM_USER+9905
#define WM_RESETAGENTDETAIL WM_USER+9906 //��Ҫ��ʾ������Ϣ

#define WM_SKILLNODE WM_USER+9907 //��Ҫ��ѯ�ļ��ܽڵ�
#define WM_SYSINFONODE WM_USER+9908 //��Ҫ��ʾsysinfo�ڵ�

class CDataMgr
{
public:
	CDataMgr(void);
	~CDataMgr(void);

	static CDataMgr* getInstance();
	void startGetAgentsDataThread();
	static UINT ThreadGetAgentsAndQueueData(LPVOID param);
	void createSkillAgentsMap(CString skill, Agents* pAgents);
	void createSkillQueueMap(CString skill, CString strQueue);
	void setQueueStringForSkill(CString skill, CString queueStr);
	Agents* getAgentsBySkill(CString skill);
	void setAgentInfo(CString agentId,AgentInfo* pAgentInfo);
	AgentInfo* getAgentInfo(CString agentId);
	AgentInfo* getAgentInfoOrCreate(CString agentId);
	CString CDataMgr::getQueueStringBySkill(CString skill);
	
	void beginGetSysInfo();
	void beginGetBySkill(CString skill);


	SkillAgentsMap m_skillAgentsMap;
	AgentMap m_agentMap;
	SkillQueueMap m_skillQueueMap;
	HWND m_hAgentDetailWnd;
	HWND m_hQueueDetailWnd;
	HWND m_hSysInfoWnd;
	HWND m_hAgentFrameView;
	HWND m_hSkillGroupTreeView;
	CMainFormView* m_pMainFormView;
	CWinThread* m_pThread;
	SysInfoT m_sysInfo;
	CRITICAL_SECTION m_csForAgents; //������ϯ������ݵ��ٽ���
	CRITICAL_SECTION m_csForQueue; //�������ܶ���������ݵ��ٽ���
	int m_privQueueColumn;



private:
	static CDataMgr* m_dataMgr;
};
