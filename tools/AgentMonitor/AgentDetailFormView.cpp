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
#include "AgentDetailFormView.h"
#include ".\agentdetailformview.h"
#include "DataMgr.h"
#include "PrivQueueDialog.h"

int CALLBACK CAgentDetailFormView::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CAgentDetailFormView* pView = reinterpret_cast<CAgentDetailFormView*>( lParamSort);
	ASSERT( pView->IsKindOf( RUNTIME_CLASS( CAgentDetailFormView ) ) );

	AgentInfo* pid1 = reinterpret_cast<AgentInfo*>( lParam1 );
	AgentInfo* pid2 = reinterpret_cast<AgentInfo*>( lParam2 );

	int res = 0;

	switch (pView->m_sortType)
	{
	case ::AGENT_SORT_TYPE_AGENT_ID:
		res = strcmp(pid1->agentId,pid2->agentId);
		break;
	case ::AGENT_SORT_TYPE_AGEND_DN:
		res = strcmp(pid1->agentDn,pid2->agentDn);
		break;
	case ::AGENT_SORT_TYPE_CURR_STATUS:
		if(pid1->currStatus == pid2->currStatus)
			res = 0;
		else if(pid1->currStatus > pid2->currStatus)
			res = 1;
		else if(pid1->currStatus < pid2->currStatus)
			res = -1;
		break;
	case ::AGENT_SORT_TYPE_LOGIN_TIME:
		res = strcmp(pid1->loginTime,pid2->loginTime);
		break;
	case ::AGENT_SORT_TYPE_LOGIN_DURX:
		res = strcmp(pid1->loginDurx,pid2->loginDurx);
		break;
	case ::AGENT_SORT_TYPE_CURR_STATUS_TIME:
		res = strcmp(pid1->currStatusTime,pid2->currStatusTime);
		break;
	case ::AGENT_SORT_TYPE_CURR_STATUS_DURX:
		res = strcmp(pid1->currStatusDurx,pid2->currStatusDurx);
		break;
	case ::AGENT_SORT_TYPE_ORIG_CALLER:
		res = strcmp(pid1->origCaller,pid2->origCaller);
		break;
	case ::AGENT_SORT_TYPE_ORIG_CALLEE:
		res = strcmp(pid1->origCallee,pid2->origCallee);
		break;
	case ::AGENT_SORT_TYPE_ORIG_CLIENT_IPADDR:
		res = strcmp(pid1->clientIpAddr,pid2->clientIpAddr);
		break;
	default:
		res = strcmp(pid1->agentId,pid2->agentId);
	}


	if(pView->m_sortOrder == ::AGENT_SORT_ORDER_DOWN)
	{
		if(res == 1) res = -1;
		else if(res == -1) res = 1;
	}

	return res;
}
// CAgentDetailFormView

IMPLEMENT_DYNCREATE(CAgentDetailFormView, CFormView)

CAgentDetailFormView::CAgentDetailFormView()
: CFormView(CAgentDetailFormView::IDD) 
{
	this->m_bInited=FALSE;
}

CAgentDetailFormView::~CAgentDetailFormView()
{
}

void CAgentDetailFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_agentList);
	DDX_Control(pDX, IDC_CHECK_IDLE, m_chkIdle);
	DDX_Control(pDX, IDC_CHECK_BUSY, m_chkBusy);
	DDX_Control(pDX, IDC_CHECK_REST, m_chkRest);
	DDX_Control(pDX, IDC_CHECK_TALKING, m_chkTalking);
	DDX_Control(pDX, IDC_CHECK_TALKING_PROGRESS, m_chkTalkingProgress);
	DDX_Control(pDX, IDC_CHECK_MONITOR, m_chkMonitor);
	DDX_Control(pDX, IDC_CHECK_OTHER, m_chkOther);

	DDX_Control(pDX, IDC_STATIC_ALL, m_txtAll);
	DDX_Control(pDX, IDC_STATIC_AGENTS_TITLE, m_staticAgentsStatus);
}

