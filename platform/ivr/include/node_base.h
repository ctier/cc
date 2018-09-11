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

#ifndef _NODE_BASE_H_
#define _NODE_BASE_H_

#include "common.h"
#include "fs_operator.h"
#include "tools.h"
struct base_script_t;

class NodeBase {
public:
    NodeBase(const uint32_t id, const string& name,
             const string& type, const string& desc, const key_map_t& keymap);
    virtual ~NodeBase(void);

    virtual NodeBase* run(base_script_t* param) = 0;

    virtual bool load_other();

public:

    /**
     * @brief �ж�map���Ƿ�����Ҫ�ı���,����У���ֵ��out
     *
     * @param [in/out] vars   : key_map_t&
     * @param [in/out] key   : const char*
     * @param [in/out] out   : std::string&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/09/06 14:17:04
     **/
    bool valid_str(key_map_t& vars, const char* key, std::string& out, bool trim = true);

    /**
     * @brief
     *
     * @param [in/out] vars   : key_map_t&
     * @param [in/out] key   : const char*
     * @param [in/out] out   : int32_t&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/09/06 15:55:47
     **/
    bool NodeBase::valid_str(key_map_t& vars, const char* key, uint32_t& out);

    static bool get_sys_var(const map<string, variable_t>& vars, uint32_t type,
                            std::string& out) {
        bool result = false;

        if (type >= _sys_var_t::END) {
            return false;
        }

        map<string, variable_t>::const_iterator it = vars.find(SYS_VAR[type]);

        if (it != vars.end() && NULL != it->second.pvalue) {
            out = *(string*)(it->second.pvalue);
            result = true;
        }

        return result;
    }
    /**
     * @brief ����ڵ�
     *
     * @param [in/out] map<string   : const
     * @param [in/out] vars   : variable_t>&
     * @return  const char*
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/09/01 14:31:23
     **/
    virtual std::string enter(const map<string, variable_t>& vars)const ;
    /**
     * @brief �뿪�ڵ�
     *
     * @param [in/out] map<string   : const
     * @param [in/out] vars   : variable_t>&
     * @return  const char*
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/09/01 14:31:34
     **/
    virtual std::string leave(const map<string, variable_t>& vars)const ;

    /**
     * @brief ��װ��key_map�Ĳ��Һ���
     *
     * @param [in/out] key   : string&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/09/02 10:54:36
     **/
    bool key_map_find(string& key);

    /**
     * @brief  ��װ�˽ڵ��¼����ڵļ��
     *
     * @param [in/out] exit   : string
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/09/02 10:54:52
     **/
    bool check_exit(string exit);

    /**
     * @brief ϵͳ����
     *
     * @param [in/out] system   : string&
     * @return  string
     * @retval
     * @see
     * @note Ŀǰ֧�ֵ�ϵͳ���ã�
     *          _DATE����ǰ����
     *          _TIME����ǰʱ��
     *          _SECOND��1970-1-1 08:00:00 ֮�󾭹�������
     *          _INBOUND_NUM������ռ�õ�ͨ����
     *          _AUTORUN_NUM�����ռ�õ�ͨ����
     * @author chenyuzhen
     * @date 2011/09/02 10:38:09
     **/
    bool system_invoke(const std::string& system, std::string& out);

    /**
     * @brief �ַ�������
     *
     * @param [in/out] func   : string&
     * @param [in/out] param   : string&
     * @return  bool �������ҵ��˺������Һ���ִ��ʧ�ܵ�ʱ�򷢻�false
     *               ��������·���true
     * @retval
     * @see
     * @note Ŀǰ֧�ֵ��ַ��������У�
     *          LENGTH����ȡ�ַ����ĳ��� LENGTH(str)
     *          LEFT����ȡ�ַ������n���ַ�LEFT(str,n)
     *          RIGHT����ȡ�ַ����ұ�n���ַ�RIGHT(str,n)
     * @author chenyuzhen
     * @date 2011/09/02 10:48:04
     **/
    bool str_func_invoke(const string& func, const string& param, std::string& out);


