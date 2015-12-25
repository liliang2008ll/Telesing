# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limi tations under the License.    
#    
# LOCAL_PATH:  ����������ڸ�����ǰ�ļ���·���� 
#��1��LOCAL_PATH:  ����������ڸ�����ǰ�ļ���·���� 
#    ������ Android.mk �Ŀ�ͷ���壬��������ʹ�ã�LOCAL_PATH := $(call my-dir) 
#    �統ǰĿ¼���и��ļ������� src�����������д $(call src)����ô�ͻ�õ� src Ŀ¼������·�� 
#     ����������ᱻ$(CLEAR_VARS)��������ÿ�� Android.mk ֻ��Ҫ����һ��(��ʹ��һ���ļ��ж����˼���ģ��������)�� 
# ��2��LOCAL_MODULE: ����ģ������֣���������Ψһ�ģ����Ҳ��ܰ����ո� 
#      �����ڰ�����һ��$(BUILD_XXXX)�ű�֮ǰ��������ģ������־����������ļ������֡� 
# ��3��LOCAL_SRC_FILES:  ����Ҫ�����Դ�����ļ��б��� 
#       ֻҪ�г�Ҫ���ݸ����������ļ�����Ϊ����ϵͳ�Զ�����������ע��Դ�����ļ����ƶ�������� LOCAL_PATH�ģ������ʹ��·�����֣����磺 
#       LOCAL_SRC_FILES := foo.c toto/bar.c\ 
#       Hello.c 
#     �ļ�֮������ÿո��Tab�����зָ�,��������"\" 
#      �����׷��Դ�����ļ��Ļ�������LOCAL_SRC_FILES += 
#      ע�⣺����LOCAL_SRC_FILES := $(call all-subdir-java-files)������ʽ������local_pathĿ¼�µ�����java�ļ��� 
# #��4��LOCAL_C_INCLUDES:  ��ѡ��������ʾͷ�ļ�������·���� 
#        Ĭ�ϵ�ͷ�ļ�������·����LOCAL_PATHĿ¼�� 
#��5��LOCAL_STATIC_LIBRARIES: ��ʾ��ģ����Ҫʹ����Щ��̬�⣬�Ա��ڱ���ʱ�������ӡ� 
# ��6��LOCAL_SHARED_LIBRARIES:  ��ʾģ��������ʱҪ�����Ĺ����⣨��̬�⣩��������ʱ����Ҫ���Ա��������ļ�ʱǶ������Ӧ����Ϣ�� 
#      ע�⣺�����ḽ���г���ģ�鵽����ͼ��Ҳ������Ȼ��Ҫ��Application.mk �а��������ӵ�����Ҫ���ģ���С� 
#  ��7��LOCAL_LDLIBS:  ����ģ��ʱҪʹ�õĸ��ӵ�������ѡ������ʹ�á�-l��ǰ׺����ָ��������������õġ� 
#      ���磬LOCAL_LDLIBS := -lz��ʾ�������������ɵ�ģ��Ҫ��  ����ʱ�����ӵ�/system/lib/libz.so 
#       �ɲ鿴 docs/STABLE-APIS.TXT ��ȡʹ�� NDK���а������ӵ��Ŀ��ŵ�ϵͳ���б��� 
#  (8)LOCAL_MODULE_PATH �� LOCAL_UNSTRIPPED_PATH 
#      �� Android.mk �ļ��У� ��������LOCAL_MODULE_PATH ��LOCAL_UNSTRIPPED_PATHָ������Ŀ�갲װ·��. 
#      ��ͬ���ļ�ϵͳ·�������µĺ����ѡ�� 
#       TARGET_ROOT_OUT����ʾ���ļ�ϵͳ�� 
#      TARGET_OUT����ʾ system�ļ�ϵͳ�� 
#      TARGET_OUT_DATA����ʾ data�ļ�ϵͳ��             
#      �÷��磺LOCAL_MODULE_PATH :=$(TARGET_ROOT_OUT) 
#    ����LOCAL_MODULE_PATH ��LOCAL_UNSTRIPPED_PATH��������ʱ��������� 
# ��9��LOCAL_JNI_SHARED_LIBRARIES��������Ҫ������so���ļ������֣��������û�в���jni������Ҫ 
#      LOCAL_JNI_SHARED_LIBRARIES := libxxx �����ڱ����ʱ��NDK�Զ�������libxxx�����apk�� �������apk/lib/Ŀ¼�� 
#���뾲̬��                         
#LOCAL_PATH := $(call my-dir)  
#include $(CLEAR_VARS)  
#LOCAL_MODULE = libhellos  
#LOCAL_CFLAGS = $(L_CFLAGS)  
#LOCAL_SRC_FILES = hellos.c  
#LOCAL_C_INCLUDES = $(INCLUDES)  
#LOCAL_SHARED_LIBRARIES := libcutils  
#LOCAL_COPY_HEADERS_TO := libhellos  
#LOCAL_COPY_HEADERS := hellos.h  
#include $(BUILD_STATIC_LIBRARY)  
  