BEGIN_MESSAGE_MAP(CAgentDetailFormView, CFormView)
	ON_MESSAGE(WM_SKILLCHANGED, OnSkillChanged)
	ON_MESSAGE(WM_DATACHANGED, OnDataChanged)
	ON_MESSAGE(WM_RESETAGENTDETAIL, OnResetDetail)
	ON_BN_CLICKED(IDC_CHECK_TALKING, OnBnClickedCheckTalking)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickList2)
	ON_BN_CLICKED(IDC_CHECK_IDLE, OnBnClickedCheckIdle)
	ON_BN_CLICKED(IDC_CHECK_BUSY, OnBnClickedCheckBusy)
	ON_BN_CLICKED(IDC_CHECK_REST, OnBnClickedCheckRest)
	ON_BN_CLICKED(IDC_CHECK_TALKING_PROGRESS, OnBnClickedCheckTalkingProgress)
	ON_COMMAND(ID_32774, On32774)
	ON_UPDATE_COMMAND_UI(ID_32774, OnUpdate32774)
	ON_COMMAND(ID_MENU_AGENT_SET_BUSY, OnMenuAgentSetBusy)
	ON_UPDATE_COMMAND_UI(ID_MENU_AGENT_SET_BUSY, OnUpdateMenuAgentSetBusy)
	ON_COMMAND(ID_MENU_AGENT_SIGNOUT, OnMenuAgentSignout)
	ON_UPDATE_COMMAND_UI(ID_MENU_AGENT_SIGNOUT, OnUpdateMenuAgentSignout)
	ON_COMMAND(ID_MENU_AGENT_MONITOR, OnMenuAgentMonitor)
	ON_UPDATE_COMMAND_UI(ID_MENU_AGENT_MONITOR, OnUpdateMenuAgentMonitor)
	ON_COMMAND(ID_MENU_AGENT_STOP_MONITOR, OnMenuAgentStopMonitor)
	ON_UPDATE_COMMAND_UI(ID_MENU_AGENT_STOP_MONITOR, OnUpdateMenuAgentStopMonitor)
	ON_COMMAND(ID_MENU_AGENT_INSERT, OnMenuAgentInsert)
	ON_UPDATE_COMMAND_UI(ID_MENU_AGENT_INSERT, OnUpdateMenuAgentInsert)
	ON_COMMAND(ID_MENU_AGENT_STOP_INSERT, OnMenuAgentStopInsert)
	ON_UPDATE_COMMAND_UI(ID_MENU_AGENT_STOP_INSERT, OnUpdateMenuAgentStopInsert)
	ON_COMMAND(ID_MENU_AGENT_INSERT_MONITOR_CONVERT, OnMenuAgentInsertMonitorConvert)
	ON_UPDATE_COMMAND_UI(ID_MENU_AGENT_INSERT_MONITOR_CONVERT, OnUpdateMenuAgentInsertMonitorConvert)
	ON_COMMAND(ID_MENU_AGENT_BREAK, OnMenuAgentBreak)
	ON_UPDATE_COMMAND_UI(ID_MENU_AGENT_BREAK, OnUpdateMenuAgentBreak)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, OnNMRclickList2)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECK_MONITOR, OnBnClickedCheckMonitor)
	ON_BN_CLICKED(IDC_CHECK_OTHER, OnBnClickedCheckOther)
 
	ON_BN_CLICKED(IDC_BUTTON_FORCE_SIGNOUT_ALL, OnBnClickedButtonForceSignoutAll)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, OnNMClickList2)
END_MESSAGE_MAP()


// CAgentDetailFormView ���

#ifdef _DEBUG
void CAgentDetailFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CAgentDetailFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CAgentDetailFormView ��Ϣ�������

void CAgentDetailFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize(0, 0));
	CDataMgr::getInstance()->m_hAgentDetailWnd=this->GetSafeHwnd();

	CString oriStr = CDataMgr::getInstance()->m_pMainFormView->m_strEnabledColumn;
 
	CFont ft;
	ft.CreateFont(-24, 0, 0, 0, 400, FALSE, FALSE,
	                0,GB2312_CHARSET , OUT_DEFAULT_PRECIS,//ANSI_CHARSET
	                CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                 VARIABLE_PITCH | FF_SCRIPT, "����_GB2312");//DEFAULT_PITCH FF_MODERN
	this->m_staticAgentsStatus.SetFont(&ft,TRUE);

	this->m_strVec.empty();
	while (true)
	{
		CString n = oriStr.SpanExcluding(",");
		m_strVec.push_back(n);
		oriStr = oriStr.Right(oriStr.GetLength()-n.GetLength()-1);
		if (oriStr.IsEmpty())
			break;
	}


	if(!m_bInited)
	{

		CRect rect;
		this->GetWindowRect(&rect);
		int width=rect.Width();

		m_agentList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		m_agentList.ModifyStyle(0,LVS_REPORT);

		int sub = 0;
		CDataMgr::getInstance()->m_privQueueColumn = -1;
		for (std::vector<CString>::const_iterator it = m_strVec.begin();it != m_strVec.end();it++)
		{
 
			CString sz = (*it);
			LV_COLUMN lvcol;	////////�����ĸ���
			lvcol.mask = LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcol.iSubItem = sub;
			lvcol.pszText = (char*)(LPCTSTR)sz;
			if(!sz.Compare("����"))
				lvcol.cx = 50;
			else if(!sz.Compare("����"))
				lvcol.cx = 100;

			else if(!sz.Compare("��¼ʱ��"))
			{
				lvcol.cx = 140;
				lvcol.pszText = "��¼ʱ���";
			}
			else if(!sz.Compare("��¼ʱ��"))
				lvcol.cx = 90;
			else if(!sz.Compare("��ǰ״̬"))
				lvcol.cx = 80;
			else if(!sz.Compare("��ǰ״̬ʱ��"))
				lvcol.cx = 140;
			else if(!sz.Compare("˽�ж���"))
			{
				lvcol.cx = 100;
				CDataMgr::getInstance()->m_privQueueColumn = sub;
			}
			else if(!sz.Compare("��ǰ״̬ʱ��"))
				lvcol.cx = 100;
			else if(!sz.Compare("����"))
				lvcol.cx = 100;
			else if(!sz.Compare("����"))
				lvcol.cx = 100;	
			else if(!sz.Compare("�ͻ���IP��ַ"))
				lvcol.cx = 100;

			this->m_agentList.InsertColumn(sub, &lvcol);
			sub ++;
		}


		this->m_sortType=::AGENT_SORT_TYPE_LOGIN_TIME;
		this->m_sortOrder=AGENT_SORT_ORDER_UP;
		m_agentList.MoveWindow(0,40,rect.Width(),rect.Height()-40,1);
		this->m_bInited=TRUE;
	}
	// TODO: �ڴ����ר�ô����/����û���
}

