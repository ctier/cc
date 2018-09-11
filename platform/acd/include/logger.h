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


#pragma once

#include <string>           //��׼C++�⣬string��
#include <fstream>          //��׼C++�⣬IO����
#include <vector>           //��׼C++�⣬vector
#include <list>             //��׼C++�⣬list
#include <cstdio>           //��׼C++�⣬remove
#include <cstring>          //��׼C++�⣬strcmp
#include <cassert>
#include "const_define.h"

using namespace std;

class Logger {
private:
    uint32_t mLogCount;                 //��־�ļ���С
    uint32_t mFileCount;                    //��־�ļ�����
    string mDirName;                            //��־�ļ�·��
    string mFileName;                           //��־�ļ���
    string mVersion;                            //����汾
    bool mShowVersion;                          //�Ƿ���ʾ�汾
    Level mLevel;                               //��־����

    uint32_t mCurLogCount;                  //��ǰ��־����
    string mFuleFileName;                       //��־�ļ�ȫ��
    ofstream mOutStream;                        //IO��
    bgcc::Mutex mLocker;                        //ͬ����
private:
    bool CreatDirectory(const string& strPath);
    void GetTimeFileName(LongString strTime);
    void GetCurrentTimeString(LongString strTime);
    void InitLogger();
    void DeleteLog();
    void UnInitLogger();
    void ProcessLog();
    bool LogFileNotExisted();
    void GetLevelString(Level level, string& levelString);
public:
    Logger();
    ~Logger();
    void Initialize(uint32_t logCount, uint32_t fileCount, const char* dirName, const char* fileName,
                    const char* version, bool showVersion = true, Level level = LOG_LEVEL_NOTICE);
    void UnInitialize();
    void WriteLog(const char* pFormat, ...);
    void WriteLog(Level level, const char* pFormat, ...);
    void WriteLog(Level level, const char* file, uint32_t line, const char* fun, const char* pFormat,
                  ...);
    void SetTimeout();
    void SetLogLevel(Level level = LOG_LEVEL_NOTICE);
};