#���붯̬��  
#LOCAL_PATH := $(call my-dir)  
#include $(CLEAR_VARS)  
#LOCAL_MODULE = libhellod  
#LOCAL_CFLAGS = $(L_CFLAGS)  
#LOCAL_SRC_FILES = hellod.c  
#LOCAL_C_INCLUDES = $(INCLUDES)  
#LOCAL_SHARED_LIBRARIES := libcutils  
#LOCAL_COPY_HEADERS_TO := libhellod  
#LOCAL_COPY_HEADERS := hellod.h  
#include $(BUILD_SHARED_LIBRARY)  
                 
#ʹ�þ�̬��  
#LOCAL_PATH := $(call my-dir)  
#include $(CLEAR_VARS)  
#LOCAL_MODULE := hellos  
#LOCAL_STATIC_LIBRARIES := libhellos  
#LOCAL_SHARED_LIBRARIES :=  
#LOCAL_LDLIBS += -ldl  
#LOCAL_CFLAGS := $(L_CFLAGS)  
#LOCAL_SRC_FILES := mains.c  
#LOCAL_C_INCLUDES := $(INCLUDES)  
#include $(BUILD_EXECUTABLE)  
  
#ʹ�ö�̬��  
#LOCAL_PATH := $(call my-dir)  
#include $(CLEAR_VARS)  
#LOCAL_MODULE := hellod  
#LOCAL_MODULE_TAGS := debug  
#LOCAL_SHARED_LIBRARIES := libc libcutils libhellod  
#LOCAL_LDLIBS += -ldl  
#LOCAL_CFLAGS := $(L_CFLAGS)  
#LOCAL_SRC_FILES := maind.c  
#LOCAL_C_INCLUDES := $(INCLUDES)  
#include $(BUILD_EXECUTABLE)  


#include $(CLEAR_VARS)
#
##�ⲿģ����
#LOCAL_MODULE:= libcrypto 
##ģ������ �ⲿģ��
#LOCAL_SRC_FILES:=C://Users/liang/Desktop/asc_a/jni/libcrypto.so
##ͷ�ļ�·��
#LOCAL_EXPORT_C_INCLUDES :=C://Users/liang/Desktop/asc_a/jni/core/openssl
##/���Ӷ�̬��so����libcrypto��������LOCAL_MODULE ָ�������ƣ����һ��Զ����Ƶ���so���Ŀ¼
#LOCAL_SHARED_LIBRARIES +=libcrypto ;
#include $(PREBUILT_SHARED_LIBRARY) 
#
#include $(CLEAR_VARS)
##�ⲿģ����
#LOCAL_MODULE:= libss
##ģ������---->�ⲿģ��
#LOCAL_SRC_FILES:=C://Users/liang/Desktop/asc_a/jni/libssl.so
##ͷ�ļ�·��
#LOCAL_EXPORT_C_INCLUDES :=C://Users/liang/Desktop/asc_a/jni/core/openssl
#
##/���Ӷ�̬��so����libcrypto��������LOCAL_MODULE ָ�������ƣ����һ��Զ����Ƶ���so���Ŀ¼
#LOCAL_SHARED_LIBRARIES += libssl;
#
#include $(PREBUILT_SHARED_LIBRARY) 
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)


#��ģ������-------> ����ָ����ģ��
LOCAL_MODULE    :=    tlib
LOCAL_SRC_FILES :=   tlib.cpp\
					.\core\core.cpp\
					.\core\dem.cpp\
					.\core\bch.cpp\
					.\core\buffer_c.cpp\
					.\core\corr_helper.cpp\
					.\core\fft/Complexs.cpp \
					.\core\fft/IdxGen.cpp \
					.\core\fft/Expression.cpp \
					.\core\fft/ExpGen.cpp \
	  				.\core\fft/Transformer.cpp \
	 				.\core\big_int.cpp \
					.\core\rsa_use.cpp \
					.\core\mod.cpp 
	
		
 	 						
LOCAL_LDLIBS    :=  -L$(SYSROOT)/usr/lib -llog  

##���Ӷ�̬��so����libcrypto_dynamic��������LOCAL_MODULE ָ�������ƣ����һ��Զ����Ƶ���so���Ŀ¼



#LOCAL_SHARED_LIBRARIES +=libcrypto
#LOCAL_SHARED_LIBRARIES +=libss


include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)  

    












