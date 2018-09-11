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

#ifndef  __FS_STRUCT_H_
#define  __FS_STRUCT_H_


#include "ims_core.h"


/**
* @brief ���з���
*
*
*/
enum fs_call_direction_t {
    FCD_UNKNOWN,
    FCD_OUTBOUND,         /**< ����       */
    FCD_INBOUND           /**< ����       */
};

/**
* @brief ����״̬
*
*/
enum fs_call_state_t {
    FCS_UNKNOWN,
    FCS_RING,         /**< ����       */
    FCS_EARLY,        /**< ����       */
    FCS_ANSWERED      /**< Ӧ��       */
};

/**
* @brief fw �¼�����
*
*/
enum fs_event_data_t {
    EDT_UNKNOWN,
    EDT_NORMAL,       /**< ��������¼�       */
    EDT_HEARTBEAT     /**< �����¼�           */
};

/**
* @brief ��������¼�
*
*/
typedef struct event_data_normal {
    char uuid[LEN_64 + 1];            /**< FW channel id  */
    char caller_no[LEN_64 + 1];       /**< ���к���       */
    char called_no[LEN_64 + 1];       /**< ���к���       */
    char channel_name[LEN_64 + 1];    /**< channel name   */
    char deviceno[LEN_64 + 1];        /**< �豸����       */

    fs_call_direction_t call_direction; /**< ���з���       */
    fs_call_state_t call_state;         /**< ����״̬       */

    char application[LEN_32 + 1];       /**< �����¼��Ĳ���       */
    char application_data[LEN_64 + 1];  /**< �����¼��Ĳ���       */
    char application_resp[LEN_128 + 1]; /**< �����¼��Ĳ������   */

    char other_uuid[LEN_64 + 1];        /**< FW other channel id      */
    char other_caller_no[LEN_64 + 1];   /**< other channel ���к���   */
    char other_called_no[LEN_64 + 1];   /**< other channel ���к���   */
    char other_channel_name[LEN_64 + 1]; /**< other channel name       */
    char other_deviceno[LEN_64 + 1];    /**< other channel �豸����   */

    char ims_data[LEN_128 + 1];         /**< IMS ��FW���õ����� */

    char reason[LEN_128 + 1];           /**<  �¼�����ԭ��      */
} event_data_normal_t;

/**
* @brief �����¼�
*
*/
typedef struct event_data_heartbeat {
    uint32_t cur_session;         /**< ��ǰ�Ự����     */
    uint32_t all_session;         /**< �ۻ��Ự����     */
    uint32_t cpu_idle;            /**< ��ǰCPU IDLE     */
    volatile uint32_t used_times;               /**< used times between two heartbeats   */
} event_data_heartbeat_t;

/**
* @brief FreeSWITCH �¼�
*   detail description
*
*/
typedef struct fs_event {
    uint64_t timestamp;       /**< ����ʱ��       */
    char name[LEN_64 + 1];    /**< �¼�����       */
    uint64_t sessionid;       /**< eventrecevier��ʱ�����*/
    uint64_t callid;          /**< eventrecevier��ʱ�����*/

    fs_event_data_t datatype; /**< �¼�����       */

    union {
        event_data_normal_t normal;       /**< typeΪ ������ ��Ч      */
        event_data_heartbeat_t heartbeat; /**< type Ϊ heatbeat ��Ч   */
    } event_data;

    fs_event() {
        timestamp = 0;
        name[0] = '\0';
        datatype = EDT_UNKNOWN;
    }

} fs_event_t;


#endif  //__FS_STRUCT_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
