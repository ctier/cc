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
#include "Tool.h"

Logger Tool::m_Logger;

void *Tool::p_Bar = NULL;

void Tool::GetVersion(HINSTANCE handle, LongString version)
{
	char fileName[MAX_PATH];
	DWORD re = GetModuleFileName(handle, fileName, MAX_PATH);
	if(re == 0)
	{
		return;
	}
	DWORD size = GetFileVersionInfoSize(fileName, NULL);
	if(size == 0)
	{
		return;
	}
	char *pData = new char[size];
	re = GetFileVersionInfo(fileName, 0, size, pData);
	if(!re)
	{
		delete[] pData;
		return;
	}
	VS_FIXEDFILEINFO *pInfo;
	UINT nLength;
	re = VerQueryValue(pData, "\\", reinterpret_cast<void**>(&pInfo), &nLength);
	if(!re)
	{
		delete[] pData;
		return;
	}
	snprintf(version, LONG_STRING_LENGHT, "Version: %d.%d.%d.%d", HIWORD(pInfo->dwFileVersionMS), LOWORD(pInfo->dwFileVersionMS),
		HIWORD(pInfo->dwFileVersionLS), LOWORD(pInfo->dwFileVersionLS));
	delete[] pData;
}

void Tool::Init(HINSTANCE handle)
{
	LongString version;
	GetVersion(handle, version);
	m_Logger.Initialize(50000, 50, "C:\\Log\\CCAgentBar\\", "CCAgentBar", version);
}

void Tool::UnInit()
{
	m_Logger.WriteLog("Exist");
}

void Tool::GetAcdResult_Str(LONG input, string &output)
{
	switch(input)
	{
	case acd::AcdResultT::ArSuccess:
		output = "�ӿڵ��óɹ�";
		break;
	case acd::AcdResultT::ArAgentIdIsEmpty:
		output = "��ϯ����Ϊ��";
		break;
	case acd::AcdResultT::ArAgentDnIsEmpty:
		output = "��ϯ�ֻ�Ϊ��";
		break;
	case acd::AcdResultT::ArSkillIsEmpty:
		output = "��ϯ����Ϊ��";
		break;
	case acd::AcdResultT::ArCallerIdIsEmpty:
		output = "·�ɱ�ʶΪ��";
		break;
	case acd::AcdResultT::ArDestIdIsEmpty:
		output = "Ŀ����ϯ����Ϊ��";
		break;
	case acd::AcdResultT::ArDestDnIsEmpty:
		output = "Ŀ����ϯ�ֻ�Ϊ��";
		break;
	case acd::AcdResultT::ArKeyIsEmpty:
		output = "��·���ݼ�Ϊ��";
		break;
	case acd::AcdResultT::ArValueIsEmpty:
		output = "��·����ֵΪ��";
		break;
	case acd::AcdResultT::ArDigitalsIsEmpty:
		output = "���β���ֵΪ��";
		break;
	case acd::AcdResultT::ArFilenameIsEmpty:
		output = "�ļ���Ϊ��";
		break;
	case acd::AcdResultT::ArAgentIdNotExist:
		output = "��ϯ���Ų�����";
		break;
	case acd::AcdResultT::ArAgentPasswordError:
		output = "��ϯ�������";
		break;
	case acd::AcdResultT::ArAgentSkillError:
		output = "��ϯ���ܴ���";
		break;
	case acd::AcdResultT::ArNotMaster:
		output = "������";
		break;
	case acd::AcdResultT::ArAgentDnBeOccupied:
		output = "��ϯ�ֻ���ռ��";
		break;
	case acd::AcdResultT::ArHasSigned:
		output = "��ǩ��";
		break;
	case acd::AcdResultT::ArNoSignin:
		output = "δǩ��";
		break;
	case acd::AcdResultT::ArDestAgentIdNotExist:
		output = "Ŀ����ϯ���Ų�����";
		break;
	case acd::AcdResultT::ArDestAgentDnNotExist:
		output = "Ŀ����ϯ�ֻ�������";
		break;
	case acd::AcdResultT::ArSkillNotExist:
		output = "���ܲ�����";
		break;
	case acd::AcdResultT::ArRouteRequestNotExist:
		output = "·�����󲻴���";
		break;
	case acd::AcdResultT::ArAgentStatusIsWrong:
		output = "��ϯ״̬����";
		break;
	case acd::AcdResultT::ArImsError:
		output = "ims����";
		break;
	case acd::AcdResultT::ArPasswordError:
		output = "�������";
		break;
	case acd::AcdResultT::ArFailed:
		output = "��������";
		break;

	case AGENTBARERROR_BGCC_CLIENTPROXYFAIL:
		output = "ͨ�Ŷ˿ڳ�ʼ������";
		break;
	case AGENTBARERROR_BGCC_EXCEPTION:
		output = "ͨ�Ŵ���";
		break;
	case AGENTBARERROR_BAR_INITIAL:
		output = "�ؼ�״̬Ϊ�ѳ�ʼ��";
		break;
	case AGENTBARERROR_BAR_UNINITIAL:
		output = "�ؼ�״̬Ϊδ��ʼ��";
		break;
	case AGENTBARERROR_BAR_UNSIGNIN:
		output = "�ؼ�״̬Ϊδǩ��";
		break;
	case AGENTBARERROR_BAR_WRONGSTATE:
		output = "�ؼ�״̬����";
		break;
	case AGENTBARERROR_BAR_OUTEVENTERROR:
		output = "�ؼ������¼�����";
		break;
	case AGENTBARERROR_BAR_ANSWERERROR:
		output = "�ؼ�Ӧ�����";
		break;
	case AGENTBARERROR_JSON_READERROR:
		output = "JSON��������";
		break;
	case AGENTBARERROR_SOFTPHONE_ERROR:
		output = "SIP�绰����";
		break;

	default:
		output = "overflow!";
		break;
	}
}

