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

#ifndef  __FS_OPERATOR_H_
#define  __FS_OPERATOR_H_

#include <esl.h>

#include "common.h"

/**
 * @brief fs������
 *   detail description
 *
 */
class fs_operator_t {
protected:
    const static uint32_t MAX_DATA_SIZE = 255;
public:
    typedef struct _fs_event_t {
        enum  event_t {
            EV_ANSWER_SUCC,

            EV_HANGUP_SUCC,

            EV_PLAY_SUCC,
            EV_COLLECT_SUCC,

            EV_DISCONNECT,
            EV_DISCONNECT_OTHER,
            EV_DISCONNECT_THIRD,
            EV_BRIDGE_SUCC,
            EV_RECORD_BEGIN_SUCC,
            EV_RECORD_END_SUCC,
            EV_TIMEOUT,
            EV_UNKNOWN        ///<  δ֪
        };
        enum media_response_t {
            MEDIA_SUCC,
            MEDIA_FILE_NOEXIST,
            MEDIA_ERR
        };
        event_t event;
        media_response_t media_resp;
        char data[MAX_DATA_SIZE + 1];
    } fs_event_t;
public:

    /**
     * @brief ����fs
     *
     * @param [in/out] handle   : esl_handle_t&
     * @param [in/out] ip   : const char*
     * @param [in/out] port   : uint32_t
     * @param [in/out] user   : const char*
     * @param [in/out] pswd   : const char*
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/09/01 10:04:38
    **/
    static bool connect(esl_handle_t& handle,
                        const char* ip, uint32_t port,
                        const char* user, const char* pswd);

    /**
     * @brief ��ȡ������ֵ
     *
     * @param [in/out] handle   : esl_handle_t*
     * @param [in/out] var   : const char*
     * @return  std::string
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/09/06 18:51:38
    **/
    static std::string get_variable(esl_handle_t* handle, const char* var);

    /**
     * @brief �Ͽ���fs������
     *
     * @param [in/out] handle   : esl_handle_t&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/09/01 10:04:43
    **/
    static inline bool disconnect(esl_handle_t& handle) {
        bool ret = (esl_disconnect(&handle) == ESL_SUCCESS);
        bzero(&handle, sizeof(esl_handle_t));

        return ret;
    }

    /**
     * @brief �����¼�
     *
     * @param [in/out] handle   : esl_handle_t&
     * @param [in/out] type   : esl_event_type_t
     * @param [in/out] val   : const char*
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/09/01 10:52:50
    **/
    static inline bool events(esl_handle_t& handle, esl_event_type_t type, const char* val) {
        return esl_events(&handle, type, val) == ESL_SUCCESS;
    }

    /**
     * @brief Ӧ��
     *
     * @param [in/out] handle   : esl_handle_t*
     * @param [in/out] callid   : const string&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/08/29 11:39:48
     **/
    static bool answer(esl_handle_t* handle, const std::string& callid);

    /**
     * @brief
     *
     * @param [in/out] handle   : esl_handle_t*
     * @param [in/out] caller   : const std::string&
     * @param [in/out] called   : const std::string&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/10/12 12:06:24
    **/
    static bool bridgeex(esl_handle_t* handle,
                         const std::string& callid,
                         const std::string& caller, const std::string& called,
                         bool external, bool usebgm, const std::string& bgmfile);


    /**
     * @brief inband ��dtmf�պŲ���
     *
     * @param [in/out] handle   : esl_handle_t*
     * @param [in/out] callid   : const std::string&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/10/11 21:20:41
    **/
    static bool startdtmf(esl_handle_t* handle, const std::string& callid);

    /**
     * @brief inband ��dtmf�պŲ���
     *
     * @param [in/out] handle   : esl_handle_t*
     * @param [in/out] callid   : const std::string&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/10/11 21:20:41
    **/
    static bool stopdtmf(esl_handle_t* handle, const std::string& callid);

    /**
     * @brief �һ�
     *
     * @param [in/out] handle   : esl_handle_t*
     * @param [in/out] callid   : const string&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/08/29 11:40:32
     **/
    static bool hangup(esl_handle_t* handle, const string& callid);
    /**
     * @brief ����
     *
     * @param [in/out] handle   : esl_handle_t*
     * @param [in/out] callid   : const string&
     * @param [in/out] playfile   : const string&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/08/29 11:40:38
     **/
    static bool playback(esl_handle_t* handle,
                         const string& callid, const string& playfile);

    /**
     * @brief
     *
     * @param [in/out] handle   : esl_handle_t*
     * @param [in/out] callid   : string&
     * @param [in/out] playfile   : string&
     * @param [in/out] min   : uint32_t
     * @param [in/out] max   : uint32_t
     * @param [in/out] timeout   : uint32_t ��sΪ��λ
     * @param [in/out] term   : string ������
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/08/29 11:44:53
    **/
    static bool collect(esl_handle_t* handle,
                        const std::string& callid, const std::string& playfile,
                        uint32_t min, uint32_t max, uint32_t timeout,
                        const std::string term);
    /**
     * @brief �����պ�
     *
     * @param [in/out] handle   : esl_handle_t*
     * @param [in/out] callid   : string&
     * @param [in/out] playfile   : string&
     * @param [in/out] min   : uint32_t
     * @param [in/out] max   : uint32_t
     * @param [in/out] tries   : uint32_t
     * @param [in/out] timeout   : uint32_t
     * @param [in/out] term   : string&
     * @param [in/out] invalidfile   : string&
     * @param [in/out] varname   : string
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/08/29 11:40:49
     **/
    static bool play_and_get_digits(esl_handle_t* handle,
                                    const std::string& callid, const std::string& playfile,
                                    uint32_t min, uint32_t max,
                                    uint32_t tries, uint32_t timeout,
                                    const std::string& term,
                                    const std::string& invalidfile);

