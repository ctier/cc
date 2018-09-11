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

#ifndef _NODE_ANSWER_H_
#define _NODE_ANSWER_H_

#include "node_resource.h"

/**
* @brief Ӧ��ڵ�
*   detail description
*
*/
class NodeAnswer : public NodeResource {
public:
    NodeAnswer(const uint32_t id, const string& name,
               const string& type, const string& desc, const key_map_t& keymap);
    virtual ~NodeAnswer(void);

    virtual NodeBase* run(base_script_t* param);

    virtual bool load_other();


private:
    /*
     * @brief ���ݳ�������ȡ���ڽڵ�ָ��
     * @param exit_name ������
     * @param ppnode [out] ���ڽڵ�ָ���ָ��
     *
     * @return true��ʾ�ɹ���ȡ���ڽڵ�ָ��; false��ʾδ�ҵ�
     * @note ������ֵΪfalse,�������Ὣppnode��ָ���ָ���޸�ΪNULL
     */
    bool get_exit_node(const char* exit_name, NodeBase** ppnode);
};

#endif
