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

#ifndef  __IMS_TOOLS_H_
#define  __IMS_TOOLS_H_
#include <stdint.h>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <ims_conf.h>
#include "common.h"
#include <vector>
#include <boost/pool/object_pool.hpp>

typedef struct ims_route_info ims_route_info_t;
class ims_tool_t {
public:
    /**
     * @brief
     *
     * @param [in/out] num   : uint64_t
     * @return  string
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/11/21 12:22:11
     **/
    static std::string decimal_to_binary(uint64_t num);
    /**
     * @brief sessionid�Ľṹ | (8bit) fs_id | (40bit) timestamp_ms | (16bit) random_num |
     *
     * @param [in/out] fs_id   : uint32_t
     * @return  uint64_t
     * @retval
     * @see
     * @note
     * @author dangdawei
     * @date 2013/1/21 16:32:24
     **/
    static uint64_t get_sessionid(uint32_t fs_id);
    /**
     * @brief reqid�Ľṹ | (8bit) reqtype | (32bit) reqip | (16bit) redport | (8bit) random_num |
     *
     * @param [in/out] reqtype   : ServiceTypeT
     * @param [in/out] reqid   : uint32_t
     * @param [in/out] reqport   : uint32_t
     * @return  uint64_t
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/11/21 12:22:16
     **/
    static uint64_t get_reqid(const ims::ServiceTypeT& reqtype, uint32_t reqip, uint32_t reqport);
    /**
     * @brief requestid�Ľṹ :
     *           | (8bit) RouteRequestType | (40bit) timestamp_ms | (16bit) random_num of sessionid
     *
     * @param [in] sessionid   : uint64_t
     * @param [in] requestType : RouteRequestType
     * @return  uint64_t
     * @retval
     * @see
     * @note
     * @author dangdawei
     * @date 2013/4/7 18:22:14
     **/
    static ims::RouteRequestIDT get_requestid(const ims::SessionIdT& sessionid,
            const ims::RouteRequestTypeT& requestType);
    /**
     * @brief
     *
     * @return  uint64_t
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/11/21 12:22:20
     **/
    static uint64_t get_timestamp_ms();
    /**
     * @brief
     *
     * @return  uint64_t
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/11/21 12:22:24
     **/
    static uint64_t get_timestamp_s();


    /**
     * @brief ����IP��ַת��Ϊ�ַ���
     *
     * @param [in/out] src   : uint32_t
     * @param [in/out] dest   : char*
     * @param [in/out] dest_len   : uint32_t
     * @return  const char*
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/11/21 10:48:22
     **/
    static const char* inet_ntop(uint32_t src, char* dest, uint32_t dest_len);
    /**
     * @brief �ַ���IP��ַת��Ϊ����
     *
     * @param [in/out] src   : const char*
     * @return  uint32_t
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/11/21 10:48:43
     **/
    static uint32_t inet_pton(const char* src);
    /**
     * @brief ��ȡ����ipv4��ַ����127.0.0.1
     *
     * @param    naddr : uint32_t
     * @return bool
     * @retval
     * @see
     * @note
     * @author  dangdawei
     * @date 2012/7/14 10:52:30
     **/
    static bool is_local_ipv4(uint32_t naddr);

    /**
     * @brief �����ַ���
     *
     * @param [in/out] src   : const char*
     * @param [in/out] pool   : object_pool&
     * @return  const char*
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2011/11/21 10:53:04
     **/
    static const char* strdup(const char* src, boost::object_pool<std::string>* pool);

    static void safe_sleeps(uint32_t s);
    static void safe_sleepms(uint32_t ms);

    /**
     * @brief parse_proxy ip��ַ���� src ��ʽ(-h ip -p port)
     *
     * @param   [in]        src     : std::string   parse_prixy��ַ�ַ���
     * @param   [in/out]    ipstr   : std::string&  ����������ip��ַ�ַ���
     * @param   [in/out]    port    : uint32_t&      ���������Ķ˿ں�
     *
     * @return  ip�Ͷ˿ڸ�ʽ��ȷ����true�����򷵻�false
     * @author wangsilan
     * @date 2011-11-22 20:20:39
     **/
    static bool parse_proxy_info(const std::string& src, std::string& ipstr, uint32_t& port);

    /**
     * @breif ip��ַ�Ϸ����ж�,��ת��Ϊ����
     *
     * @param [in]      src     : const char*
     * @param [in/out]  result  : uint32_t&    IPת��������
     *
     * @rerurn ip��ַ�Ϸ�����true�����򷵻�false
     * @author wangsilan
     * @date 2011-11-22 20:23:54
     **/
    static bool pa_chkip(const char* src, uint32_t& result);

