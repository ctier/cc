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

#include "calldata_manager.h"
#include "lock.h"
#include "tools.h"

extern call_data_conf_t g_calldata_conf;

CallDataManager::CallDataManager() {
    pthread_mutex_init(&_mutex, NULL);
}

CallDataManager::~CallDataManager() {
    pthread_mutex_destroy(&_mutex);
}

CallDataManager* CallDataManager::get_instance() {
    ///<gccĬ�ϵ�threadsafe-statics��֤�̰߳�ȫ
    static CallDataManager mgr;
    return &mgr;
}

//���������� push��������
void CallDataManager::push_calldata(const char* calldata) {
    ::lock_t lock(_mutex);

    if (lock.locked()) {
        _calldata_deque.push_back(calldata);
    } else {
        IVR_TRACE("����ʧ��,�������ʧ��[%s]", calldata);
    }
}

//����ʱ�ļ� �ƶ��� sendto Ŀ¼
bool CallDataManager::move_generate_sendto(const char* generate_filename) {
    time_t rawtime;
    time(&rawtime);

    char buffer[25];
    bzero(buffer, 25);
    struct tm* timeinfo = localtime(&rawtime);

    strftime(buffer, 25, "%Y-%m-%d-%H.%M.%S", timeinfo);

    string sendto_filename = g_calldata_conf.sendto_path + buffer + g_calldata_conf.file_name;

    if (ivr_tools_t::check_and_create_dir(g_calldata_conf.sendto_path.c_str())) {
        ostringstream ostm;
        ostm << "mv " << generate_filename << " " << sendto_filename;

        if (system(ostm.str().c_str()) == 0) {
            return true;
        }
    } else {
        IVR_WARN("mkdir %s failed", g_calldata_conf.sendto_path.c_str());
    }

    return false;
}

//���������� ���ɵ��ļ�
void CallDataManager::calldata_to_file_block_func() {
    string calldata;
    string generate_filename = g_calldata_conf.generate_path + g_calldata_conf.file_name;

    ofstream fout;
    struct stat st;

    time_t begin_tm;
    time(&begin_tm);

    while (!g_stopped) {
        if (stat(generate_filename.c_str(), &st) == 0) {
            time_t now_tm;
            time(&now_tm);

            //��ǰʱ��-�ļ���ʼʱ�� ����������ֵ ���ƶ��ļ�
            if ((now_tm - begin_tm) > g_calldata_conf.generate_interval) {
                IVR_TRACE("��ǰʱ��-�ļ���ʼʱ�� ����������ֵ ���ƶ��ļ�");

                if (move_generate_sendto(generate_filename.c_str())) {
                    begin_tm = now_tm;
                }
            } else if (st.st_size > (g_calldata_conf.generate_file_size * 1024)) {
                //�ļ���С ����������ֵ ���ƶ��ļ�
                IVR_TRACE("�ļ���С ����������ֵ ���ƶ��ļ�");

                if (move_generate_sendto(generate_filename.c_str())) {
                    begin_tm = now_tm;
                }
            }
        }

        if (!ivr_tools_t::check_and_create_dir(g_calldata_conf.generate_path.c_str())) {
            IVR_WARN("mkdir %s failed", g_calldata_conf.generate_path.c_str());
            ivr_tools_t::safe_sleeps(2);
            continue;
        }

        fout.open(generate_filename.c_str(), ofstream::app | ofstream::out);

        if (fout.good()) {
            deque<string> to_write_queue;

            {
                ::lock_t lock(_mutex);

                if (lock.locked()) {
                    to_write_queue.assign(_calldata_deque.begin(), _calldata_deque.end());
                    _calldata_deque.clear();
                }
            }

            if (to_write_queue.size()) {
                for (deque<string>::iterator it = to_write_queue.begin();
                        it != to_write_queue.end(); ++it) {

                    fout << *it << "\n";
                }

            }
        } else {
            IVR_WARN("�򿪻����ļ�%sʧ��", generate_filename.c_str());
        }

        if (fout.is_open()) {
            fout << flush;
            fout.close();
        }

        ivr_tools_t::safe_sleeps(2);
    }

    return;
}
