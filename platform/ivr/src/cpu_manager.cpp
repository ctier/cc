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

#include "cpu_manager.h"
#include "tools.h"
#include "lock.h"

CpuManager::CpuManager() {
    _cpu_occupy = 0;
    pthread_rwlock_init(&_rwlock, NULL);
}

CpuManager::~CpuManager() {
    pthread_rwlock_destroy(&_rwlock);
}

CpuManager* CpuManager::get_instance() {
    static CpuManager mgr;
    return &mgr;
}

//��ȡcpuռ����
uint32_t CpuManager::get_cpu_occupy() {
    uint32_t cpu_occupy = 0;

    rw_lock_t lock(_rwlock);

    if (lock.locked()) {
        cpu_occupy = _cpu_occupy;
    }

    return cpu_occupy;
}

//��ȡcpu��Ϣ
void CpuManager::get_cpu_info(CPU_INFO* cpust) {
    FILE* fd;
    char buff[256];
    CPU_INFO* cpu_occupy;
    cpu_occupy = cpust;

    if (NULL == (fd = fopen("/proc/stat", "r"))) {
        return ;
    }

    fgets(buff, sizeof(buff), fd);

    sscanf(buff, "%s %u %u %u %u", cpu_occupy->name,
           &cpu_occupy->user, &cpu_occupy->nice, &cpu_occupy->system, &cpu_occupy->idle);
    fclose(fd);
}

//ͨ��cpu��Ϣ���м���
uint32_t CpuManager::calc_cpu_info(CPU_INFO* first, CPU_INFO* second) {
    uint32_t first_total = 0;
    uint32_t second_total = 0;
    uint32_t user = 0;
    uint32_t system = 0;
    uint32_t nice = 0;
    uint32_t cpu_use = 0;

    //��һ��(�û�+���ȼ�+ϵͳ+����)�ܵ�ʱ��
    first_total = (uint32_t)(first->user + first->nice + first->system + first->idle);
    //�ڶ���(�û�+���ȼ�+ϵͳ+����)�ܵ�ʱ��
    second_total = (uint32_t)(second->user + second->nice + second->system + second->idle);

    user = (uint32_t)(second->user - first->user); //�û���һ�κ͵ڶ��ε�ʱ��֮��
    system = (uint32_t)(second->system - first->system); //ϵͳ��һ�κ͵ڶ��ε�ʱ��֮��
    nice = (uint32_t)(second->nice - first->nice); //nice��һ�κ͵ڶ��ε�ʱ��֮��

    if (second_total != first_total) {
        //((�û�+ϵͳ+nice)*100)��(��һ�κ͵ڶ��ε�ʱ���)�ٸ���g_cpu_used
        cpu_use = (uint32_t)((system + user + nice) * 100) / (second_total - first_total);
    } else {
        cpu_use = 0;
    }

    return cpu_use;
}

//����CPUռ�����߳� ��������
void CpuManager::calculate_cpu_occupy_block_func() {
    CPU_INFO cpu_info1;
    CPU_INFO cpu_info2;
    uint32_t cpu_occupy = 0;

    while (!g_stopped) {

        //��һ�λ�ȡcpuʹ�����
        get_cpu_info((CPU_INFO*)&cpu_info1);

        ivr_tools_t::safe_sleeps(1);

        //�ڶ��λ�ȡcpuʹ�����
        get_cpu_info((CPU_INFO*)&cpu_info2);

        //����cpuʹ����
        cpu_occupy = calc_cpu_info((CPU_INFO*)&cpu_info1, (CPU_INFO*)&cpu_info2);

        rw_lock_t lock(_rwlock, true);

        if (lock.locked()) {
            _cpu_occupy = cpu_occupy;
        }

    }

    return;
}
