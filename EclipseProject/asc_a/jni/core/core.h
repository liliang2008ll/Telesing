/*******************************************************************
 *  Copyright(c) 2015-2017 Nanjing telesing
 *  All rights reserved.
 *
 *  文件名称: core
 *  简要描述: acomms 核心算法对interface接口封装类库
 *
 *  当前版本:1.1
 *  作   者:Lee.liang
 *  日   期:
 *  说   明:
 *
 *  取代版本:1.0
 *  作   者:
 *  日   期:
 *  说   明:
 ******************************************************************/
#ifndef CORE_H
#define CORE_H
#include <stdio.h>
#include "tlib.h"

//!函数返回值
//!         调用成功
#define SUCCEED 0
//!         调用失败
#define FAILED  1


class  CORE{

    private:

    /*调试用参数*/
        int             local_debug_lv;//!调试等级      4
        int             local_init;    //!初始化标志位

    /*系统配置参数*/
        Recog_cfg       local_cfg;     //!            32
    /*系统状态参数*/
        Recog_status    local_status;  //!            12

        CER             local_cer;
    public :
    /*证书管理*/
        char * cer_encrypt(char * cer);//!根据输入的证书结构体加密
        char * cer_decrypt( char * cer);//!根据输入的证书结构体解密

        int cer_bind  (char * cer,int cer_len);//!绑定指定长度的证书文件
        int cer_unbind(char * cer,int cer_len);//!解绑定指定长度的证书文件
        int cer_isbind(char * cer,int cer_len);//!检查指定长度的证书文件是否绑定
        int cer_section_auth(char * input_section);//!检查证书(是否绑定?)//authority==>auth

        /*声码生成*/
        short* genr_wave(char * section,char * data,int genr_result);//out_genr_result 状态值 0成功,非0错误,找状态

    /*声码识别*/
//@      int recog_init(void);//!识别前初始化
//@      int recog_pause(void);//!暂停识别
//@      int recog_res(void);//!继续识别

        int recog_stop(void);//!停止识别
        int recog_start(Recog_cfg cfg);//!开始识别
        int recog_setconfig(Recog_cfg  cfg);//!运行中新设置参数
        int recog_getconfig(Recog_cfg cfg);//!获取系统的配置参数
        int recog_write(short *data,int data_len);//!写入获取到的音频数据到待分析的缓冲区
        int recog_status(Recog_status status);//!获取当前语音识别的状态

};


#endif