void CAgentDetailFormView::OnDraw(CDC* /*pDC*/)
{
	// TODO: �ڴ����ר�ô����/����û���
	CRect rect;
	this->GetWindowRect(&rect);
	int width=rect.Width();

	m_agentList.MoveWindow(0,40,rect.Width(),rect.Height()-40,1);
}

LRESULT CAgentDetailFormView::OnSkillChanged(WPARAM wParam, LPARAM lParam)
{
	LONGLONG address = MAKELONGLONG(wParam, lParam);
	char* skill = reinterpret_cast<char *>(address);

	this->m_currSkill=skill;
	this->displayAgentsForSkill(m_currSkill);

	delete skill;
	return 0;
}

LRESULT CAgentDetailFormView::OnDataChanged(WPARAM wParam, LPARAM lParam)
{
	this->displayAgentsForSkill(this->m_currSkill);
	return 0;
}

void CAgentDetailFormView::displayAgentsForSkill(CString skill)
{
	//	this->m_agentList.DeleteAllItems();
	if (!CDataMgr::getInstance()->m_pMainFormView->m_bStartGettingData)
	{

		this->m_agentList.DeleteAllItems();
		return;
	}
	::EnterCriticalSection(&CDataMgr::getInstance()->m_csForAgents);
	Agents* al = CDataMgr::getInstance()->getAgentsBySkill(skill);

	if(al == NULL)
	{
		::LeaveCriticalSection(&CDataMgr::getInstance()->m_csForAgents);
		return  ;
	}

	CMap<CString,LPCTSTR,int,int> dataMap;
	POSITION pos = al->GetStartPosition();

	int nIdle = 0,nBusy = 0,nRest = 0,nTalking = 0,nTalkingProgress = 0,nMonitor=0,nOther=0,nAll=0;

	CString strKey;
	int value;
 
	while(pos)
	{
		al->GetNextAssoc(pos, strKey, value);

		CString agentId = strKey;

		AgentInfo* ai = CDataMgr::getInstance()->getAgentInfo(agentId);

		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT ;

		bool shouldAdd = false;

		if(!m_chkIdle.GetCheck()
			&& !m_chkBusy.GetCheck()
			&& !m_chkRest.GetCheck()
			&& !m_chkTalking.GetCheck()
			&& !m_chkTalkingProgress.GetCheck()
			&& !m_chkMonitor.GetCheck()
			&& !m_chkOther.GetCheck())
			shouldAdd = true;	// ��ûѡ���൱�ڶ�ѡ��

		char stateStr[128];
		switch (ai->currStatus)
		{
		case acd::AgentStatusT::AsReadyState:
			strcpy(stateStr,"��������");
			if(m_chkIdle.GetCheck()) shouldAdd = true;
			nIdle++;
			break;
		case acd::AgentStatusT::AsBusyState:
			strcpy(stateStr,"��������");
			if(m_chkBusy.GetCheck()) shouldAdd = true;
			nBusy++;
			break;
		case acd::AgentStatusT::AsRestState:
			strcpy(stateStr,"��Ϣ");
			if(m_chkRest.GetCheck()) shouldAdd = true;
			nRest++;
			break;
		case acd::AgentStatusT::AsConnectedState:
			strcpy(stateStr,"ͨ��");
			if(m_chkTalking.GetCheck()) shouldAdd = true;
			nTalking++;
			break;
		case acd::AgentStatusT::AsInternalConnectedState:
			strcpy(stateStr,"�ڲ�ͨ��");
			if(m_chkTalking.GetCheck()) shouldAdd = true;
			nTalking++;
			break;
		case acd::AgentStatusT::AsSuspendedState:
			strcpy(stateStr,"����");
			if(m_chkTalking.GetCheck()) shouldAdd = true;
			nTalking++;
			break;
		case acd::AgentStatusT::AsConsultConnectingState:
			strcpy(stateStr,"��ѯ");
			if(m_chkTalking.GetCheck()) shouldAdd = true;
			nTalking++;
			break;
		case acd::AgentStatusT::AsConsultConnectedState:
			strcpy(stateStr,"��ѯͨ��");
			if(m_chkTalking.GetCheck()) shouldAdd = true;
			nTalking++;
			break;
		case acd::AgentStatusT::AsConferenceState:
			strcpy(stateStr,"����");
			if(m_chkTalking.GetCheck()) shouldAdd = true;
			nTalking++;
			break;
		case acd::AgentStatusT::AsMonitoredState:
			strcpy(stateStr,"������");
			if(m_chkTalking.GetCheck()) shouldAdd = true;
			nTalking++;
			break;
		case acd::AgentStatusT::AsInsertedState:
			strcpy(stateStr,"��ǿ��");
			if(m_chkTalking.GetCheck()) shouldAdd = true;
			nTalking++;
			break;
		case acd::AgentStatusT::AsAlertingState:
			strcpy(stateStr,"����");
			if(m_chkTalkingProgress.GetCheck()) shouldAdd = true;
			nTalkingProgress++;
			break;
		case acd::AgentStatusT::AsHalfAlertingState:
			strcpy(stateStr,"������");
			if(m_chkTalkingProgress.GetCheck()) shouldAdd = true;
			nTalkingProgress++;
			break;
		case acd::AgentStatusT::AsConnectingState:
			strcpy(stateStr,"������");
			if(m_chkTalkingProgress.GetCheck()) shouldAdd = true;
			nTalkingProgress++;
			break;
		case acd::AgentStatusT::AsHalfConnectedState:
			strcpy(stateStr,"��ͨ��");
			if(m_chkTalkingProgress.GetCheck()) shouldAdd = true;
			nTalkingProgress++;
			break;
		case acd::AgentStatusT::AsMonitorState:
			strcpy(stateStr,"����");
			if(m_chkMonitor.GetCheck()) shouldAdd = true;
			nMonitor++;
			break;
		case acd::AgentStatusT::AsInsertState:
			strcpy(stateStr,"ǿ��");
			if(m_chkMonitor.GetCheck()) shouldAdd = true;
			nMonitor++;
			break;
		case acd::AgentStatusT::AsUnknownState:
			strcpy(stateStr,"δ֪״̬");
			if(m_chkOther.GetCheck()) shouldAdd = true;
			nOther++;
			break;
		case acd::AgentStatusT::AsUnLoginState:
			strcpy(stateStr,"δǩ��");
			if(m_chkOther.GetCheck()) shouldAdd = true;
			nOther++;
			break;
		case acd::AgentStatusT::AsLockState:
			strcpy(stateStr,"Ԥռ");
			if(m_chkOther.GetCheck()) shouldAdd = true;
			nOther++;
			break;
		}

		nAll++;
		strcpy(ai->statusStr,stateStr);
		if(shouldAdd)
		{
			dataMap.SetAt(agentId,1);
		}

	}

	CString stats;
	stats.Format("%s-%d","��������",nIdle);
	this->m_chkIdle.SetWindowText(stats);

	stats.Format("%s-%d","��������",nBusy);
	this->m_chkBusy.SetWindowText(stats);

	stats.Format("%s-%d","��Ϣ",nRest);
	this->m_chkRest.SetWindowText(stats);

	stats.Format("%s-%d","ͨ����",nTalking);
	this->m_chkTalking.SetWindowText(stats);

	stats.Format("%s-%d","����ͨ��",nTalkingProgress);
	this->m_chkTalkingProgress.SetWindowText(stats);

	stats.Format("%s-%d","�ʼ�",nMonitor);
	this->m_chkMonitor.SetWindowText(stats);
	
	stats.Format("%s-%d","����",nOther);
	this->m_chkOther.SetWindowText(stats);

	stats.Format("%s-%d","ȫ��",nAll);
	this->m_txtAll.SetWindowText(stats);
	
	// ���´�����ͼ�ڸ���List����ʱ������ԭ�е�item���ػ��� �Ա�����潩��������Ҳ�ܾ�������ѡ�е�item������Ϊ���±�ɷ�ѡ����
	// �õ��������еģ�����������û�еģ�ɾ��֮
	CMap<CString,LPCTSTR,int,int> uiMap;
	CList<int> deletel;
	int listC = m_agentList.GetItemCount();
	for(int j = 0; j < listC; j++)
	{
		AgentInfo* aginfo=(AgentInfo*)(m_agentList.GetItemData(j));
		CString agid = aginfo->agentId;
		int tmpI;
		if(!dataMap.Lookup(agid,tmpI))
		{
			deletel.AddHead(j);
		}

	}

	// �Ӻ���ǰɾ��Ҫ�������
	POSITION posList = deletel.GetHeadPosition();
	while(posList != NULL)   
	{   
		int item = deletel.GetNext(posList);
		m_agentList.DeleteItem(item);
	}

	// �õ��������Agent Map
	listC = m_agentList.GetItemCount();
	for( j = 0; j < listC; j++)
	{
		AgentInfo* aginfo = (AgentInfo*)(m_agentList.GetItemData(j));
		CString agid = aginfo->agentId;
		uiMap.SetAt(agid,j);

	}

	pos = dataMap.GetStartPosition();

	while(pos)
	{
		dataMap.GetNextAssoc(pos, strKey, value);
		CString agentId = strKey;
		AgentInfo* ai = CDataMgr::getInstance()->getAgentInfo(agentId);

		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT ;

		int tmpI;
		if(!uiMap.Lookup(agentId,tmpI))	// List has no this agentid, should create and add ������û�еģ������������еģ������
		{
			int cItem = m_agentList.GetItemCount();
			lvi.iItem = cItem;	 
			lvi.mask = LVIF_TEXT | LVIF_PARAM;
			lvi.iSubItem = 0; 
			lvi.lParam = (LPARAM)ai;

			int sub = 0;
			for (std::vector<CString>::const_iterator it = m_strVec.begin();it != m_strVec.end();it++)
			{
 
				CString sz = (*it);
				
				CString newStr = "";
				if(!sz.Compare("����"))
					newStr = ai->agentId;
				else if(!sz.Compare("����"))
					newStr = ai->agentDn;
				else if(!sz.Compare("��¼ʱ��"))
					newStr = ai->loginTime;
				else if(!sz.Compare("��¼ʱ��"))
					newStr = ai->loginDurx;
				else if(!sz.Compare("��ǰ״̬"))
					newStr = ai->statusStr;
				else if(!sz.Compare("��ǰ״̬ʱ��"))
					newStr = ai->currStatusTime;
				else if(!sz.Compare("˽�ж���"))
				{
					if(ai->privQueueNum>0)
					newStr.Format("%d (����鿴)",ai->privQueueNum);
					else
					newStr.Format("%d",ai->privQueueNum);
				}
				else if(!sz.Compare("��ǰ״̬ʱ��"))
					newStr = ai->currStatusDurx;
				else if(!sz.Compare("����"))
					newStr = ai->origCaller;
				else if(!sz.Compare("����"))
					newStr = ai->origCallee;
				else if(!sz.Compare("�ͻ���IP��ַ"))
					newStr = ai->clientIpAddr;

				if(sub==0)
				{
					lvi.pszText = (char*)(LPCTSTR) newStr;	 
					int iItem = this->m_agentList.InsertItem(&lvi);	 
				}
				else
				{
					this->m_agentList.SetItemText(cItem,sub,(char*)(LPCTSTR) newStr);
				}
		
				sub ++;
			}

		}
		else //just update �������еģ�������Ҳ�еģ������
		{
			int sub = 0;
			for (std::vector<CString>::const_iterator it = m_strVec.begin();it != m_strVec.end();it++)
			{
 
				CString sz = (*it);
				
				CString newStr = "";
				if(!sz.Compare("����"))
					newStr = ai->agentId;
				else if(!sz.Compare("����"))
					newStr = ai->agentDn;
				else if(!sz.Compare("��¼ʱ��"))
					newStr = ai->loginTime;
				else if(!sz.Compare("��¼ʱ��"))
					newStr = ai->loginDurx;
				else if(!sz.Compare("��ǰ״̬"))
					newStr = ai->statusStr;
				else if(!sz.Compare("��ǰ״̬ʱ��"))
					newStr = ai->currStatusTime;
				else if(!sz.Compare("˽�ж���"))
				{
					if(ai->privQueueNum>0)
					newStr.Format("%d (����鿴)",ai->privQueueNum);
					else
					newStr.Format("%d",ai->privQueueNum);
				}
				else if(!sz.Compare("��ǰ״̬ʱ��"))
					newStr = ai->currStatusDurx;
				else if(!sz.Compare("����"))
					newStr = ai->origCaller;
				else if(!sz.Compare("����"))
					newStr = ai->origCallee;
				else if(!sz.Compare("�ͻ���IP��ַ"))
					newStr = ai->clientIpAddr;

				 
				{
					this->m_agentList.SetItemText(tmpI,sub,(char*)(LPCTSTR) newStr);
				}
		
				sub ++;
			}
		}

	}

	if(this->m_sortType != AGENT_SORT_TYPE_NONE)
		this->m_agentList.SortItems(CAgentDetailFormView::CompareFunc,(DWORD_PTR)this);

	::LeaveCriticalSection(&CDataMgr::getInstance()->m_csForAgents);
}


