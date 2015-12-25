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
#include "rsa_use.h"
#include "time_diff.h"
#include "mod.h"
//!函数返回值
//!         调用成功
#define SUCCEED 0
//!         调用失败
#define FAILED  1

#define MAX_CER 50

class  CORE{

    public:

	    int       cer_max_number;//允许导入的证书最大本书

		int       cer_generate_offset;//证书偏移
		int       cer_recognise_offset;
		int       cer_generate_recognise_offset;

    /*调试用参数*/
        int             local_debug_lv;//!调试等级      4
        char            first_run;    //!初始化标志位
        rsa_use         rsa1024;

    /*系统配置参数*/
        Recog_cfg 	  user_input_sys_cfg;	//用户输入的配置参数
        Recog_cfg 	  current_sys_cfg;	 	//系统当前的配置参数
        Recog_cfg 	  default_sys_cfg;	 	//系统默认的配置参数


    /*系统状态参数*/
        Recog_status   status;              //系统现在的状态

   /*存放证书原版*/
        int       cer_generate_days_left;
        int       cer_recognise_days_left;
        int       cer_generate_recognise_days_left;

        CER		  cer_generate[MAX_CER]; 	      //生成证书
        CER		  cer_recognise[MAX_CER]; 	      //识别证书
        CER		  cer_generate_recognise[MAX_CER]; //识别+生成证书

        int       generate_days_left[MAX_CER];
        int       recognise_days_left[MAX_CER];
        int       generate_recognise_days_left[MAX_CER];



        //更新系统各项参数
        void     updata_cfg(Recog_cfg cfg);
        void     updata_status(Recog_status status);
        void     updata_cer();
        char*    gen_random_string(int length); //产生随机字符串
        char     check_cer_magic_code();        //检查证书魔术字段
        char     check_cer_section();
      //  char

        //系统内部使用的小函数


    public :
        time_diff_days diff_days;

        char * cer_encrypt(char * cer);//!根据输入的证书结构体加密
        char * cer_decrypt( char * cer);//!根据输入的证书结构体解密

        int cer_bind  (char * cer);//!绑定指定长度的证书文件
        int cer_unbind(char * cer);//!解绑定指定长度的证书文件
        int cer_isbind(char * cer);//!检查指定长度的证书文件是否绑定
        int checkSectionsBindState(char * input_section);//!检查证书(是否绑定?)//authority==>auth


        short* genr_wave(char * section,char * data,int genr_result);//out_genr_result 状态值 0成功,非0错误,找状态


        int recog_stop(void);//!停止识别
        int recog_start(Recog_cfg cfg);//!开始识别
        int recog_setconfig(Recog_cfg  cfg);//!运行中新设置参数
        int recog_getconfig(Recog_cfg cfg);//!获取系统的配置参数
        int recog_write(short *data,int data_len);//!写入获取到的音频数据到待分析的缓冲区
        int recog_status(Recog_status status);//!获取当前语音识别的状态

     //更新系统各项参数
     //   void     updata_cfg(Recog_cfg cfg);
     //   void     updata_status(Recog_status status);

        //证书每个字段的长度
        char store_cer_section_length[9];

        CORE();
};


#endif
