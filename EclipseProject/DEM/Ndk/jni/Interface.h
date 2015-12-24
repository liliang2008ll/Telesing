//
//  Interface.h
//  AE
//
//  Created by 李亮 on 15-11-17.
//  Copyright (c) 2015年 ___LIANG_LEE___. All rights reserved.
//

#ifndef AE_Interface_h
#define AE_Interface_h

#include <stdio.h>

//操作系统类型
//#define IOS_INTERFACE

//!内核打印相关函数从定义
#ifndef IOS_INTERFACE
  #include <android/log.h>
  #include <jni.h> //ndk头文件调用的时候使用
        #define  AE_LOG_VERBOSE   ANDROID_LOG_VERBOSE
        #define  AE_LOG_DEBUG     ANDROID_LOG_DEBUG
        #define  AE_LOG_INFO      ANDROID_LOG_INFO
        #define  AE_LOG_LOG_WARN  ANDROID_LOG_WARN
        #define  AE_LOG_LOG_ERROR ANDROID_LOG_ERROR
#else
        #define  AE_LOG_VERBOSE   stdout
        #define  AE_LOG_DEBUG     stdout
        #define  AE_LOG_INFO      stdout
        #define  AE_LOG_LOG_WARN  stdout
        #define  AE_LOG_LOG_ERROR stderr
#endif

//!数据类型重新定义
#ifndef IOS_INTERFACE //安卓下的数据类型

    typedef  jbyte        AE_INT8;
    typedef  jshort       AE_INT16;
    typedef  jint         AE_INT32;
    typedef  jfloat       AE_FLOAT32;
    typedef  jdouble      AE_FLOAT64;
    typedef  void         AE_VOID;
    typedef  jboolean     AE_UIN8;
    typedef  jchar        AE_UIN16;
    typedef  jlong        AE_INT64;

#else //ios下的数据类型

    typedef  char            AE_INT8;
    typedef  short           AE_INT16;
    typedef  int             AE_INT32;
    typedef  float           AE_FLOAT32;
    typedef  double          AE_FLOAT64;
    typedef  void            AE_VOID;
    typedef  unsigned char   AE_UIN8;
    typedef  unsigned short  AE_UIN16;
    typedef  unsigned int    AE_UIN32;

#endif

#define AE_OK 1
//调试等级
typedef enum{
    DLV0,
    DLV1,
    DLV2,
    DLV3,
    DLV4
}DBUGLV;

//！打印函数重新定义
typedef struct
{
    DBUGLV debuglv;
#ifndef IOS_INTERFACE
    AE_VOID (*fAE_Log)(int prio, const char *tag, const char *fmt, ...);
#else
    AE_INT32 (*fAE_Log)(FILE *restrict, const char*,...);
#endif

}AE_HANDLE,pAE_HANDLE;


//！系统算法函数库
typedef struct{

    //！以下函数为系统调用入口地址
    void   (*start)(void);
    void   (*stop)(void);
    int    (*getStatus)(void);
    int    (*getBufferSize)(void) ;
    void   (*setBufferSize)(int size);
    void   (*clearBuffer)();
    int    (*wirte)(short data[],int data_size);
    double (*fillRate)(void);
    short* (*getRegSignal)(int *len_v);//本来的函数调用中使用的是c++语法中的引用,c语言中改成*

#ifndef IOS_INTERFACE
    int (*dem_init)(
                 JavaVM * gJavaVM_v,jobject  gJavaObj_v,jclass  gJavaClass_v,
                 int n_v, double st_v, int gap_v, double *sFeq_v, double *eFeq_v,int feq_size);
#else
    int    (*dem_init)(int n_v, double st_v, int gap_v, double *sFeq_v, double *eFeq_v,int feq_size);
#endif
    int    (*max)(int x,int y,short buf_p[],int buf_size);

//！以下函数均为系统回调函数入口地址
#ifndef IOS_INTERFACE


#else
    AE_VOID (*OnSynced)(AE_VOID);   //同步到数据后的回调函数
    AE_VOID (*OnsuccRsv)(AE_INT8 *);//解析到非加密数据
    AE_VOID (*OnsuccEnc)(AE_INT8 *);//解析道加密数据
    AE_VOID (*Onfailure)(AE_INT8 *);//解析失败回调函数
#endif

    }ALG_Fxn;

AE_VOID InterfaceInit(AE_HANDLE *AE_handle);
AE_VOID ALG_Init(ALG_Fxn *Algfxn);

#include "./core/dem.h"
#endif
