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

#ifndef  __IMS_CORE_H_
#define  __IMS_CORE_H_

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <list>
#include <map>
#include <set>
#include <boost/functional/hash.hpp>

#define LEN_8  8
#define LEN_16 16
#define LEN_32 32
#define LEN_64 64
#define LEN_128 128
#define LEN_256 256
#define LEN_512 512

#define IMS_FAIL_GENERAL        -1
#define IMS_SUCCESS             0
#define IMS_FAIL_LOCK           1
#define IMS_FAIL_EXISTS         2
#define IMS_FAIL_NOTEXISTS      3
#define IMS_FAIL_MEM            4
#define IMS_FAIL_INVALID_FSNO   5
#define IMS_FAIL_ALL_BUSY       6
#define IMS_FAIL_CONNECT        7
#define IMS_FAIL_TIMEOUT        8

#define IMS_CONF_FILE ./conf/ims.conf.xml
#define DEFAULT_HOLDON_MUSIC "${sound_prefix}/hold_music.wav"

#define IMS_WAIT_RING_TIMEOUT   30000
#define IMS_WAIT_ANSWER_TIMEOUT 60000

#define SPECIAL_CHANNELID "create empty session by ivr"

typedef uint64_t callid_t;

/**
* @brief session ��ǰ����
*
*/
enum ims_session_operation_t {
    SOPR_UNKNOWN,
    SOPR_MAKECALL,
    SOPR_HOLD,
    SOPR_RETRIEVE,
    SOPR_CONSULT,
    SOPR_RECONNECT,
    SOPR_TRANSFER,
    SOPR_CONFERENCE,
    SOPR_SINGLESTEPTRANSFER,
    SOPR_LISTEN,
    SOPR_INSERT,
    SOPR_IVRTRANSFER            /* transferAgent������ʱ*/
};

/**
* @brief sessiong ����ԭ��
*
*/
enum ims_session_create_reason_t {
    SCRT_UNKNOWN,
    SCRT_INBOUND_IVR,         ///<  IVR ����
    SCRT_INBOUND_NORMAL,      ///<  ֱ�Ӻ���
    SCRT_OUTBOUND_IVR,        ///<  IVR���
    SCRT_OUTBOUND_NORMAL      ///<  ��ϯ����
};

/**
 * @brief state of channel when signaled
 */
enum ims_state_signal_t {
    STATE_UNKNOWN,
    STATE_RING,
    STATE_ANSWER,
};

typedef struct ims_global_data {
    const static uint32_t MAX_SESSION = 2048;         ///<  ��̨IMS֧�ֵ�session
    const static uint32_t MAX_REQUEST = 1024;         ///<  ���ͻ��˶������
    const static uint32_t DN_SIZE = 64;               ///<  ��ϯ�ֻ���󳤶�
    const static uint32_t MAX_OBJ_PER_SESSION = 16;   ///<  session�и��ֶ����������
    ///<  ����session������ʱ��24Сʱ, ��λ΢��
    const static uint64_t MAX_SESSION_LIFETIME = 24ll * 60 * 60 * 1000000;
    ///<  ��session������ʱ��1Сʱ, ��λ΢��
    const static uint64_t MAX_EMPTY_SESSION_LIFETIME = 1ll * 60 * 60 * 1000000;
} global_data_t;


struct hashfunctor {
    size_t operator()(const char* pstr) const {
        std::string str(pstr);
        boost::hash<std::string> hash_string;
        return hash_string(str);
    }
};

struct ims_equal {
    /**
     * @brief char * ��stl�еıȽϺ���
     *
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2012/11/15 14:01:28
    **/
    inline bool operator()(const char* _1, const char* _2) const {
        if (!_1 && !_2) {
            return true;
        }

        if (!_1 || !_2) {
            return false;
        }

        return strcmp(_1, _2) == 0;
    }
};

#endif  //__IMS_CORE_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
