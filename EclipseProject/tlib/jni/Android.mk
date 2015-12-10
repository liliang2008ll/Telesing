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
# limitations under the License.
#
# LOCAL_PATH:  这个变量用于给出当前文件的路径。 
#（1）LOCAL_PATH:  这个变量用于给出当前文件的路径。 
#    必须在 Android.mk 的开头定义，可以这样使用：LOCAL_PATH := $(call my-dir) 
#    如当前目录下有个文件夹名称 src，则可以这样写 $(call src)，那么就会得到 src 目录的完整路径 
#     这个变量不会被$(CLEAR_VARS)清除，因此每个 Android.mk 只需要定义一次(即使在一个文件中定义了几个模块的情况下)。 
# （2）LOCAL_MODULE: 这是模块的名字，它必须是唯一的，而且不能包含空格。 
#      必须在包含任一的$(BUILD_XXXX)脚本之前定义它。模块的名字决定了生成文件的名字。 
# （3）LOCAL_SRC_FILES:  这是要编译的源代码文件列表。 
#       只要列出要传递给编译器的文件，因为编译系统自动计算依赖。注意源代码文件名称都是相对于 LOCAL_PATH的，你可以使用路径部分，例如： 
#       LOCAL_SRC_FILES := foo.c toto/bar.c\ 
#       Hello.c 
#     文件之间可以用空格或Tab键进行分割,换行请用"\" 
#      如果是追加源代码文件的话，请用LOCAL_SRC_FILES += 
#      注意：可以LOCAL_SRC_FILES := $(call all-subdir-java-files)这种形式来包含local_path目录下的所有java文件。 
# #（4）LOCAL_C_INCLUDES:  可选变量，表示头文件的搜索路径。 
#        默认的头文件的搜索路径是LOCAL_PATH目录。 
#（5）LOCAL_STATIC_LIBRARIES: 表示该模块需要使用哪些静态库，以便在编译时进行链接。 
# （6）LOCAL_SHARED_LIBRARIES:  表示模块在运行时要依赖的共享库（动态库），在链接时就需要，以便在生成文件时嵌入其相应的信息。 
#      注意：它不会附加列出的模块到编译图，也就是仍然需要在Application.mk 中把它们添加到程序要求的模块中。 
#  （7）LOCAL_LDLIBS:  编译模块时要使用的附加的链接器选项。这对于使用‘-l’前缀传递指定库的名字是有用的。 
#      例如，LOCAL_LDLIBS := -lz表示告诉链接器生成的模块要在加载时刻链接到/system/lib/libz.so 
#       可查看 docs/STABLE-APIS.TXT 获取使用 NDK发行版能链接到的开放的系统库列表。 
#  (8)LOCAL_MODULE_PATH 和 LOCAL_UNSTRIPPED_PATH 
#      在 Android.mk 文件中， 还可以用LOCAL_MODULE_PATH 和LOCAL_UNSTRIPPED_PATH指定最后的目标安装路径. 
#      不同的文件系统路径用以下的宏进行选择： 
#       TARGET_ROOT_OUT：表示根文件系统。 
#      TARGET_OUT：表示 system文件系统。 
#      TARGET_OUT_DATA：表示 data文件系统。 
#      用法如：LOCAL_MODULE_PATH :=$(TARGET_ROOT_OUT) 
#    至于LOCAL_MODULE_PATH 和LOCAL_UNSTRIPPED_PATH的区别，暂时还不清楚。 
# （9）LOCAL_JNI_SHARED_LIBRARIES：定义了要包含的so库文件的名字，如果程序没有采用jni，不需要 
#      LOCAL_JNI_SHARED_LIBRARIES := libxxx 这样在编译的时候，NDK自动会把这个libxxx打包进apk； 放在你的apk/lib/目录下 
#编译静态库  
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
  
#编译动态库  
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
  
#使用静态库  
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
  
#使用动态库  
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

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
#库文件名称
LOCAL_MODULE    :=    tlib
LOCAL_SRC_FILES :=   tlib.cpp
LOCAL_LDLIBS    :=  -L$(SYSROOT)/usr/lib -llog
include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)  

