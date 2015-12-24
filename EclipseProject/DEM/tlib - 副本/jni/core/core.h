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
#include <string>
#include "./tlib/interface/i4cpp/tlib.h"
using namespace std;

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

    public :
    /*证书管理*/
        string cer_encrypt(string input_cer);//!根据输入的证书结构体加密
        string cer_decrypt(string input_cer);//!根据输入的证书结构体解密

        int cer_bind  (string input_cer,int input_cer_len);//!绑定指定长度的证书文件
        int cer_unbind(string input_cer,int input_cer_len);//!解绑定指定长度的证书文件
        int cer_isbind(string input_cer,int input_cer_len);//!检查指定长度的证书文件是否绑定
        int cer_section_auth(string input_section);//!检查证书(是否绑定?)//authority==>auth

        /*声码生成*/
        short* genr_wave(string input_section,string input_data,int out_genr_result);//out_genr_result 状态值 0成功,非0错误,找状态

    /*声码识别*/
//@      int recog_init(void);//!识别前初始化
//@      int recog_pause(void);//!暂停识别
//@      int recog_res(void);//!继续识别

        int recog_stop(void);//!停止识别
        int recog_start(Recog_cfg input_cfg);//!开始识别
        int recog_setconfig(Recog_cfg input_cfg);//!运行中新设置参数
        int recog_getconfig(Recog_cfg input_cfg);//!获取系统的配置参数
        int recog_write(short *input_data,int input_data_len);//!写入获取到的音频数据到待分析的缓冲区
        int recog_status(Recog_status output_status);//!获取当前语音识别的状态

};


#endif
