void CAgentDetailFormView::OnHdnItemclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	NM_LISTVIEW*   pNMListView   =   (NM_LISTVIEW*)pNMHDR;

	CHeaderCtrl *pHeaderCtrl = m_agentList.GetHeaderCtrl(); 

	HDITEM hdi; 

	TCHAR lpBuffer[MAX_PATH]; 
	hdi.mask = HDI_TEXT; 
	hdi.pszText = lpBuffer; 
	hdi.cchTextMax = 256; 
	pHeaderCtrl-> GetItem(pNMListView-> iItem,&hdi); 

	CString  str = CString(lpBuffer);

	CString strColumn = str;
	if(str.Right(2) == CString("��"))
	{
		CString newStr;
		newStr.Format("%s%s",str.Left(str.GetLength()-2),"��");
		strColumn = str.Left(str.GetLength()-2);

		hdi.pszText = newStr.GetBuffer();; 
		pHeaderCtrl->SetItem(pNMListView->iItem,&hdi); 
		this->m_sortOrder = ::AGENT_SORT_ORDER_UP;
		
	}
	else if(str.Right(2) == CString("��"))
	{
		CString newStr;
		newStr.Format("%s%s",str.Left(str.GetLength()-2),"��");
		strColumn = str.Left(str.GetLength()-2);

		hdi.pszText = newStr.GetBuffer();; 
		pHeaderCtrl->SetItem(pNMListView->iItem,&hdi); 
		this->m_sortOrder = ::AGENT_SORT_ORDER_DOWN;
	}
	else
	{
		CString newStr;
		newStr.Format("%s%s",str,"��");

		hdi.pszText = newStr.GetBuffer();; 
		pHeaderCtrl->SetItem(pNMListView->iItem,&hdi); 

		this->m_sortOrder = ::AGENT_SORT_ORDER_UP;

	}

	int count=pHeaderCtrl->GetItemCount();

	for (int i = 0; i<count; i++)
	{
		if(i==pNMListView->iItem) continue;

		HDITEM hdi; 

		TCHAR lpBuffer[MAX_PATH]; 
		hdi.mask = HDI_TEXT; 
		hdi.pszText = lpBuffer; 
		hdi.cchTextMax = 256; 
		pHeaderCtrl-> GetItem(i,&hdi); 

		CString  str = CString(lpBuffer);

		if(str.Right(2) == CString("��"))
		{
			CString newStr;
			newStr.Format("%s%s",str.Left(str.GetLength()-2),"");

			hdi.pszText = newStr.GetBuffer();; 
			pHeaderCtrl->SetItem(i,&hdi); 
		}
		else if(str.Right(2) == CString("��"))
		{
			CString newStr;
			newStr.Format("%s%s",str.Left(str.GetLength()-2),"");

			hdi.pszText = newStr.GetBuffer();; 
			pHeaderCtrl->SetItem(i,&hdi); 
		}
	}

 
	if(!strColumn.Compare("����"))
		this->m_sortType = ::AGENT_SORT_TYPE_AGENT_ID;
	else if(!strColumn.Compare("����"))
		this->m_sortType = ::AGENT_SORT_TYPE_AGEND_DN;
	else if(!strColumn.Compare("��¼ʱ��"))
		this->m_sortType = ::AGENT_SORT_TYPE_LOGIN_TIME;
	else if(!strColumn.Compare("��¼ʱ��"))
		this->m_sortType = ::AGENT_SORT_TYPE_LOGIN_DURX;
	else if(!strColumn.Compare("��ǰ״̬"))
		this->m_sortType = ::AGENT_SORT_TYPE_CURR_STATUS;
	else if(!strColumn.Compare("��ǰ״̬ʱ��"))
		this->m_sortType = ::AGENT_SORT_TYPE_CURR_STATUS_TIME;
	else if(!strColumn.Compare("˽�ж���"))
		this->m_sortType = ::AGENT_SORT_TYPE_ORIG_PRIVATE_QUEUE_NUM;
	else if(!strColumn.Compare("��ǰ״̬ʱ��"))
		this->m_sortType = ::AGENT_SORT_TYPE_CURR_STATUS_DURX;
	else if(!strColumn.Compare("����"))
		this->m_sortType = ::AGENT_SORT_TYPE_ORIG_CALLER;
	else if(!strColumn.Compare("����"))
		this->m_sortType = ::AGENT_SORT_TYPE_ORIG_CALLEE;
	else if(!strColumn.Compare("�ͻ���IP��ַ"))
		this->m_sortType = ::AGENT_SORT_TYPE_ORIG_CLIENT_IPADDR;

	//this->m_sortType = (AGENT_SORT_TYPE)(pNMListView->iItem);

	if(this->m_sortType != AGENT_SORT_TYPE_NONE)
	{
		::EnterCriticalSection(&CDataMgr::getInstance()->m_csForAgents);
		this->m_agentList.SortItems(CAgentDetailFormView::CompareFunc,(DWORD_PTR)this);
		::LeaveCriticalSection(&CDataMgr::getInstance()->m_csForAgents);
	}

	*pResult = 0;
}