    /**
     * @brief ���
     *
     * @param [in/out] handle   : esl_handle_t*
     * @param [in/out] callid   : string&
     * @param [in/out] called_number   : string&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/08/29 11:42:33
     **/
    static bool dial(esl_handle_t* handle,
                     std::string& callid, const std::string& called,
                     const std::string& caller, uint32_t timeout, bool external);

    /**
     * @brief �绰���
     *
     * @param [in/out] handle   : esl_handle_t*
     * @param [in/out] callid1   : const std::string&
     * @param [in/out] callid2   : const std::string&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/08/29 11:43:49
     **/
    static bool bridge(esl_handle_t* handle,
                       const std::string& callid1, const std::string& callid2);
    /**
     * @brief ¼��
     *
     * @param [in/out] handle   : esl_handle_t*
     * @param [in/out] callid   : const std::string&
     * @param [in/out] recordfile   : const std::string&
     * @return  bool
     * @retval
     * @see
     * @note Ŀǰ�޴���Ŀ¼�Ĺ��ܣ���Ҫ����
     *       ��һ�������ĺ�����record֮���޷��������������Ĳ���
     * @author chenyuzhen
     * @date 2011/08/29 11:44:04
     **/
    static bool record(esl_handle_t* handle,
                       const std::string& callid,
                       const std::string& recordpath,
                       const std::string& recordfile);

    /**
     * @brief ��ȡ��������
     *
     * @param [in/out] handle   : esl_handle_t*
     * @param [in/out] callid   : string&
     * @param [in/out] varname   : string&
     * @return  const char*
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/08/29 11:44:11
     **/
    //static const char* uuid_getvar(esl_handle_t* handle,
    //        const string& callid, const string& varname);

    /**
     * @brief ��ȡ��������Ϣ
     *
     * @param [in/out] hdl   : esl_handle_t*
     * @param [in/out] caller   : std::string&
     * @param [in/out] called   : std::string&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/08/31 14:34:17
    **/
    static bool get_callercalled(esl_handle_t& hdl, std::string& caller, std::string& called);

    /**
     * @brief ��ȡcallid
     *
     *
     * @param [in/out] hdl   : esl_handle_t*
     * @param [in/out] callid   : std::string&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/08/31 14:40:59
    **/
    static bool get_callid(esl_handle_t& hdl, std::string& callid);

protected:
    const static char* CMD_ANSWER;
    const static char* CMD_HANGUP;
    const static char* CMD_PLAY;
    const static char* CMD_COLLECT;
    const static char* CMD_PLAY_WITH_COLLECT;
    const static char* CMD_DIAL;
    const static char* CMD_RECORD;
    const static char* CMD_BRIDGE;
    const static char* CMD_GETVAR;
    const static char* CMD_END;
    const static char* CMD_UNDEF;
    const static char* CMD_SET;
    const static char* CMD_MKDIR;
    const static char* CMD_CREATEUUID;
    const static char* CMD_STARTDTMF;
    const static char* CMD_STOPDTMF;
    const static char* CMD_FLUSH_DTMF;
    const static char* CMD_BRIDGEEX;

    const static char* RESULT_OK;
    const static char* REPLY_TEXT;
    const static char* CONTENT_TYPE;
    const static char* PLAIN;
    const static char* APPLICATION;
    const static char* APP_RESP;
    const static char* DISCONNECT;

    const static char* MEDIA_FILE_PLAYED;
    const static char* MEDIA_FILE_NOEXIST;

    const static char* CH_ST_HANGUP;
    const static char* CH_ST;

    const static char* FS_VAR_TMP;  ///< �¼���ʱ����������

    const static char* FS_VAR_PREFIX;

    const static char* FS_DIGIT_RESULT_SUCC;
    const static char* FS_DIGIT_RESULT_FAIL;
    const static char* FS_DIGIT_RESULT_TO;
    const static char* FS_VAR_DIGIT_READ;

private:
    /**
     * @brief �ж�esl��response�Ƿ�ɹ�
     *
     * @param [in/out] result   : const char*
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/08/29 11:38:23
     **/
    static inline bool check_result(const char* result);
public:

    /**
     * @brief
     *
     * @param [in] handle   : esl_handle_t*
     * @param [in] timeout   : int32_t -1:�޳�ʱ,>0��ʱn��
     * @param [in/out] : fs_event_t �¼�����
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/08/29 14:42:22
    **/
    static bool check_event(esl_handle_t* handle, int32_t timeout, fs_event_t& event);

    /**
     * @brief ����uuid�������ʱ���õ�
     *
     * @param [in/out] handle   : esl_handle_t&
     * @return  std::string
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/10/11 20:24:03
    **/
    static std::string create_uuid(esl_handle_t& handle);
};














#endif  //__INCLUDE/FS_OPERATOR_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
