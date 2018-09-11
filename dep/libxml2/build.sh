#!/bin/bash

# Copyright 2002-2014 the original author or authors.

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      CC/LICENSE
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


#libxml2��Դ��tar���ļ���
SRCTAR=libxml2-2.6.30.tar.gz

if [ ! -f ${SRCTAR} ]; then
    wget http://xmlsoft.org/sources/old/libxml2-2.6.30.tar.gz
fi

if [ ! -f ${SRCTAR} ]; then
    echo "can't find ${SRCTAR}, build failed";
    exit 1;
fi

THIRDSRC=$PWD
SRC=libxml2-2.6.30
INSTALL=$PWD/install

#��ѹ
cd $THIRDSRC
rm -rf $SRC
tar xzf $SRCTAR
	
#����Ŀ¼���б��밲װ
cd $SRC
#����configure����
#��װ��$INSTALL, ����������������������������, 
./configure --prefix=$INSTALL --with-pic=yes --with-threads=yes
#���벢��װ
make; make install

cd $INSTALL/..

#������������,�������Ҫ�����Ľ���ŵ���ǰĿ¼�µ�output�м���
rm -rf output
mkdir output
mkdir output/include
mkdir output/lib
cp -r $INSTALL/include/libxml2/* output/include/
cp -r $SRC/libxml.h $SRC/config.h output/include
cp $INSTALL/lib/*.a output/lib
                                 


