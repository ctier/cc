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

#include "node_bridge.h"
#include "fs_opr.h"
#include "fs_mgr.h"

NodeBridge::NodeBridge(const uint32_t id, const string& name,
                       const string& type, const string& desc, const key_map_t& keymap)
    : NodeResource(id, name, type, desc, keymap) {
}

NodeBridge::~NodeBridge(void) {
}

NodeBase* NodeBridge::run(base_script_t* param) {
    if (NULL == param) {
        IVR_WARN("base_script_t pointer should not be null");
        return NULL;
    }

    const char* exit = EXIT_FAIL;
    IVR_TRACE("%s", enter(param->name_var_map).c_str());

    string callid1;
    string callid2;
    fw_id_t fs_no;

    fs_no = param->fid;

    if (true == parse_expression(_callid1, param->name_var_map, callid1)
            && true == parse_expression(_callid2, param->name_var_map, callid2)) {
        IVR_TRACE("callid1=%s,callid2=%s", callid1.c_str(), callid2.c_str());

        fs_opr_t* opr = NULL;

        if (fs_mgr_t::instance()->fetch_opr(fs_no, &opr) == IVR_SUCCESS) {
            if (NULL != opr) {
                //TODO: ȷ�ϵ��ýӿ��Ƿ���bridgeonly
                if (IVR_SUCCESS == opr->bridgeonly(callid1.c_str(), callid2.c_str())) {
                    *(string*)param->name_var_map[SYS_VAR[sys_var_t::CALLID]].pvalue = callid2;
                    exit = EXIT_SUCC;
                } else {
                    IVR_WARN("bridge failed(callid1: %s, callid2: %s)", _callid1.c_str(), _callid2.c_str());
                }

                fs_mgr_t::instance()->free_opr(&opr);
            } else {
                IVR_WARN("fetch opr failed. freeswitch no is %d", fs_no);
            }
        } else {
            IVR_WARN("fetch opr failed. freeswitch no is %d", fs_no);
        }
    } else {
        IVR_WARN("callid1 or callid2 parse failed(%s,%s)", _callid1.c_str(), _callid2.c_str());
    }


    NodeBase* exit_node_ptr = NULL;
    std::map<std::string, NodeBase*>::iterator citr;
    citr = _exit_node_map.find(exit);

    if (citr != _exit_node_map.end()) {
        exit_node_ptr = citr->second;

        IVR_TRACE("%s exit from %s(bridge %s and %s)",
                  leave(param->name_var_map).c_str(), exit, callid1.c_str(), callid2.c_str());
    } else {
        IVR_WARN("Can not find exit %s in _exit_node_map", exit);
    }

    return exit_node_ptr;
}

const char* NodeBridge::PARAM_CALLID2 = "callid2";
const char* NodeBridge::PARAM_CALLID1 = "callid1";

bool NodeBridge::load_other() {
    return NodeBase::load_other()
           && valid_str(_key_map, PARAM_CALLID1, _callid1)
           && valid_str(_key_map, PARAM_CALLID2, _callid2);
}

std::string NodeBridge::enter(const map<string, variable_t>& vars)const {
    ostringstream ostm;
    ostm << NodeBase::enter(vars);
    ostm << "callid2:" << _callid2;
    return ostm.str();
}