    bool db_func_invoke(const string& func, const string& name, uint32_t param,
                        const map<string, variable_t>& name_var_map, std::string& out);

    bool map_func_invoke(const string& func, const string& name, const string& param,
                         const map<string, variable_t>& name_var_map, std::string& out);
    /**
     * @brief �������ʽ��ת��Ϊϵͳ�����ĵ���
     *
     * @param [in] exp   : string&
     * @param [in] name_var_map   : map<string,variable_t>&
     * @return  string&
     * @retval
     * @see system_invoke
     * @note
     * @author chenyuzhen
     * @date 2011/09/02 10:48:04
     **/
    bool parse_sys_invoke(const string& exp, const map<string, variable_t>& name_var_map,
                          string& out);

    /**
     * @brief �������̼���ı��ʽ���滻Ϊ���̼���ı���ֵ
     *
     * @param [in] exp   : string&
     * @param [in] name_var_map   : map<string,variable_t>&
     * @return  string&
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/09/02 10:48:04
     **/
    bool parse_variable(const string& exp_, const map<string, variable_t>& name_var_map,
                        string& out);

    /**
     * @brief �����������ã�Ȼ�������غ��������ع���
     *
     * @param [in] exp   : string&
     * @param [in] name_var_map   : map<string,variable_t>&
     * @return  string&
     * @retval
     * @see str_func_invoke,db_func_invoke,map_func_invoke
     * @note
     * @author chenyuzhen
     * @date 2011/09/02 10:48:04
     **/
    bool parse_function(const string& exp_, const map<string, variable_t>& name_var_map,
                        string& out);

    /**
     * @brief �������ʽ��ͳһ���
     *
     * @param [in] exp   : string&
     * @param [in] name_var_map   : map<string,variable_t>&
     * @return  string&
     * @retval
     * @see parse_function,parse_variable,parse_sys_invoke
     * @note
     * @author chenyuzhen
     * @date 2011/09/02 10:48:04
     **/
    bool parse_expression(const string exp, const map<string, variable_t>& name_var_map,
                          string& out);

    /**
     * @brief ���б��ʽ����ת��
     *
     * @param [in/out] exp   : const std::string
     * @param [in/out] map<string   : const
     * @param [in/out] name_var_map   : variable_t>&
     * @param [in/out] out   : string&
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/09/21 12:35:35
    **/
    bool parse_all(const std::string exp, const map<string, variable_t>& name_var_map,
                   string& out);

public:
    uint32_t      _id;
    string        _name;
    string        _desc;
    string        _type;
    key_map_t     _key_map;

    map<string, uint32_t>  _exit_id_map;     //�¼����ڵ��ڵ�ID��ӳ��
    map<string, NodeBase*> _exit_node_map;   //�¼����ڵ��ڵ�����ӳ��

    vector<string> _system_invoke_vec;
    vector<string> _function_vec;

protected:
    const static char* EXIT_SUCC;         ///<
    const static char* EXIT_FAIL;         ///<
    const static char* SYS_DATE;          ///<
    const static char* SYS_TIME;          ///<
    const static char* SYS_SEC;           ///<
    const static char* SYS_INBOUND_NUM;           ///<
    const static char* SYS_OUTBOUND_NUM;          ///<
    const static char* SYS_ANSWER_TIME;           ///<
    const static char* SYS_HANGUP_CAUSE;          ///<

    const static char* FUNC_STR_LEN;          ///<
    const static char* FUNC_STR_LEFT;         ///<
    const static char* FUNC_STR_RIGHT;        ///<
    const static char* FUNC_STR_FIND;         ///<
    const static char* FUNC_STR_SUB;          ///<
    const static char* FUNC_STR_REPLACE;      ///<
    const static char* FUNC_INT_RAND;         ///<

    const static char* FUNC_DB_NEXT;          ///<
    const static char* FUNC_DB_VAL;           ///<

    const static char* FUNC_MAP_VAL;          ///<

    const static char* HANGUP_CAUSE_SYS;
    const static char* HANGUP_CAUSE_USER;
    const static char* HANGUP_CAUSE_THIRD;
};

#endif