void CAgentDetailFormView::OnBnClickedCheckIdle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->displayAgentsForSkill(this->m_currSkill);
}

void CAgentDetailFormView::OnBnClickedCheckBusy()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->displayAgentsForSkill(this->m_currSkill);
}

void CAgentDetailFormView::OnBnClickedCheckRest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->displayAgentsForSkill(this->m_currSkill);
}

void CAgentDetailFormView::OnBnClickedCheckTalking()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->displayAgentsForSkill(this->m_currSkill);
}

void CAgentDetailFormView::OnBnClickedCheckMonitor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->displayAgentsForSkill(this->m_currSkill);
}

void CAgentDetailFormView::OnBnClickedCheckOther()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->displayAgentsForSkill(this->m_currSkill);
}

void CAgentDetailFormView::OnBnClickedCheckTalkingProgress()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->displayAgentsForSkill(this->m_currSkill);
}

void CAgentDetailFormView::On32774()
{
	// TODO: �ڴ���������������
	
	POSITION pos = m_agentList.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
			int nIdx = m_agentList.GetNextSelectedItem(pos);
			AgentInfo* ai = (AgentInfo*)(m_agentList.GetItemData(nIdx));
			CDataMgr::getInstance()->m_pMainFormView->m_agentCtrl.ForceIdle(ai->agentId);
	}
 
}