    /**
     * @breif ip��ַ�Ϸ����ж�,��ת��Ϊ����
     *
     * @param [in]      src     : const char*
     *
     * @rerurn  ip��ַ�Ϸ�����true�����򷵻�false
     * @author wangsilan
     * @date 2011-11-22 20:34:08
     **/
    static bool pa_chkip(const char* src);

    /**
     * @breif ͨ�������ļ��õ�ʵ������ͺ������
     *
     * @param [in]      caller_no   : const std::string&    ���к���
     * @param [in]      cadded_no   : const std::string&    ���к���
     * @param [in]      dn          : const std::string&    �����ϯ��IVRͨ����assign�ĺ��룩
     * @param [in]      conf        : const ims_conf_t&     ������
     * @param [in/out]  disp_caller : std::string&          ͨ��������ƥ����disp
     * @param [in/out]  caller      : std::string&          ͨ��������ƥ����caller
     * @param [in/out]  called      : std::string&          ͨ��������ƥ����called
     *
     * @return �ҵ���Ӧroute�������true�����򷵻�false
     * @author wangsilan
     * @date 2011-11-24 14:37:12
     **/
    static bool set_callno(const std::string& caller_no, const std::string& called_no,
                           const std::string& dn, const ims_conf_t& conf,
                           std::string& disp_caller, std::string& caller, std::string& called);

    /**
     * @breif �ַ����ִ��滻�����ַ���dest�������Ӵ�substr_replaced,�滻Ϊ�ַ���src
     *
     * @param [in/out]  dest            : std::string&
     * @param [in]      substr_replaced : std::string&
     * @param [in]      src             : std::string&
     *
     * @return �ַ���dest����substr_replace������true�����򷵻�false
     * @author wangsilan
     * @date 2011-11-24 14:46:49
     *
     **/
    static bool replace_substring(std::string& dest, const std::string& substr_replaced,
                                  const std::string& src);

    /**
     * @breif ����res�滻callno�еı���,����callno�е�$1��res[1]��ֵ�滻
     *
     * @param [in/out]  callno      : std::string&
     * @param [in]      res         : const std::vector<std::string>&
     *
     * @return callno�д���Ҫ�滻�ı�������true�����򷵻�false
     * @author wangsilan
     * date 2011-11-24 14:54:15
     *
     **/
    static bool replace_callno(std::string& callno, const std::vector<std::string>& res);

    /**
     * @breif ���룺xxxx/yyyy/12345@123.12.3123.132:9000�� �����12345
     *
     * @param [in]      src     : const std::string&    xxxx/yyyy/12345@123.12.3123.132:9000
     * @param [in/out]  dest    : std::string&          12345
     *
     * @result ת���Ƿ�ɹ�
     * @author wangsilan
     * @date 2011-11-28 13:08:51
     *
     **/
    static bool chlname2no(const std::string& src, std::string& dest);
    static std::string chlname2no(const std::string& src);

    static bool pa_chkport(const char* src, uint32_t& port);

    static uint32_t get_fsno(uint64_t sessionid);

    static uint32_t str_to_uint32(const char* src);

    static bool ip_mask_parse(const char* src , uint32_t& ip, uint32_t& mask, uint32_t& bit);
    static std::string ims_tool_t::num2str(uint64_t num);

    static void prepare_call_event(ims::CallEventT& call_event,
                                   const uint64_t time,
                                   const ims::SessionIdT sid,
                                   const ims::CallIdT callid,
                                   const std::string& device,
                                   ims::CallStateT state);

    static void prepare_route_event(ims::RouteEventT& ims_event,
                                    ims::SessionIdT sessionid, const ims_route_info_t& info);

    static bool push_cancel_route_event(ims::SessionIdT sessionid);

    static void print_conf_DEBUGLOG(const ims_conf_t& conf);

    /*
     * @breif ���������ļ��õ�ʵ�����к���
     *
     * @param [in] dn :����ϯ��IVR����
     * @param [in] ani : ���к���
     *
     * @return ʵ�����к���
     * @author wangsilan
     * @date 2011-11-30 19:50:21
    **/
    static std::string get_assigned_caller(const std::string& dn, const std::string& ani);

    /*
     * @breif ���������ļ��õ�ʵ�ʱ��к���
     *
     * @param [in] dnis :�����к���
     * @param [in] ani : ���к���
     *
     * @return ʵ�ʱ��к���
     * @author wangsilan
     * @date 2011-11-30 19:51:58
     **/
    static std::string get_assigned_called(const std::string& dnis, const std::string& ani);

    static std::string generate_uuid();

    /**
     * @brief  ip�ǲ��ǿ�����IMS��ע��
     * @param  [in] addr    : ip��ַ
     * @param  [in] type    : ����Ԫ����
     * @return bool
     * @author dangdawei
     * @date   2012/7/4  14:47:30
    **/
    static bool is_accessible_ip(const std::string& addr, ims::ServiceTypeT type);

};














#endif  //__IMS_TOOLS_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
