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

#ifndef APSERVICE_H
#define APSERVICE_H

#include <bgcc.h>
#include "ap.h"
#include "AgentProxy.h"

using namespace ap;

class ApService: public apapi {
public:
    /**
     * @brief InitApService    ��ʼ��AP�������������ӦReLoadConfig��GetAgents�ӿ�
     * @param ApListenIp       IP��ַ
     * @param ApListenPort     �����˿�
     * @param threadPoolNum    ������������
     * @return  �ɹ���true;ʧ�ܣ�false
     * @see
     * @note
     * @author  caozhenxing(caozhenxing@baidu.com)
     * @date    2013��03��26�� 21ʱ23��25��
     */
    bool InitApService(std::string ApListenIp, int32_t ApListenPort, int32_t threadPoolNum);

    /**
     * @brief ReLoadConfig     �����ؼ��ؽӿ�
     * @param ctx              proxyЯ����������Ϣ
     * @return  �ɹ���true;ʧ�ܣ�false
     * @see
     * @note
     * @author  caozhenxing(caozhenxing@baidu.com)
     * @date    2013��03��26�� 21ʱ23��25��
     */
    virtual bool ReLoadConfig(const std::map<std::string, std::string>& ctx);

    /**
     * @brief GetAgents        ��ȡAP�����е���ϯ��Ϣ
     * @param agentInfoList    ��ϯ��Ϣ���������
     * @param ctx              proxyЯ����������Ϣ
     * @return  �ɹ���true;ʧ�ܣ�false
     * @see
     * @note
     * @author  caozhenxing(caozhenxing@baidu.com)
     * @date    2013��03��26�� 21ʱ23��25��
     */
    virtual bool GetAgents(ApAgentInfoListT& agentInfoList,
                           const std::map<std::string, std::string>& ctx);
private:

    int32_t     _threadPoolNum;
    int32_t     _ApListenPort;
    std::string _ApListenIp;
    Server*     _server;

public:
    ApService() {}
    virtual ~ApService();
};

#endif

