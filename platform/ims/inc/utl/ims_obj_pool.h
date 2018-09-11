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

#ifndef  __IMS_OBJ_POOL_H_
#define  __IMS_OBJ_POOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <new>

#include <bgcc.h>

/**
 * @brief ���̰߳�ȫ���������
 *   ��ʼ����ʱ��Ĭ�Ϲ�������еĶ���
 *   ����ʼ����ʱ���ͷ����ж���
 *   ����Ƶ����new/delete
 *
 */
template< typename T >
class ims_obj_pool {
typedef void CALLBACK(T*);
public:
    ~ims_obj_pool() {
        uninit();
    }
    /**
     * @brief ��ʼ�������
     *
     * @param [in] obj_count   : uint32_t
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2013/01/05 14:22:54
     **/
    bool init(uint32_t obj_count) {
        _inited = false;

        if (!obj_count) {
            return false;
        }

        bgcc::Guard<bgcc::Mutex> lock(&_mutex);

        if (obj_count % 8) {
            _free_count = _count = (obj_count & (~0x7)) + 0x8;
        } else {
            _free_count = _count = obj_count;
        }

        _bit = new(std::nothrow)char[_count];

        if (!_bit) {
            return false;
        }

        memset(_bit, '0', _count);

        _objs = new(std::nothrow) T[_count];

        if (!_objs) {
            delete []_bit;
            return false;
        }

        return (_inited = true);
    }

    /**
     * @brief ����ʼ�������
     *
     * @return  bool
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2013/01/05 14:23:13
     **/
    bool uninit() {
        if (!_inited) {
            return false;
        }

        bgcc::Guard<bgcc::Mutex> lock(&_mutex);

        delete [] _objs;
        delete [] _bit;

        _objs = NULL;
        _bit = NULL;

        return (_inited = false);
    }

    /**
     * @brief ���ڴ�ػ�ȡ����
     *
     * @param [in/out] callback   : CALLBACK*
     * @return  T*
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2013/01/09 16:27:24
    **/
    T* get(CALLBACK* callback = NULL) {
        bgcc::Guard<bgcc::Mutex> lock(&_mutex);

        if (!_inited || !_free_count) {
            return NULL;
        }

        if (!_free_count) {
            return NULL;
        }

        char* ptmp = _bit;

        while (ptmp != (_bit + _count)) {
            if ('0' == *ptmp) {
                *ptmp = '1';
                _free_count--;

                if (callback) {
                    callback(_objs + (ptmp - _bit));
                }

                return _objs + (ptmp - _bit);
            }

            ptmp++;
        }

        return NULL;
    }

    /**
     * @brief �ͷŶ����ڴ��
     *
     * @param [in/out] obj   : T**
     * @param [in/out] callback   : CALLBACK*
     * @return  void
     * @retval
     * @see
     * @note
     * @author chenyuzhen
     * @date 2013/01/09 16:27:29
    **/
    void put(T** obj, CALLBACK* callback = NULL) {
        bgcc::Guard<bgcc::Mutex> lock(&_mutex);

        if (!_inited || _free_count == _count) {
            return;
        }

        T* tmp = *obj;

        if (tmp - _objs < 0 || tmp - _objs >= _count) {
            return;
        }

        *obj = NULL;

        _bit[tmp - _objs] = '0';
        _free_count++;

        if (callback) {
            callback(tmp);
        }
    }

private:
    char*    _bit;                ///<ʹ�ñ�־λ
    T*       _objs;               ///<�����б�
    uint32_t _count;              ///<��������
    uint32_t _free_count;         ///<���ö������

    bool    _inited;              ///<  �Ƿ��Ѿ���ɳ�ʼ������ֹ��γ�ʼ��

    bgcc::Mutex _mutex;         ///<  ����ػ�����

    /**
     * @brief ��ȡ����֮�󣬻��ͷŶ���֮ǰִ�еĻص�����
     *
     */
//    typedef void CALLBACK(T*);

};

#endif  //__IMS_OBJ_POOL_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