void CAgentDetailFormView::OnUpdate32774(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������

	POSITION pos = m_agentList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{

	 
	}
 
}

void CAgentDetailFormView::OnMenuAgentSetBusy()
{
	// TODO: �ڴ���������������
 
	POSITION pos = m_agentList.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
 			int nIdx = m_agentList.GetNextSelectedItem(pos);
			AgentInfo* ai = (AgentInfo*)(m_agentList.GetItemData(nIdx));
			CDataMgr::getInstance()->m_pMainFormView->m_agentCtrl.ForceBusy(ai->agentId);
	}
}

void CAgentDetailFormView::OnUpdateMenuAgentSetBusy(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CAgentDetailFormView::OnMenuAgentSignout()
{
	// TODO: �ڴ���������������
	POSITION pos = m_agentList.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
 			int nIdx = m_agentList.GetNextSelectedItem(pos);
			AgentInfo* ai = (AgentInfo*)(m_agentList.GetItemData(nIdx));
			CDataMgr::getInstance()->m_pMainFormView->m_agentCtrl.ForceOut(ai->agentId);
	}
}

void CAgentDetailFormView::OnUpdateMenuAgentSignout(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CAgentDetailFormView::OnMenuAgentMonitor()
{
	// TODO: �ڴ���������������
	POSITION pos = m_agentList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
 			int nIdx = m_agentList.GetNextSelectedItem(pos);
			AgentInfo* ai = (AgentInfo*)(m_agentList.GetItemData(nIdx));
			CDataMgr::getInstance()->m_pMainFormView->m_agentCtrl.Listen(ai->agentId,"","");
	}
}

void CAgentDetailFormView::OnUpdateMenuAgentMonitor(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������

}

