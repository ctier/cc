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

#ifndef _IVR_TIMER_H_
#define _IVR_TIMER_H_

#include <stdint.h>
#include <map>
#include <time.h>
#include <pthread.h>
#include "ivr_type.h"
#include "common.h"

typedef uint64_t session_timer_id;      //ʵ���м�ʱ�����־�������ظ�

/**
 * IvrTimer
 * �̷߳ǰ�ȫ��
 */

class IvrTimer {
protected:
    // timer info
    struct timer_index_t {
        session_timer_id id; // timer id
        timer_type_t ttype; // timer type

        bool operator <(const timer_index_t& other) const {
            return this->id < other.id ||
                   (this->id == other.id && this->ttype < other.ttype);
        }
    };
public:
#ifdef DEBUG
    int32_t get_size() {
        return _ivr_timer_task.size();
    }

    int32_t info() {
        timer_task_iterator itr;

        for (itr = _ivr_timer_task.begin(); itr != _ivr_timer_task.end(); ++itr) {
            IVR_WARN("timer id: %lu, clock: %ld, type: %d", itr->first.id, itr->second, itr->first.ttype);
        }

        return 0;
    }
#endif
    /**
     * @brief   ���캯��
     * @param
     * @return
     */
    IvrTimer();

    /**
     * @brief   ���캯��
     * @param
     * @return
     */
    ~IvrTimer();

    /**
     * @brief   ��Ӽ�ʱ����
     * @param   tid ��ʱ��ID
     * @param   time ��ʱ����ʱʱ��
     * @return  �ɹ�����IVR_SUCCESS;
     */
    int32_t add_timer(session_timer_id tid, time_t time, timer_type_t ttype = user_define);

    /**
     * @brief   ɾ����ʱ����
     * @param   tid ��ʱ��ID
     * @return  �ɹ�����IVR_SUCCESS
     */
    int32_t delete_timer(session_timer_id tid, timer_type_t ttype = user_define);

    /**
     * @brief   ��������Ƿ�ʱ
     * @param   sid [in] �������IVR�¼�
     * @return  �ɹ�����IVR_SUCCESS
     */
    int32_t check_timer(ivr_session_id_t sid);

    /**
     * @brief   ��ռ�ʱ����������ʵ��ʱʹ�ã�
     * @param
     * @return  �ɹ�����IVR_SUCCESS
     */
    int32_t clear_all_timer();

private:
    /** ��ʱ���� */
    std::map<timer_index_t, time_t> _ivr_timer_task;
    typedef std::map<timer_index_t, time_t>::iterator timer_task_iterator;
};

#endif // _IVR_TIMER_H_

