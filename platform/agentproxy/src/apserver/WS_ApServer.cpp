#include "WS_ApServer.h"
#include "AgentProxy.h"
#include "../../../../interface/third/json-cpp/include/json/json.h"
using websocketpp::lib::bind;

char* CmdType[]={
	"SignIn","SignOut","SetAgentStatus","GetAgentStatus","ResetStatuschangetype","ResetAutoAnswer",
	"ResetSkill","Reset",
	"OutboundCall","AnswerCall","ReleaseCall","Hold","Retrieve","Consult","ConsultReconnect","ConsultTransfer","SingleStepTransfer","ConsultConference"
	,"ConferenceJoin","SetAssociateData","GetAssociateData","JumptheQueue","ForceSignIn",
	"ResetConfig"
};
WSapserver::WSapserver()
{

}
WSapserver::~WSapserver()
{

}


bool WSapserver::InitApServer(std::string ApListenIp, int32_t ApListenPort, int32_t threadPoolNum)
{

	bool status = true;

	m_ApListenIp = ApListenIp;
	m_ApListenPort = ApListenPort;
	m_threadPoolNum = threadPoolNum;
	m_server=new wsServer();
	return status;
}

bool WSapserver::startServer()
{
	m_server->init_asio();
	m_server->set_message_handler(bind(&WSapserver::on_message,this,m_server,::_1,::_2));
	m_server->set_open_handler(bind(&WSapserver::on_open,this,m_server,::_1));
	m_server->set_close_handler(bind(&WSapserver::on_close,this,m_server,::_1));
	m_server->set_reuse_addr(true);
	m_server->listen(m_ApListenPort);
	m_server->start_accept();
	m_server->run();
	return true;
}
bool WSapserver::stopServer()
{
	m_server->stop();
	m_server->stop_listening();
	return true;
}
void WSapserver::on_open(wsServer* s, websocketpp::connection_hdl hdl)
{
	//BGCC_TRACE("ap", "on_open called with hdl:%d", hdl.lock().get() );
	string msg="connect succeed";
	s->send(hdl, msg.c_str(), msg.length(),frame::opcode::text);
}

