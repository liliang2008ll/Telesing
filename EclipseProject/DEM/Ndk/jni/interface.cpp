//
//  interface.c
//  AE
//
//  Created by 李亮 on 15-11-17.
//  Copyright (c) 2015年 ___LIANG_LEE___. All rights reserved.
//

#include <stdio.h>
#include "Interface.h"
#include ".\core\dem.h"//算法接口函数在里面

#ifndef IOS_INTERFACE

#endif
//！接口函数初始化
//! 初始化系统调用的打印函数
//
//
//
//！不接受输入参数,默认输出参数空

AE_VOID InterfaceInit(AE_HANDLE *AE_handle)
{
    
#ifndef IOS_INTERFACE
 //   AE_handle->fAE_Log = __android_log_print;
#else
   // AE_handle->fAE_Log = fprintf;
#endif
    
}

//！算法指针函数
//！主要是系统的调用函数和回调函数
//
//
//
//！不接受输入参数,默认输出参数空
/*
 void   (*start)(void);
 void   (*stop)(void);
 int    (*getStatus)(void);
 int    (*getBufferSize)(void) ;
 void   (*setBufferSize)(int size);
 void   (*clearBuffer)();
 int    (*wirte)(short data[],int data_size);
 double (*fillRate)();
 short* (*getRegSignal)(int *len_v);
 int    (*dem_init)(int n_v, double st_v, int gap_v, double *sFeq_v, double *eFeq_v,int feq_size);
 int    (*max)(int x,int y,short buf_p[],int buf_size);
 */
AE_VOID ALG_Init(ALG_Fxn *Algfxn)
{
    Algfxn->start         = start;
    Algfxn->stop          = stop;
    Algfxn->getStatus     = getStatus;
    Algfxn->getBufferSize = getBufferSize;
    Algfxn->setBufferSize = setBufferSize;
    Algfxn->clearBuffer   = clearBuffer;
    Algfxn->wirte         = wirte;
    Algfxn->fillRate      = fillRate;
   // Algfxn->getRegSignal  = getRegSignal;
  //  Algfxn->dem_init      = dem_init;
  //  Algfxn->max           = max;
}













