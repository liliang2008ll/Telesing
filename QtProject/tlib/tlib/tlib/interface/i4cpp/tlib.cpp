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
#include "./tlib.h"
#include "./tlib/core/core.h"


//!定义一个用于调用的核心类
CORE core;

//!调用库里面的函数
//!开始语音识别
int ACOMMS::recog_start(Recog_cfg input_cfg)
{
   return core.recog_start(input_cfg);
}

//!调用库里面的函数
//!配置运行参数
int ACOMMS::recog_setconfig(Recog_cfg input_cfg)
{
   return core.recog_setconfig(input_cfg);
}

//!调用库里面的函数
//!获取当前配置的系统参数
int ACOMMS::recog_getconfig(Recog_cfg input_cfg)
{
   return core.recog_getconfig(input_cfg);
}

//!调用库里面的函数
//!输入语音数据给core
int ACOMMS::recog_write(short *input_data, int input_data_len)
{
   return core.recog_write(input_data,input_data_len);
}

//!调用库里面的函数
//!获取语音识别库运行时的状态参数
int ACOMMS::recog_status(Recog_status output_status)
{
   return core.recog_status(output_status);
}

//!证书解密
string ACOMMS::cer_encrypt(string input_cer)
{
    return core.cer_encrypt(input_cer);
}
//!证书加密
string ACOMMS::cer_decrypt(string input_cer)
{
    return core.cer_decrypt(input_cer);
}
//!调用库里面的函数
//!绑定一个证书
int ACOMMS::cer_bind(string input_cer, int input_cer_len)
{
   return core.cer_bind(input_cer,input_cer_len);
}

//!调用库里面的函数
//!消除一个证书
int ACOMMS::cer_unbind(string input_cer, int input_cer_len)
{
   return core.cer_unbind(input_cer,input_cer_len);
}

//!调用库里面的函数
//!检查输入的证书状态
int ACOMMS::cer_isbind(string input_cer, int input_cer_len)
{
   return core.cer_isbind(input_cer,input_cer_len);
}
//!检查证书权限
int ACOMMS::cer_section_auth(string input_section)
{
   return core.cer_section_auth(input_section);
}

//!调用库里面的函数
//!停止语音识别
int ACOMMS::recog_stop()
{
    return core.recog_stop();
}