void CAgentDetailFormView::OnMenuAgentStopMonitor()
{
	// TODO: �ڴ���������������
	POSITION pos = m_agentList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
 			int nIdx = m_agentList.GetNextSelectedItem(pos);
			AgentInfo* ai = (AgentInfo*)(m_agentList.GetItemData(nIdx));
			CDataMgr::getInstance()->m_pMainFormView->m_agentCtrl.StopListen(ai->agentId);
	}
}

void CAgentDetailFormView::OnUpdateMenuAgentStopMonitor(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CAgentDetailFormView::OnMenuAgentInsert()
{
	// TODO: �ڴ���������������
	POSITION pos = m_agentList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
 			int nIdx = m_agentList.GetNextSelectedItem(pos);
			AgentInfo* ai = (AgentInfo*)(m_agentList.GetItemData(nIdx));
			CDataMgr::getInstance()->m_pMainFormView->m_agentCtrl.Insert(ai->agentId,"","");
	}
}

void CAgentDetailFormView::OnUpdateMenuAgentInsert(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CAgentDetailFormView::OnMenuAgentStopInsert()
{
	// TODO: �ڴ���������������
	POSITION pos = m_agentList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
 			int nIdx = m_agentList.GetNextSelectedItem(pos);
			AgentInfo* ai = (AgentInfo*)(m_agentList.GetItemData(nIdx));
			CDataMgr::getInstance()->m_pMainFormView->m_agentCtrl.StopInsert(ai->agentId);

	}
}

void CAgentDetailFormView::OnUpdateMenuAgentStopInsert(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CAgentDetailFormView::OnMenuAgentInsertMonitorConvert()
{
	// TODO: �ڴ���������������
	POSITION pos = m_agentList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
 			int nIdx = m_agentList.GetNextSelectedItem(pos);
			AgentInfo* ai = (AgentInfo*)(m_agentList.GetItemData(nIdx));
			CDataMgr::getInstance()->m_pMainFormView->m_agentCtrl.SwitchInsertorListen(ai->agentId,"","");
	}
 
}

void CAgentDetailFormView::OnUpdateMenuAgentInsertMonitorConvert(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CAgentDetailFormView::OnMenuAgentBreak()
{
	// TODO: �ڴ���������������
	POSITION pos = m_agentList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
 			int nIdx = m_agentList.GetNextSelectedItem(pos);
			AgentInfo* ai = (AgentInfo*)(m_agentList.GetItemData(nIdx));
			CDataMgr::getInstance()->m_pMainFormView->m_agentCtrl.Break(ai->agentId);
	}
 
}

void CAgentDetailFormView::OnUpdateMenuAgentBreak(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CAgentDetailFormView::OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!CDataMgr::getInstance()->m_pMainFormView->m_bStartGettingData)
	{
		return;
	}

	CRect rect;
	::GetWindowRect(pNMHDR->hwndFrom,&rect);

	CPoint point;
	::GetCursorPos(&point);

	CMenu menu;
	if (!menu.LoadMenu(IDR_MENU_AGENT_OPERATION)) return;

	CMenu* pSubMenu = menu.GetSubMenu(0);

	pSubMenu->EnableMenuItem(ID_32774,MF_GRAYED);
	pSubMenu->EnableMenuItem(ID_MENU_AGENT_SET_BUSY,MF_GRAYED);
	pSubMenu->EnableMenuItem(ID_MENU_AGENT_SIGNOUT,MF_GRAYED);
	pSubMenu->EnableMenuItem(ID_MENU_AGENT_MONITOR,MF_GRAYED);
	pSubMenu->EnableMenuItem(ID_MENU_AGENT_STOP_MONITOR,MF_GRAYED);
	pSubMenu->EnableMenuItem(ID_MENU_AGENT_INSERT,MF_GRAYED);
	pSubMenu->EnableMenuItem(ID_MENU_AGENT_STOP_INSERT,MF_GRAYED);
	pSubMenu->EnableMenuItem(ID_MENU_AGENT_INSERT_MONITOR_CONVERT,MF_GRAYED);
	pSubMenu->EnableMenuItem(ID_MENU_AGENT_BREAK,MF_GRAYED);

	POSITION pos = m_agentList.GetFirstSelectedItemPosition();


	if (pos == NULL)
	{
 
	}
	else
	{

			POSITION p = pos;
			int sc = 0;
			while(p)
			{
				m_agentList.GetNextSelectedItem(p);
				sc ++;
			}
			if(sc>1)
			{
				pSubMenu->EnableMenuItem(ID_32774,MF_ENABLED );
				pSubMenu->EnableMenuItem(ID_MENU_AGENT_SET_BUSY,MF_ENABLED );
				pSubMenu->EnableMenuItem(ID_MENU_AGENT_SIGNOUT,MF_ENABLED );
			}
			else
			{

				pSubMenu->EnableMenuItem(ID_32774,MF_ENABLED );
				pSubMenu->EnableMenuItem(ID_MENU_AGENT_SET_BUSY,MF_ENABLED );
				pSubMenu->EnableMenuItem(ID_MENU_AGENT_SIGNOUT,MF_ENABLED );


				int nIdx = m_agentList.GetNextSelectedItem(pos);
				AgentInfo* ai = (AgentInfo*)(m_agentList.GetItemData(nIdx));
				switch (ai->currStatus)
				{
					case acd::AgentStatusT::AsConnectedState:
						pSubMenu->EnableMenuItem(ID_MENU_AGENT_MONITOR,MF_ENABLED);
 
						pSubMenu->EnableMenuItem(ID_MENU_AGENT_INSERT,MF_ENABLED);
 
						pSubMenu->EnableMenuItem(ID_MENU_AGENT_BREAK,MF_ENABLED);
						break;
					case acd::AgentStatusT::AsMonitoredState:
						pSubMenu->EnableMenuItem(ID_MENU_AGENT_STOP_MONITOR,MF_ENABLED);
	 
						pSubMenu->EnableMenuItem(ID_MENU_AGENT_BREAK,MF_ENABLED);
						break;
					case acd::AgentStatusT::AsInsertedState:
						pSubMenu->EnableMenuItem(ID_MENU_AGENT_STOP_INSERT,MF_ENABLED);
	 
						pSubMenu->EnableMenuItem(ID_MENU_AGENT_BREAK,MF_ENABLED);
						break;

				}
			}
	}
 	
	::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);
	::SetForegroundWindow(this->GetSafeHwnd());	

	::TrackPopupMenu(pSubMenu->m_hMenu, 0, point.x,point.y, 0,
		GetSafeHwnd(), NULL);
	*pResult = 0;
}

