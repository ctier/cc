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

#ifndef _CALLDATA_MANAGER_H_
#define _CALLDATA_MANAGER_H_

#include "common.h"

class CallDataManager {
public:
    CallDataManager();
    virtual ~CallDataManager();

public:
    static CallDataManager* get_instance();

    //���������� push��������
    void push_calldata(const char* calldata);

    //����ʱ�ļ� �ƶ��� sendto Ŀ¼
    bool move_generate_sendto(const char* generate_filename);

    //�������������ɵ��ļ� �߳���������
    void calldata_to_file_block_func();

private:
    deque<string> _calldata_deque;
    pthread_mutex_t _mutex;
};

#endif