void Tool::WriteAgentEventLog(const acd::AgentEventT *agentEvent, const char *type)
{
	assert(agentEvent != NULL);

	CString str;
	CTime ct(agentEvent->timestamp / 1000000);
	str.Format("%04d-%02d-%02d %02d:%02d:%02d.%06d", ct.GetYear(), ct.GetMonth(), ct.GetDay(),ct.GetHour(), ct.GetMinute(), ct.GetSecond(), 
		agentEvent->timestamp % 1000000);

	m_Logger.WriteLog(Level_High, "=========================AgentEvent=========================");
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s]          agentId: %s",    type, agentEvent->agentId.c_str());
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s]          eventId: %s",    type, agentEvent->eventId.get_desc().c_str());
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s]          agentDn: %s",    type, agentEvent->agentDn.c_str());
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s]           callId: %s",    type, agentEvent->callId.c_str());
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s]        sessionId: %I64d", type, agentEvent->sessionId);
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s]      agentStatus: %s",    type, agentEvent->agentStatus.get_desc().c_str());
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s]   originalCallid: %s",    type, agentEvent->originalCallid.c_str());
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s]         partyNum: %d",    type, agentEvent->partyNum);
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s]       otherParty: %s",    type, agentEvent->otherParty.c_str());
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s]        otherAttr: %s",    type, agentEvent->otherAttr.get_desc().c_str());
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s] originatingParty: %s",    type, agentEvent->originatingParty.c_str());
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s]      originalAni: %s",    type, agentEvent->originalAni.c_str());
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s]     originalDnis: %s",    type, agentEvent->originalDnis.c_str());
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s]           reason: %s",    type, agentEvent->reason.get_desc().c_str());
	m_Logger.WriteLog(Level_High, "[AgentEvent][%s]        timestamp: %s",    type, str.GetString());
}

void Tool::WriteMediaEventLog(const acd::MediaEventT *mediaEvent, const char *type)
{
	assert(mediaEvent != NULL);

	CString str;
	CTime ct(mediaEvent->timestamp / 1000000);
	str.Format("%04d-%02d-%02d %02d:%02d:%02d.%06d", ct.GetYear(), ct.GetMonth(), ct.GetDay(),ct.GetHour(), ct.GetMinute(), ct.GetSecond(), 
		mediaEvent->timestamp % 1000000);

	m_Logger.WriteLog(Level_High, "=========================MediaEvent=========================");
	m_Logger.WriteLog(Level_High, "[MediaEvent][%s]          agentId: %s",    type, mediaEvent->agentId.c_str());
	m_Logger.WriteLog(Level_High, "[MediaEvent][%s]        eventType: %s",    type, mediaEvent->eventType.get_desc().c_str());
	m_Logger.WriteLog(Level_High, "[MediaEvent][%s]          agentDn: %s",    type, mediaEvent->agentDn.c_str());
	m_Logger.WriteLog(Level_High, "[MediaEvent][%s]           callId: %s",    type, mediaEvent->callId.c_str());
	m_Logger.WriteLog(Level_High, "[MediaEvent][%s]        sessionId: %I64d", type, mediaEvent->sessionId);
	m_Logger.WriteLog(Level_High, "[MediaEvent][%s]            state: %s",    type, mediaEvent->state.get_desc().c_str());
	m_Logger.WriteLog(Level_High, "[MediaEvent][%s]           reason: %s",    type, mediaEvent->reason.get_desc().c_str());
	m_Logger.WriteLog(Level_High, "[MediaEvent][%s]       otherParty: %s",    type, mediaEvent->otherParty.c_str());
	m_Logger.WriteLog(Level_High, "[MediaEvent][%s]           buffer: %s",    type, mediaEvent->buffer.c_str());
	m_Logger.WriteLog(Level_High, "[MediaEvent][%s]        timestamp: %s",    type, str.GetString());
}

