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

#ifndef  __IMS_CONF_LOADER_H_
#define  __IMS_CONF_LOADER_H_

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "ims_conf.h"
#include "ims_log.h"

class runtime_pool_t;
/**
* @brief IMS ���ü�����
*
*/
class ims_conf_loader_t {
public:
    /**
    * @brief ȫ��Ψһ
    *
    */
    static ims_conf_loader_t* instance() {
        static ims_conf_loader_t loader;
        return &loader;
    }

    /**
     * @brief
     *
     * @param [in/out] pool   : ResourcePool&
     * @param [in/out] conf   : ims_conf_t&
     * @param [in/out] isreload   : bool �Ƿ������¼���
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/11/21 11:18:18
    **/
    bool load_conf(runtime_pool_t& pool, ims_conf_t& conf, bool isreload = false);
    \

private:
    ims_conf_loader_t() {}
    /**
    * @brief ���ط��� ��ء�������
    *
    */
    bool load_service(xmlNodePtr cur_node, runtime_pool_t& pool, ims_conf_t& conf);
    /**
    * @brief ����������Ŀ
    *
    */
    bool load_param(xmlNodePtr cur_node, runtime_pool_t& pool, ims_conf_t& conf);
    /**
    * @brief �Ϸ�client
    *
    */
    bool load_client(xmlNodePtr cur_node, runtime_pool_t& pool, ims_conf_t& conf);
    /**
    * @brief ��������(FreeSWITCH)
    *
    */
    bool load_gateway(xmlNodePtr cur_node, runtime_pool_t& pool, ims_conf_t& conf);
    /**
    * @brief ·�ɹ���
    *
    */
    bool load_route(xmlNodePtr cur_node, runtime_pool_t& pool, ims_conf_t& conf);

};













#endif  //__IMS_CONF_LOADER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
