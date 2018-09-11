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

#ifndef APCOMMON_H
#define APCOMMON_H

#ifdef _WIN32
#include <hash_set>
#include <hash_map>
using namespace stdext;
#else
#include <ext/hash_map>
#endif

#include <bgcc.h>
#include <acd.h>


#ifdef _WIN32
const char APCONFIG[]    = "conf\\Ap.cfg";
const char APLOGCONFIG[] = "conf\\Aplog.cfg";
#else
const char APCONFIG[]    = "conf/Ap.cfg";
const char APLOGCONFIG[] = "conf/Aplog.cfg";

using namespace __gnu_cxx;  //for hash_map
namespace __gnu_cxx {
template<> struct hash< std::string > {
    size_t operator()(const std::string& x) const {
        return hash< const char* >()(x.c_str());
    }
};
}
#endif

#ifdef _WIN32
#define int64ld "I64d"
#define uint64ld "U64d"
#else
#define int64ld "ld"
#define uint64ld "lu"
#endif

#include "Queue.h"
using namespace acd;

#define CALLBACKTHREADNUM    4
#define CHECKHBTHREADNUM     2

/**
 * @brief   AP��ACD������״̬
 * @see
 * @note
 * @author  caozhenxing(caozhenxing@baidu.com)
 * @date    2012��06��28�� 12ʱ59��21��
 */
enum APCONNACDSTATE {
    AP_CONNACD_MASTER,
    AP_CONNACD_SLAVE,
    AP_CONNACD_BACKING,
    AP_CONNACD_UNKNOW,
};


/**
 * @brief   AP�����agentbar��Ϣ�����ڻص��¼�����
 * @see
 * @note
 * @author  caozhenxing(caozhenxing@baidu.com)
 * @date    2012��06��28�� 13ʱ03��12��
 */
struct AgentData {
    std::string agentId;            /*��ϯ���ţ���Ψһ��ʾһ��agentbar����*/
    std::string agentDn;            /*��ϯʹ�õĵ绰����*/
    std::string agentPwd;           /*��ϯǩ�������*/
    StatusChangeT statusChangetype; /*�ҶϺ�״̬���Զ�ʾ�С��Զ�ʾæ��*/
    bool autoAnswer;                /*�ҶϺ󣬴�ʾ�л�ʾæ״̬�������ʱ������Ĭ��5��*/
    bool fcSignin;                  /*��ϯ��ǩ�룬�Ƿ�ǿ��ǩ����Ǩ�� */
    std::string skill;              /*���ܺͼ��ܼ�������*/
    int64_t handle;
    int32_t flag;                   /*��ʾ��ϯ�Ƿ�signout*/
    int64_t timestamp;              /*��ʾ��flag��ʱ��*/
    std::string agentIP;
    std::string proxyname;
    std::string proxyname_old;
};

typedef hash_map<std::string, AgentData*> MapAgentIdDataType; /*��ϯ���š����Ӵ�������*/

/**
 * @brief   �¼����нṹ�壬���ڷ��ͻص��¼�
 * @see
 * @note
 * @author  caozhenxing(caozhenxing@baidu.com)
 * @date    2012��06��28�� 14ʱ05��06��
 */
struct ApEventQ {
    Queue<AgentEventT > _qEventCall;
    Queue<MediaEventT> _qEventMedia;
    Queue<RouteEventT> _qEventRoute;
    Queue<OtherEventT > _qEventOther;
};

typedef struct ApEventQ ApEventQT;

#endif // APCOMMON_H