void Tool::WriteRouteEventLog(const acd::RouteEventT *routeEvent, const char *type)
{
	assert(routeEvent != NULL);

	CString str;
	CTime ct(routeEvent->timestamp / 1000000);
	str.Format("%04d-%02d-%02d %02d:%02d:%02d.%06d", ct.GetYear(), ct.GetMonth(), ct.GetDay(),ct.GetHour(), ct.GetMinute(), ct.GetSecond(), 
		routeEvent->timestamp % 1000000);

	m_Logger.WriteLog(Level_High, "=========================RouteEvent=========================");
	m_Logger.WriteLog(Level_High, "[RouteEvent][%s]          agentId: %s",    type, routeEvent->agentId.c_str());
	m_Logger.WriteLog(Level_High, "[RouteEvent][%s]        eventType: %s",    type, routeEvent->eventType.get_desc().c_str());
	m_Logger.WriteLog(Level_High, "[RouteEvent][%s]          agentDn: %s",    type, routeEvent->agentDn.c_str());
	m_Logger.WriteLog(Level_High, "[RouteEvent][%s]           callId: %s",    type, routeEvent->callId.c_str());
	m_Logger.WriteLog(Level_High, "[RouteEvent][%s]        sessionId: %I64d", type, routeEvent->sessionId);
	m_Logger.WriteLog(Level_High, "[RouteEvent][%s]        requestId: %I64d", type, routeEvent->requestId);
	m_Logger.WriteLog(Level_High, "[RouteEvent][%s]      originalAni: %s",    type, routeEvent->originalAni.c_str());
	m_Logger.WriteLog(Level_High, "[RouteEvent][%s]     originalDnis: %s",    type, routeEvent->originalDnis.c_str());
	m_Logger.WriteLog(Level_High, "[RouteEvent][%s]        timestamp: %s",    type, str.GetString());
}

void Tool::WriteOtherEventLog(const acd::OtherEventT *otherEvent, const char *type)
{
	assert(otherEvent != NULL);

	CString str;
	CTime ct(otherEvent->timestamp / 1000000);
	str.Format("%04d-%02d-%02d %02d:%02d:%02d.%06d", ct.GetYear(), ct.GetMonth(), ct.GetDay(),ct.GetHour(), ct.GetMinute(), ct.GetSecond(), 
		otherEvent->timestamp % 1000000);

	m_Logger.WriteLog(Level_High, "=========================OtherEvent=========================");
	m_Logger.WriteLog(Level_High, "[OtherEvent][%s]          agentId: %s",    type, otherEvent->agentId.c_str());
	m_Logger.WriteLog(Level_High, "[OtherEvent][%s]        eventType: %s",    type, otherEvent->eventType.get_desc().c_str());
	m_Logger.WriteLog(Level_High, "[OtherEvent][%s]          agentDn: %s",    type, otherEvent->agentDn.c_str());
	m_Logger.WriteLog(Level_High, "[OtherEvent][%s]           callId: %s",    type, otherEvent->callId.c_str());
	m_Logger.WriteLog(Level_High, "[OtherEvent][%s]        sessionId: %I64d", type, otherEvent->sessionId);
	m_Logger.WriteLog(Level_High, "[OtherEvent][%s]      originalAni: %s",    type, otherEvent->originalAni.c_str());
	m_Logger.WriteLog(Level_High, "[OtherEvent][%s]     originalDnis: %s",    type, otherEvent->originalDnis.c_str());
	m_Logger.WriteLog(Level_High, "[OtherEvent][%s]        timestamp: %s",    type, str.GetString());
}

void Tool::WriteAgentIdListLog(const char *agentId, const acd::StringListT &destAgentIdList)
{
	for(acd::StringListT::const_iterator it = destAgentIdList.begin(); it != destAgentIdList.end(); it++)
	{
		m_Logger.WriteLog(Level_Super, "[AcdClient-AcdGetAgentDetailsByAgentIDs] agentId:%s destAgentId:%s", agentId, (*it).c_str());
	}
}

void Tool::WriteAgenDntListLog(const char *agentId, const acd::StringListT &destAgentDnList)
{
	for(acd::StringListT::const_iterator it = destAgentDnList.begin(); it != destAgentDnList.end(); it++)
	{
		m_Logger.WriteLog(Level_Super, "[AcdClient-AcdGetAgentDetailsByAgentDNs] agentId:%s destAgentDn:%s", agentId, (*it).c_str());
	}
}

void Tool::WriteRestReasonLog(const acd::StringListT &restReason)
{
	for(acd::StringListT::const_iterator it = restReason.begin(); it != restReason.end(); it++)
	{
		m_Logger.WriteLog(Level_Super, "[AcdClient-AcdGetRestReason] [out] restReason:%s", (*it).c_str());
	}
}
