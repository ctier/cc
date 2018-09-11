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

#include "ButtonEx.h"
#include "StaticEx.h"
#include "CallOut.h"
#include "AcdClient.h"
#include "SipphoneClient.h"

class CCCAgentBarCtrl;

class CCCBar : public CDialog
{
	DECLARE_DYNAMIC(CCCBar)

public:
	CCCBar(CWnd* pParent = NULL);						// ��׼���캯��
	virtual ~CCCBar();

	// �Ի�������
	enum { IDD = IDD_DIALOG_CCBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

// �Զ������
private:
	CCCAgentBarCtrl *p_m_BarCtrl;
	AcdClient m_acd;
	SipphoneClient m_sip;
	CRect m_RectTop;
	CRect m_RectBottom;
	CBrush m_BrushTop;
	CBrush m_BrushBottom;
	acd::AgentStatusT m_AgentStatus;
	ButtonStatus m_lastBtnStatus;
	ButtonStatus m_btnStatus;
	CTime m_beginStatusTime;
	CTime m_lastStatusTime;			// ������ϯ�Լ���ʱ��
	CTime m_beginTalkTime;
	CTime m_lastTalkTime;			// ������ϯ�Լ���ʱ��
	CTimeSpan m_totalTime;			// ����signin֮��Ŀ�ʼͨ��ʱ��
	CCallOut m_dlgCallOut;
	int m_heartbeatCount;
	string m_strActiveCallID;		//��ǰ����е�callid
	LONGLONG m_lDetaTime;

	bgcc::SharedPointer<bgcc::Thread> m_thrd_heartbeat;	//�����߳�
// ˽�к���
private:
	void SetButton();
	// �¼�������
	void PostMessageEx(const acd::AgentStatusT &agentStatus, const acd::CallEventReasonT &reason);
	void PostMessageEx(const acd::OtherEventTypeT &otherEventType);
	void Backup();
	void ReSignIn();

// �ؼ�
private:
	CButtonEx m_btnSignInOut;
	CButtonEx m_btnAnswer;
	CButtonEx m_btnCallOut;
	CButtonEx m_btnHangup;
	CButtonEx m_btnHold;
	CButtonEx m_btnConsult;
	CButtonEx m_btnTransfer;
	CButtonEx m_btnConference;
	CButtonEx m_btnSingleStepTransfer;
	CButtonEx m_btnRest;
	CButtonEx m_btnReady;
	CButtonEx m_btnBusy;
	CButtonEx m_btnSkillChange;
	CButtonEx m_btnFengeOne;
	CButtonEx m_btnFengeTwo;
	CButtonEx m_btnFengeThree;
	CButtonEx m_btnFengeFour;
	CStaticEx m_stcAgentState;
	CStaticEx m_stcStateTime;
	CStaticEx m_stcTalk;
	CStaticEx m_stcTalkTime;
	CStaticEx m_stcTotalTalk;
	CStaticEx m_stcTotalTalkTime;
	CComboBox m_cmbRest;
	CComboBox m_cmbSkill;

// �������غ���Ϣ��Ӧ
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonSigninout();
	afx_msg void OnBnClickedButtonAnswer();
	afx_msg void OnBnClickedButtonCallout();
	afx_msg void OnBnClickedButtonHungup();
	afx_msg void OnBnClickedButtonHold();
	afx_msg void OnBnClickedButtonConsult();
	afx_msg void OnBnClickedButtonTransfer();
	afx_msg void OnBnClickedButtonConference();
	afx_msg void OnBnClickedButtonSinglesteptransfer();
	afx_msg void OnBnClickedButtonRest();
	afx_msg void OnBnClickedButtonReady();
	afx_msg void OnBnClickedButtonBusy();
	afx_msg void OnBnClickedButtonSkillchange();
	afx_msg LRESULT OnAgentEvent(WPARAM wParam, LPARAM lParam);				// ���߳��¼�
	afx_msg LRESULT OnMediaEvent(WPARAM wParam, LPARAM lParam);				// ���߳��¼�
	afx_msg LRESULT OnRouteEvent(WPARAM wParam, LPARAM lParam);				// ���߳��¼�
	afx_msg LRESULT OnOtherEvent(WPARAM wParam, LPARAM lParam);				// ���߳��¼�

// ��������
public:
	CString m_strAgentID;
	CString m_strPassWord;
	CString m_strDN;
	acd::StatusChangeT m_intStatusChangeType;
	BOOL m_intAutoAnswer;
	BOOL m_intForceSignIn;
	CString m_strServiceList;
	CString m_strMainCTIIP;
	CString m_strBackCTIIP;
	LONG m_intCTIPort;
	LONG m_intTimeOut;
	BOOL m_intIsInitial;
	BOOL m_intIsSignIn;
	BOOL m_intIsSIPPhone;
	CString m_strSIPServerIP;
	LONG m_intSIPServerPort;
	CString m_strPhoneNum;
	CString m_strPhonePwd;
	CString m_strAgentTag;
	CString m_strCustomerTag;
	ANICHANGE m_intANIChangeType;
	BOOL m_intIsShowCallOut;
	BOOL m_intIsCallOut;
	Level m_intLogLevel;
	BOOL m_intEnableSeniorCall;

	CString m_callingNo;
	CString m_orgCallingNo;
	CString m_calledNo;
	CString m_orgCalledNo;
	
// ��������
public:
	// �ص�����
	LONG BInitial();
	LONG BUnInitial();
	LONG BSignIn();
	LONG BSignOut();
	LONG BSetIdle();
	LONG BSetBusy();
	LONG BSetRest(const char *reason);
	LONG BGetAgentStatus(acd::AgentStatusT &agentStatus);
	// BSetAgentStatus��BGetAgentStatus�˴���ʵ��
	LONG BResetSkill(const char *skillList);
	LONG BReset();
	LONG BMakeCall(const char *dest, const char *showANI, const char *showDest, acd::CallModeT callMode, acd::CallTypeT callType);
	LONG BAnswer();
	LONG BReleaseCall();
	LONG BHold();
	LONG BRetrieve();
	LONG BConsult(const char *consultNum, const char *showANI, const char *showDest, acd::CallTypeT callType);
	LONG BReconnect();
	LONG BTransfer();
	LONG BSingleStepTransfer(const char *transferNum, const char *showANI, const char *showDest, BOOL transferStyle, acd::CallTypeT callType);
	LONG BTransferIVR(const char *transferNum, const char *callerNum=NULL); //chenyz add 20130220
	LONG BConference();
	// BSetAssociateData��BGetAssociateData�˴���ʵ��
	LONG BListen(const char *destAgentId, const char *forCallerDispNum, const char *forCalleeDispNum);
	LONG BStopListen(const char *destAgentId);
	LONG BInsert(const char *destAgentId, const char *forCallerDispNum, const char *forCalleeDispNum);
	LONG BStopInsert(const char *destAgentId);
	LONG BSwitchInsertorListen(const char *destAgentId, const char *forCallerDispNum, const char *forCalleeDispNum);
	LONG BSendDTMF(const char *digitals);
	LONG BGetRestReason(acd::StringListT &restReason);
	LONG BSynchronizeCTITime();
	LONG BHeartbeat();
	AcdClient &GetAcdClient();
	SipphoneClient &GetSipphoneClient();		// suxiangmao
	// ���������˴���ʵ��

};


class HeartBeatTask : public bgcc::Runnable{
private:
	CCCBar *m_bar;
public:
	HeartBeatTask(CCCBar *_bar):m_bar(_bar){}
	int32_t operator()(const bool *isstopped, void *param=NULL);	//�����߳�ʵ��
};