void CAgentDetailFormView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CFormView::OnRButtonDown(nFlags, point);
}


void CAgentDetailFormView::OnBnClickedButtonForceSignoutAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*
	POSITION pos = m_agentList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
 			int nIdx = m_agentList.GetNextSelectedItem(pos);
			AgentInfo* ai = (AgentInfo*)(m_agentList.GetItemData(nIdx));
			CDataMgr::getInstance()->m_pMainFormView->m_agentCtrl.ForceOut(ai->agentId);
	}
	*/
}

void CAgentDetailFormView::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	if(CDataMgr::getInstance()->m_privQueueColumn < 0)
		return;
	POINT point;
	UINT uFlags;
	GetCursorPos(&point);
	m_agentList.ScreenToClient(&point);
	int nItem = m_agentList.HitTest(point, &uFlags);
	if(nItem >= 0)
	{
		CRect rc;
		m_agentList.GetSubItemRect(nItem,CDataMgr::getInstance()->m_privQueueColumn ,LVIR_BOUNDS,rc);
		if(PtInRect(&rc,point))
		{
			
			CPrivQueueDialog dlg;
			AgentInfo* ai = (AgentInfo*) (m_agentList.GetItemData(nItem));
			dlg.m_currAgentID = ai->agentId;
			dlg.DoModal();

		}
	}

}

LRESULT CAgentDetailFormView::OnResetDetail(WPARAM wParam, LPARAM lParam)
{
	this->m_agentList.DeleteAllItems();
	int column = m_agentList.GetHeaderCtrl()->GetItemCount();

	while(m_agentList.GetHeaderCtrl()->GetItemCount() != 0)
	{
		this->m_agentList.DeleteColumn(0);
	}

	CRect rect;
	this->GetWindowRect(&rect);
	int width=rect.Width();

	m_agentList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_agentList.ModifyStyle(0,LVS_REPORT);

	CString oriStr = CDataMgr::getInstance()->m_pMainFormView->m_strEnabledColumn;


	this->m_strVec.clear();

	while (true)
	{
		CString n = oriStr.SpanExcluding(",");
		m_strVec.push_back(n);
		oriStr = oriStr.Right(oriStr.GetLength()-n.GetLength()-1);
		if (oriStr.IsEmpty())
			break;
	}
	int sub = 0;
	CDataMgr::getInstance()->m_privQueueColumn = -1;

	for (std::vector<CString>::const_iterator it = m_strVec.begin();it != m_strVec.end();it++)
	{

		CString sz = (*it);
		LV_COLUMN lvcol;	////////�����ĸ���
		lvcol.mask = LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcol.iSubItem = sub;
		lvcol.pszText = (char*)(LPCTSTR)sz;

		if(!sz.Compare("����"))
			lvcol.cx = 50;
		else if(!sz.Compare("����"))
			lvcol.cx = 100;

		else if(!sz.Compare("��¼ʱ��"))
		{
			lvcol.cx = 140;
			lvcol.pszText = "��¼ʱ���";
		}
		else if(!sz.Compare("��¼ʱ��"))
			lvcol.cx = 90;
		else if(!sz.Compare("��ǰ״̬"))
			lvcol.cx = 80;
		else if(!sz.Compare("��ǰ״̬ʱ��"))
			lvcol.cx = 140;
		else if(!sz.Compare("˽�ж���"))
		{
			lvcol.cx = 100;
			CDataMgr::getInstance()->m_privQueueColumn = sub;
		}
		else if(!sz.Compare("��ǰ״̬ʱ��"))
			lvcol.cx = 100;
		else if(!sz.Compare("����"))
			lvcol.cx = 100;
		else if(!sz.Compare("����"))
			lvcol.cx = 100;	
		else if(!sz.Compare("�ͻ���IP��ַ"))
			lvcol.cx = 100;	

		this->m_agentList.InsertColumn(sub, &lvcol);
		sub ++;
	}


	this->m_sortType=::AGENT_SORT_TYPE_LOGIN_TIME;
	this->m_sortOrder=AGENT_SORT_ORDER_UP;
	m_agentList.MoveWindow(0,40,rect.Width(),rect.Height()-40,1);
	this->m_bInited=TRUE;
	SendMessage(WM_DATACHANGED,NULL,NULL); //֪ͨCAgentDetailFormView

	return 0;
}
