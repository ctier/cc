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

#ifndef _IVR_INSTANCE_MANAGER_H_
#define _IVR_INSTANCE_MANAGER_H_

#include <queue>
#include <map>
#include <pthread.h>
#include <stdint.h>

//#include "ivr_event.h" //ivr_base_event_t
#include "ivr_type.h"  //some macros
#include "common.h"

class IvrInstanceManager {
public:
    /**
     * @brief   ���캯��
     * @param
     * @return
     */
    IvrInstanceManager();

    /**
     * @brief   ���캯��
     * @param
     * @return
     */
    ~IvrInstanceManager();

    /**
     * @brief   ��ȡIvrInstanceManager����
     * @param
     * @return  IvrInstanceManager����ָ��
     */
    static IvrInstanceManager* get_instance();

    /**
     * @brief   ��ʼ��IvrInstanceManager
     * @param
     * @return  �ɹ�����IVR_SUCCESS;���򷵻ش�����
     */
    int32_t init();

    /**
     * @brief   ����IvrInstanceManager
     * @param
     * @return  �ɹ�����IVR_SUCCESS;���򷵻ش�����
     */
    int32_t destroy();

    /**
     * @brief   ����ivrʵ��
     * @param   type ����ʵ�����ͣ���Ϊauto��inbound����
     * @param   freeswitch_no freeswitch���
     * @param   id [out] �����ɹ���IVRʵ��ID
     * @param   dynmic_resoure ��̬��Դ������queue, sem_t, IvrTimer���ߵ�ָ�룩
     * @param   uuid ͨ����ʶ
     * @return  �ɹ�����IVR_SUCCESS;���򷵻ش�����
     * @note ����id���ɵ���������ռ䡣�������ɹ��󣬳ɹ�������IVRʵ��ID�ᱻд��id
     */
    int32_t create_ivr_instance(int32_t type, fw_id_t freeswith_no, ivr_session_id_t* id,
                                session_dynamic_resource_t dynmic_resoure, const string& uuid);

    /**
     * @brief   ����ivrʵ��
     * @param   id �������ٵ�IVRʵ��id
     * @return  �ɹ�����IVR_SUCCESS;���򷵻ش�����
     */
    int32_t destroy_ivr_instance(ivr_session_id_t id);

    /**
     * @brief   ����IVRʵ���Ƿ����
     * @param   id �����ҵ�IVRʵ��ID
     * @return  ���ڷ���true;���򷵻�false
     */
    bool  search_ivr_instance(ivr_session_id_t id);

    /**
     * @brief   ��IVR�¼������Ӧ��IVR ID�¼�������
     * @param   event Ivr�¼� event���Ѿ���ivr_session_id_t�����ڽӿ��в������ivr_session_id_tһ��
     * @return  �ɹ�����IVR_SUCCESS;���򷵻ش�����
     */
    int32_t put_ivr_event(ivr_base_event_t* event);

    /**
     * @brief   ��ȡָ��ID��IVR�¼����еĶ����¼������������Ƴ�
     * @param   sid IVR�¼�ID
     * @param   event �¼�
     * @return
     */
    int32_t get_ivr_event(ivr_session_id_t sid, ivr_base_event_t** event);

    /**
     * @brief   ���ָ��IVRʵ�����¼�����
     * @param   id IVRʵ��ID
     * @return  �ɹ�����IVR_SUCCESS;���򷵻ش�����
     */
    int32_t clear_event_queue(ivr_session_id_t id);


    /**
     * @brief   ���uuid�Ƿ����
     * @param   uuid ������uuid
     * @param   sid ��uuid���ڣ���uuid��Ӧ��session idд��sid
     * @return  ��uuid���ڣ�����true�����򷵻�false
     */
    bool check_uuid(const std::string& uuid, ivr_session_id_t& sid);

    /**
     * @brief   ����uuid��sessionid�Ĺ�ϵ
     * @param   sid session id
     * @param   uuid ����ӵ�uuid
     * @return  �ɹ�����IVR_SUCCESS;���򷵻ش�����
     */
    int32_t add_uuid(ivr_session_id_t sid, const std::string& uuid);

    int32_t add_channel_info(const std::string& uuid, const channel_info_t& info);

    int32_t get_channel_info(const std::string& uuid, channel_info_t& info);

public:

    // set flag to mark flow had(or not) transagent
    //  _trans_valid[sid] = valid;
    void mark_transagent(ivr_session_id_t sid, bool valid);

    // test wether flow had transagent
    //  return _trans_valid[sid]
    bool is_transagent(ivr_session_id_t sid);
private:
    /**
     * @brief   ɾ��uuid��sessionid�Ĺ�ϵ
     * @param   uuid
     * @return  �ɹ�����IVR_SUCCESS;���򷵻ش�����
     */
    int32_t remove_uuid(const std::string& uuid);

    int32_t remove_uuid(ivr_session_id_t sid);

    int32_t remove_tav(ivr_session_id_t sid);

private:

    /** sessionid�붯̬��Դ(queue,sem,timer)�Ķ�Ӧ��ϵ */
    std::map<ivr_session_id_t, session_dynamic_resource_t> _sid_dynres_map;
    typedef std::map<ivr_session_id_t, session_dynamic_resource_t>::iterator iterator;

    /** _sid_dynres_map������ */
    //static pthread_mutex_t _mutex;
    static pthread_mutex_t _mutex;

    std::map<std::string, ivr_session_id_t> _uuid_sid_map;
    typedef std::map<std::string, ivr_session_id_t>::iterator uuid_sid_iterator;
    //static pthread_mutex_t _uuid_map_mutex;
    static pthread_mutex_t _uuid_map_mutex;

    // mark ivr flow had(or not) trans agent
    //  if true, flow can't send fs cmd and receive fs event
    std::map<ivr_session_id_t, bool> _transagent_valid;
    typedef std::map<ivr_session_id_t, bool>::iterator tav_iterator;

    std::map<std::string, channel_info_t> _uuid_info_map;
    typedef std::map<std::string, channel_info_t>::iterator cinfo_iterator;

    /**
     * @brief   ����ivr session id
     * @param
     * @return  ivr session id
     */
    static ivr_session_id_t create_ivr_session_id(int32_t type, int32_t freeswith_no);
};


#endif // _IVR_INSTANCE_MANAGER_H_

