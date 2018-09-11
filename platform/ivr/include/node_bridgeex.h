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

#ifndef  __NODE_BRIDGEEX_H_
#define  __NODE_BRIDGEEX_H_

#include "node_resource.h"

/**
* @brief ��ʹ������callid�����Žӣ��ŽӰ��������
*   detail description
*
*/
class node_bridgeex_t: public NodeResource {
public:
    /**
     * @brief
     *
     * @param [in/out] id   : const uint32_t
     * @param [in/out] name   : const string&
     * @param [in/out] type   : const string&
     * @param [in/out] desc   : const string&
     * @param [in/out] keymap   : const key_map_t&
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/10/12 11:46:33
    **/
    node_bridgeex_t(const uint32_t id, const string& name,
                    const string& type, const string& desc, const key_map_t& keymap);
    /**
     * @brief
     *
     * @param [in/out] param   : base_script_t*
     * @return  NodeBase*
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/10/12 11:46:40
    **/
    virtual NodeBase* run(base_script_t* param);

    /**
     * @brief
     *
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/10/12 11:46:43
    **/
    virtual bool load_other();
private:
    const static char* PARAM_CALLER;          ///<����
    const static char* PARAM_CALLED;          ///<����
    const static char* PARAM_CALLTYPE;        ///<��������
    const static char* PARAM_BGTYPE;          ///<���б�������:bgm-������;ring-͸��������
    const static char* PARAM_BGMFILE;         ///<�������ļ�
    std::string _caller, _called, _calltype;  ///<������
    std::string _bgtype, _bgmfile;

    const static char* PARAMITEM_BGM;
    const static char* PARAMITEM_RING;
    /**
     * @brief
     *
     * @param [in/out] map<string   : const
     * @param [in/out] vars   : variable_t>&
     * @return  std::string
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/10/12 11:46:54
    **/
    virtual std::string enter(const map<string, variable_t>& vars)const;
};

#endif  //__NODE_BRIDGEEX_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