void WSapserver::on_close(wsServer* s, websocketpp::connection_hdl hdl)
{
	//BGCC_TRACE("ap", "on_close called with hdl:%d", hdl.lock().get() );

}
void WSapserver::on_message(wsServer *s, websocketpp::connection_hdl hdl, message_ptr msg)
{
	//BGCC_TRACE("ap", "recv message:%s.", msg->get_payload().c_str());
	printf("recv message:%s.\n",msg->get_payload().c_str());
	// check for a special command to instruct the server to stop listening so
	// it can be cleanly exited.
	if (msg->get_payload() == "stop-listening")
	{
		s->stop_listening();
		return;
	}
	try
	{
		//s->send(hdl, msg->get_payload(), msg->get_opcode());
		if(msg->get_payload()!="")
			on_callapclient(s,  hdl,msg->get_payload());
		wsServer::connection_ptr clientconn=	s->get_con_from_hdl(hdl);
		string strhost=clientconn->get_host();
		uri_ptr uri_ = clientconn->get_uri();
		string remoteinfo = clientconn->get_remote_endpoint();
		string anhost = uri_->get_host();
		printf("host:%s\n",strhost.c_str());

	}
	catch (websocketpp::exception const &e)
	{
		//BGCC_WARN("ap", "Echo failed because: %s", e.what());
	}

}
int WSapserver::Getcmdvalue(string strcmd)
{

}
int WSapserver::GetcmdType(string cmdfield)
{
	//CmdType[]
	int size=sizeof(CmdType)/sizeof(CmdType[0]);
	for(int i=0;i<size;i++)
	{
		if(CmdType[i]==cmdfield)
		{
			return i;
		}
	}
	return WSAP_ERRORTYPE;

	
}
void WSapserver::on_callapclient(wsServer *s, websocketpp::connection_hdl hdl,string applicationcmd)
{
	Json::Reader reader;
	Json::Value root;
	if(!reader.parse(applicationcmd,root))
	{
		string respmsg = OnparamError(acd::AcdResultT::ArKeyIsEmpty,"json parse failed","");
		s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
		return;
	}
	if(root["Action"].isNull()||!root["Action"].isString())
	{
		string respmsg = OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose [Action] field","");
		s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
		return;
	}
	int cmdNUM=GetcmdType(root["Action"].asString());
	acd::AcdResultT ret;
	wsServer::connection_ptr clientconn=	s->get_con_from_hdl(hdl);
	string remoteinfo = clientconn->get_remote_endpoint();
	string peerIP=remoteinfo;
	//��ȡ��������
 	Json::Value jsonparam;
 	jsonparam=root["param"];
 	if(jsonparam.isNull())
	{
		string respmsg = OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param field","");
		s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
		return;
	}
	if(cmdNUM==WSAP_ResetConfig) //����acd����
	{
		if(jsonparam["acdpassword"].isNull()||!jsonparam["acdpassword"].isString())
		{
			string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param of Action ResetConfig field","");
			s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
			return;
		}
		string  acdpassword = jsonparam["acdpassword"].asString();
		uint64_t time_rcv = TimeUtil::get_timestamp_ms();
		AgentProxy *agentProxy = AgentProxy::Instance();
		ret = agentProxy->ResetConfig(acdpassword,peerIP,time_rcv);
		string respmsg = Onresponse(ret.get_value(),"","");
		s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
		return;
	}
	if(jsonparam["agentId"].isNull()||!jsonparam["agentId"].isString())
	{
		string respmsg = OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose [agentId] field","");
		s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
		return;
	}
 	string agentId = jsonparam["agentId"].asString();
	int64_t handle=-1;
	if(cmdNUM!=WSAP_SignIn)
	{
		if(jsonparam["handle"].isNull()||!jsonparam["handle"].isInt())
		{
			string respmsg = OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose [handle] field","");
			s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
			return;
		}
		handle = jsonparam["handle"].asInt();
	}
	switch (cmdNUM)
	{
	case WSAP_SignIn:
		{
			if((jsonparam["agentDn"].isNull()||!jsonparam["agentDn"].isString())||(jsonparam["agentPwd"].isNull()||!jsonparam["agentPwd"].isString())||(jsonparam["statusChangetype"].isNull()||!jsonparam["statusChangetype"].isInt())||
				(jsonparam["autoAnswer"].isNull()||!jsonparam["autoAnswer"].isBool())||(jsonparam["fcSignin"].isNull()||!jsonparam["fcSignin"].isBool())||(jsonparam["skills"].isNull()||!jsonparam["skills"].isString()))
			{
				string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param of Action SignIn field",agentId);
				s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
				return;
			}
			string agentDn = jsonparam["agentDn"].asString();
			string agentPwd = jsonparam["agentPwd"].asString();
			acd::StatusChangeT statusChangetype = jsonparam["statusChangetype"].asInt();
			bool autoAnswer = jsonparam["autoAnswer"].asBool();
			bool fcSignin = jsonparam["fcSignin"].asBool();
			string skills = jsonparam["skills"].asString();
			//int64_t handle = jsonparam["handle"].asInt64();
				BGCC_TRACE("ap",
				"SignIn AgentId = %s,AgentDn = %s,AgentPwd = %s,statusChangetype = %d,autoAnswer = %d,fcSignin = %d,skill = %s.",
				agentId.c_str(), agentDn.c_str(), agentPwd.c_str(), statusChangetype.get_value(), autoAnswer,
				fcSignin, skills.c_str());
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->SignIn(agentId, agentDn, agentPwd, statusChangetype, autoAnswer, fcSignin, skills,
				handle, peerIP, time_rcv);
			printf("handle:%lld",handle);
			string respmsg=OnSignIn(ret.get_value(),"",agentId,handle);
			s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
			return;
		}
		break;
	case WSAP_SignOut:
		{
			BGCC_TRACE("ap", "SignOut AgentId = %s,handle = %" int64ld ".", agentId.c_str(), handle);
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->SignOut(handle, agentId, peerIP, time_rcv);
		}
		break;
	case WSAP_SetAgentStatus:
		{
			if((jsonparam["restReason"].isNull()||!jsonparam["restReason"].isString())||(jsonparam["agentStatus"].isNull()||!jsonparam["agentStatus"].isInt()))
			{
				string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param of Action SetAgentStatus field",agentId);
				s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
				return;
			}
			string restReason = jsonparam["restReason"].asString();
			AgentStatusT agentStatus=jsonparam["agentStatus"].asInt();
			BGCC_TRACE("ap",
				"SetAgentStatus AgentId = %s,handle = %" int64ld ",agentStatus = %d,restReason = %s.",
				agentId.c_str(), handle, agentStatus.get_value(), restReason.c_str());
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->SetAgentStatus(handle, agentId, agentStatus, restReason, peerIP, time_rcv);
		}
		break;
	case WSAP_GetAgentStatus:
		{
			acd::AgentStatusT agentStatus;
 			BGCC_TRACE("ap", "GetAgentStatus AgentId = %s,handle = %" int64ld ",agentStatus = %d.",
 				agentId.c_str(), handle, agentStatus.get_value());
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->GetAgentStatus(handle, agentId, agentStatus, peerIP, time_rcv);
			string respmsg = OngetAgentStatus(ret.get_value(),"",agentId,agentStatus.get_value());
			s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);

			return;
		}
		break;
	case WSAP_ResetStatuschangetype:
		{
			if(jsonparam["statusChangetype"].isNull()||!jsonparam["statusChangetype"].isInt())
			{
				string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param of Action ResetStatuschangetype field",agentId);
				s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
				return;
			}
			acd::StatusChangeT statusChangetype = jsonparam["statusChangetype"].asInt();
			//acd::AgentStatusT statusChangetype;
			BGCC_TRACE("ap", "ResetStatuschangetype AgentId = %s,handle = %" int64ld ",statusChangetype = %d.",
				agentId.c_str(), handle, statusChangetype.get_value());
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->ResetStatuschangetype(handle, agentId, statusChangetype, peerIP, time_rcv);
		}
		break;
	case WSAP_ResetSkill:
		{
			if(jsonparam["skill"].isNull()||!jsonparam["skill"].isString())
			{
				string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param of Action ResetSkill field",agentId);
				s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
				return;
			}
			string skill = jsonparam["skill"].asString();
			BGCC_TRACE("ap", "ResetSkill AgentId = %s,handle = %" int64ld ",skill = %s.",
				agentId.c_str(), handle, skill.c_str());
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->ResetSkill(handle, agentId, skill, peerIP, time_rcv);
		}
		break;
	case WSAP_Reset:
		{
			BGCC_TRACE("ap", "Reset AgentId = %s,handle = %" int64ld ".", agentId.c_str(), handle);
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->Reset(handle, agentId, peerIP, time_rcv);
		}
		break;
	case WSAP_ResetAutoAnswer:
		{
			if(jsonparam["autoAnswer"].isNull()||!jsonparam["autoAnswer"].isBool())
			{
				string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param of Action ResetAutoAnswer field",agentId);
				s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
				return;
			}
			bool autoAnswer = jsonparam["autoAnswer"].asBool();
			BGCC_TRACE("ap", "ResetAutoAnswer AgentId = %s,handle = %" int64ld ",autoAnswer = %d.",
				agentId.c_str(), handle, autoAnswer);
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->ResetAutoAnswer(handle, agentId, autoAnswer, peerIP, time_rcv);
		}
		break;
	case WSAP_OutboundCall:
		{
			if((jsonparam["callerId"].isNull()||!jsonparam["callerId"].isString())||(jsonparam["destId"].isNull()||!jsonparam["destId"].isString())||
				(jsonparam["forCallerDispNum"].isNull()||!jsonparam["forCallerDispNum"].isString())||(jsonparam["forCalleeDispNum"].isNull()||!jsonparam["forCalleeDispNum"].isString())||(jsonparam["callMode"].isNull()||!jsonparam["callMode"].isInt())||
				(jsonparam["callType"].isNull()||!jsonparam["callType"].isInt())||(jsonparam["timeout"].isNull()||!jsonparam["timeout"].isInt()))
			{
				string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param of Action OutboundCall field",agentId);
				s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
				return;
			}
			//bool autoAnswer = jsonparam["autoAnswer"].asBool();
			string callerId = jsonparam["callerId"].asString();
			string destId = jsonparam["destId"].asString();
			string forCallerDispNum = jsonparam["forCallerDispNum"].asString();
			string forCalleeDispNum = jsonparam["forCalleeDispNum"].asString();
			const CallModeT callMode =  jsonparam["callMode"].asInt();
			const CallTypeT callType =  jsonparam["callType"].asInt();
			int Timeout = jsonparam["timeout"].asInt();
			BGCC_TRACE("ap",
				"OutboundCall AgentId = %s,handle = %" int64ld ",callerId = %s,destId = %s,forCallerDispNum = %s,forCalleeDispNum = %s,timeout = %d,callMode = %d,callType = %d.",
				agentId.c_str(), handle, callerId.c_str(), destId.c_str(), forCallerDispNum.c_str(),
				forCalleeDispNum.c_str(),
				Timeout, callMode.get_value(), callType.get_value());
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->OutboundCall(handle, agentId, callerId, destId, forCallerDispNum,
				forCalleeDispNum, Timeout, callMode, callType, peerIP, time_rcv);
		}
		break;
	case WSAP_AnswerCall:
		{
			BGCC_TRACE("ap", "AnswerCall AgentId = %s,handle = %" int64ld ".", agentId.c_str(), handle);
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->AnswerCall(handle, agentId, peerIP, time_rcv);
		}
		break;
	case WSAP_ReleaseCall:
		{
			BGCC_TRACE("ap", "ReleaseCall AgentId = %s,handle = %" int64ld ".", agentId.c_str(), handle);
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->ReleaseCall(handle, agentId, peerIP, time_rcv);
		}
		break;
	case WSAP_Hold:
		{
			BGCC_TRACE("ap", "Hold AgentId = %s,handle = %" int64ld ".", agentId.c_str(), handle);
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->Hold(handle, agentId, peerIP, time_rcv);
		}
		break;
	case WSAP_Retrieve:
		{
			BGCC_TRACE("ap", "Retrieve AgentId = %s,handle = %" int64ld ".", agentId.c_str(), handle);
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->RetrieveHeld(handle, agentId, peerIP, time_rcv);
		}
		break;
	case WSAP_Consult:
		{
			if((jsonparam["callerId"].isNull()||!jsonparam["callerId"].isString())||(jsonparam["destId"].isNull()||!jsonparam["destId"].isString())||
				(jsonparam["forCallerDispNum"].isNull()||!jsonparam["forCallerDispNum"].isString())||(jsonparam["forCalleeDispNum"].isNull()||!jsonparam["forCalleeDispNum"].isString())||
				(jsonparam["callType"].isNull()||!jsonparam["callType"].isInt()))
			{
				string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param of Action Consult field",agentId);
				s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
				return;
			}
			string callerId = jsonparam["callerId"].asString();
			string destId = jsonparam["destId"].asString();
			string forCallerDispNum = jsonparam["forCallerDispNum"].asString();
			string forCalleeDispNum = jsonparam["forCalleeDispNum"].asString();
			const CallTypeT callType =  jsonparam["callType"].asInt();
			BGCC_TRACE("ap",
				"Consult AgentId = %s,handle = %" int64ld ",callerId = %s,destId = %s,forCallerDispNum = %s,forCalleeDispNum = %s,callType = %d.",
				agentId.c_str(), handle, callerId.c_str(), destId.c_str(), forCallerDispNum.c_str(),
				forCalleeDispNum.c_str(), callType.get_value());
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->Consult(handle, agentId, callerId, destId, forCallerDispNum, forCalleeDispNum,
				callType, peerIP, time_rcv);
		}
		break;
	case WSAP_ConsultReconnect:
		{
			BGCC_TRACE("ap", "ConsultReconnect AgentId = %s,handle = %" int64ld ".", agentId.c_str(), handle);
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->ConsultReconnect(handle, agentId, peerIP, time_rcv);
		}
		break;
	case WSAP_ConsultTransfer:
		{
			BGCC_TRACE("ap", "ConsultTransfer AgentId = %s,handle = %" int64ld ".", agentId.c_str(), handle);
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->ConsultTransfer(handle, agentId, peerIP, time_rcv);
		}
		break;
	case WSAP_SingleStepTransfer:
		{
			if((jsonparam["callerId"].isNull()||!jsonparam["callerId"].isString())||(jsonparam["destId"].isNull()||!jsonparam["destId"].isString())||(jsonparam["forCallerDispNum"].isNull()||!jsonparam["forCallerDispNum"].isString())||
				(jsonparam["forCalleeDispNum"].isNull()||!jsonparam["forCalleeDispNum"].isString())||(jsonparam["callType"].isNull()||!jsonparam["callType"].isInt())||(jsonparam["isPassthrough"].isNull()||!jsonparam["isPassthrough"].isBool()))
			{
				string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param of Action SingleStepTransfer field",agentId);
				s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
				return;
			}
			string callerId = jsonparam["callerId"].asString();
			string destId = jsonparam["destId"].asString();
			string forCallerDispNum = jsonparam["forCallerDispNum"].asString();
			string forCalleeDispNum = jsonparam["forCalleeDispNum"].asString();
			const CallTypeT transferType =  jsonparam["callType"].asInt();
			bool isPassthrough = jsonparam["isPassthrough"].asBool();
			BGCC_TRACE("ap",
				"SingleStepTransfer AgentId = %s,handle = %" int64ld ",callerId = %s,destId = %s,forCallerDispNum = %s,forCalleeDispNum = %s,isPassthrough = %d,transferType = %d.",
				agentId.c_str(), handle, callerId.c_str(), destId.c_str(), forCallerDispNum.c_str(),
				forCalleeDispNum.c_str(), isPassthrough, transferType.get_value());
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->SingleStepTransfer(handle, agentId, callerId, destId, forCallerDispNum,
				forCalleeDispNum, isPassthrough, transferType, peerIP, time_rcv);
		}
		break;
	case WSAP_ConsultConference:
		{
			BGCC_TRACE("ap", "ConsultConference AgentId = %s,handle = %" int64ld ".", agentId.c_str(), handle);
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->ConsultConference(handle, agentId, peerIP, time_rcv);
		}
		break;
	case WSAP_ConferenceJoin:
		{
			if((jsonparam["callerId"].isNull()||!jsonparam["callerId"].isString())||(jsonparam["destId"].isNull()||!jsonparam["destId"].isString())||(jsonparam["destAgentId"].isNull()||!jsonparam["destAgentId"].isString())||
				(jsonparam["forCallerDispNum"].isNull()||!jsonparam["forCallerDispNum"].isString())||(jsonparam["forCalleeDispNum"].isNull()||!jsonparam["forCalleeDispNum"].isString())||(jsonparam["conferenceType"].isNull()||!jsonparam["conferenceType"].isInt()))
			{
				string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param of Action ConferenceJoin field",agentId);
				s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
				return;
			}
			string callerId = jsonparam["callerId"].asString();
			string destId = jsonparam["destId"].asString();
			const std::string destAgentId =  jsonparam["destAgentId"].asString();
			string forCallerDispNum = jsonparam["forCallerDispNum"].asString();
			string forCalleeDispNum = jsonparam["forCalleeDispNum"].asString();
			const ConferenceT &conferenceType = jsonparam["conferenceType"].asInt();
			BGCC_TRACE("ap",
				"ConferenceJoin AgentId = %s,handle = %" int64ld ",callerId = %s,destAgentId = %s,forCallerDispNum = %s,forCalleeDispNum = %s,conferenceType = %d.",
				agentId.c_str(), handle, callerId.c_str(), destAgentId.c_str(), forCallerDispNum.c_str(),
				forCalleeDispNum.c_str(), conferenceType.get_value());
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->ConferenceJoin(handle, agentId, callerId, destAgentId, forCallerDispNum,
				forCalleeDispNum, conferenceType, peerIP, time_rcv);
		}
		break;
	case WSAP_SetAssociateData:
		{
			if((jsonparam["key"].isNull()||!jsonparam["key"].isString())||(jsonparam["value"].isNull()||!jsonparam["value"].isString()))
			{
				string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param of Action SetAssociateData field",agentId);
				s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
				return;
			}
			string key=jsonparam["key"].asString();
			string strvalue = jsonparam["value"].asString();
			BGCC_TRACE("ap", "SetAssociateData AgentId = %s,handle = %" int64ld ".", agentId.c_str(), handle);
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->SetAssociateData(handle, agentId, key, strvalue, peerIP, time_rcv);
		}
		break;
	case WSAP_GetAssociateData:
		{
			if(jsonparam["key"].isNull()||!jsonparam["key"].isString())
			{
				string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param of Action GetAssociateData field",agentId);
				s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
				return;
			}
			string key=jsonparam["key"].asString();
			string strvalue="";
			BGCC_TRACE("ap", "GetAssociateData AgentId = %s,handle = %" int64ld ".", agentId.c_str(), handle);
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			AcdResultT ret;
			ret = agentProxy->GetAssociateData(handle, agentId, key, strvalue, peerIP, time_rcv);
			string respmsg=Onresponse(ret.get_value(),"",agentId,key,strvalue);
			s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
			return ;
		}
		break;
	case WSAP_JumptheQueue:
		{
			if(jsonparam["requestId"].isNull()||!jsonparam["requestId"].isString())
			{
				string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param of Action JumptheQueue field",agentId);
				s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
				return;
			}
			int64_t requestId = jsonparam["requestId"].asInt();
			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
			AgentProxy *agentProxy = AgentProxy::Instance();
			ret = agentProxy->JumptheQueue(handle, agentId, requestId, peerIP, time_rcv);
		}
		break;
	case WSAP_ResetConfig:
		{
// 			if(jsonparam["acdpassword"].isNull())
// 			{
// 				string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"lose param of Action ResetConfig field",agentId);
// 				s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
// 				return;
// 			}
// 			string  acdpassword = jsonparam["acdpassword"].asString();
// 			uint64_t time_rcv = TimeUtil::get_timestamp_ms();
// 			AgentProxy *agentProxy = AgentProxy::Instance();
// 			ret = agentProxy->ResetConfig(acdpassword,peerIP,time_rcv);
		}
		break;
	default:
		{
			string respmsg=OnparamError(acd::AcdResultT::ArKeyIsEmpty,"unsuport Action",agentId);
			s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);
			return;
		}
		break;
	}
	string respmsg=Onresponse(ret.get_value(),"",agentId);
	s->send(hdl,respmsg.c_str(), respmsg.length(),frame::opcode::text);

}
string WSapserver::Onresponse(int code,string desc,string agentId)
{

	Json::Value respjosn;
	Json::Value jsoncontent;
	Json::Value jsonvalue;
	Json::Value jsondesc;
	//jsonvalue["value"]=0;
	jsonvalue["desc"]=desc;
	jsonvalue["agentId"]=agentId;
	jsoncontent.append(jsonvalue);
/*	jsoncontent.append(jsondesc);*/
	respjosn["result"]=code;
	respjosn["param"]=jsoncontent;
	respjosn.toStyledString();
	return  respjosn.toStyledString();
}
string WSapserver::Onresponse(int code,string desc,string agentId,string strkey2,string param2)
{
	Json::Value respjosn;
	Json::Value jsoncontent;
	Json::Value jsonvalue;
	Json::Value jsondesc;
	jsonvalue["desc"]=desc;
	jsonvalue["agentId"]=agentId;
	jsonvalue[strkey2]=param2;
	jsoncontent.append(jsonvalue);
	jsoncontent.append(jsondesc);
	respjosn["result"]=code;
	respjosn["param"]=jsoncontent;
	respjosn.toStyledString();
	return  respjosn.toStyledString();
}
string WSapserver::Onresponse(int code,string desc,string agentId,string strkey2,int param2)
{
	Json::Value respjosn;
	Json::Value jsoncontent;
	Json::Value jsonvalue;
/*	Json::Value jsondesc;*/
	jsonvalue["desc"]=desc;
	jsonvalue["agentId"]=agentId;
	jsonvalue[strkey2]=param2;
	jsoncontent.append(jsonvalue);
/*	jsoncontent.append(jsondesc);*/
	respjosn["result"]=code;
	respjosn["param"]=jsoncontent;
	respjosn.toStyledString();
	return  respjosn.toStyledString();
}
string WSapserver::OnSignIn(int code,string desc,string agentId,int64_t handle)
{
	Json::Value respjosn;
	Json::Value jsoncontent;
	Json::Value jsonvalue;
	Json::Value jsondesc;
	Json::Value jsonhandle;
	//jsonvalue["value"]=0;
	jsonvalue["desc"]=desc;
	jsonvalue["agentId"]=agentId;
	jsonvalue["handle"]=(Json::Int64)handle;
	jsoncontent.append(jsonvalue);
	respjosn["result"]=code;
	respjosn["param"]=jsoncontent;
	return  respjosn.toStyledString();
}
string WSapserver::OngetAgentStatus(int code,string desc,string agentId,int status)
{
	return Onresponse(code,desc,agentId,"agentStatus",status);
}
string WSapserver::OnparamError(int code,string desc,string agentId)
{
	return Onresponse(code,desc,agentId);
}