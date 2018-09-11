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

#include <string>			//��׼C++�⣬string��
#include <fstream>			//��׼C++�⣬IO����
#include <vector>			//��׼C++�⣬vector
#include <list>				//��׼C++�⣬list
#include <cstdio>			//��׼C++�⣬remove
#include <cstring>			//��׼C++�⣬strcmp
#include <io.h>				//C�⣬access����
#include <cassert>
#include "ConstDef.h"

using namespace std;

class Logger
{
private:
	unsigned long mLogCount;					//��־�ļ���С
	unsigned long mFileCount;					//��־�ļ�����
	string mDirName;							//��־�ļ�·��
	string mFileName;							//��־�ļ���
	string mVersion;							//����汾
	bool mShowVersion;							//�Ƿ���ʾ�汾
	Level mLevel;								//��־����

	unsigned long mCurLogCount;					//��ǰ��־����
	string mFuleFileName;						//��־�ļ�ȫ��
	ofstream mOutStream;						//IO��
	bgcc::Mutex mLocker;								//ͬ����
private:
	bool CreatDirectory(const string &strPath);
	void GetTimeFileName(LongString strTime);
	void GetCurrentTimeString(LongString strTime);
	void InitLogger();
	void DeleteLog();
	void UnInitLogger();
	void ProcessLog();
public:
	Logger();
	~Logger();
	void Initialize(unsigned long logCount, unsigned long fileCount, const char *dirName, const char *fileName, const char *version, bool showVersion = true, Level level = Level_High);
	void UnInitialize();
	void WriteLog(Level level, const char *pFormat, ...);
	void WriteLog(const char *content, Level level = Level_Low);
	void WriteLog(const char *owner, const char *method, const char *content, int result, Level level = Level_Low);
	void WriteLog(const char *owner, const char *method, int content, int result, Level level = Level_Low);
	void SetLogLevel(Level level = Level_High);
	const string &GetFullFileName();
};
