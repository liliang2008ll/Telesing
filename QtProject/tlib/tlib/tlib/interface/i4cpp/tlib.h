/*******************************************************************
 *  Copyright(c) 2015-2017 Nanjing telesing
 *  All rights reserved.
 *
 *  文件名称: core
 *  简要描述: acomms 核心算法对interface接口封装类库
 *          1.2修改函数名称到新版本
 *
 *  当前版本:1.2
 *  作   者:Lee.liang
 *  日   期:
 *  说   明:
 *
 *  取代版本:1.1
 *  作   者:
 *  日   期:
 *  说   明:
 ******************************************************************/
#ifndef TLIB_H
#define TLIB_H

#include <stdio.h>
#include <string>

using namespace std;

/*证书帧结构*/
typedef  struct{

//!验证信息字段
    string section;         //码段    4      4字节 "1352"
    string expiration;      //失效日期 10    YYYY-MM-DD "2015-12-20"  "2012-08-08"
    string authority;       //权限     1   ’1‘生成权限  ’2‘ 识别权限    ’3‘通用 =  生成 + 识别

    //@内部使用,调试接口
    string append_data;     //随机附加数据

//!附加信息字段
    string id;               //编号      10     例如”12345678“ 正整数
    string org_id;           //机构编号   10     例如”1123123123“ 正整数

    string genr_time;        //生成时间   19       YYYY-MM-DD hh:mm:ss
    string genr_type;        //生成方式   1        '1' 网站获取 ‘2’ 接口获取(API)
    string genr_ip;          //         15      192.168.001.100
     string sect_src;         //码段来源   64     例如 ”智声E联“
//!辅助调试信息
//!
    int cer_length;  //整个字符串长度

}CER;

/*系统配置参数*/
typedef struct {
            int     input_gap;        //!同步失败后系统FFT窗口跳转大小      4
            int     input_buffer_len; //!音频数据缓冲区大小               4
            double  *input_start_feq;  //!开始频率                       4
            double  *input_end_feq;    //!结束频率                       4
            int     input_fft_points; //!fft点数
            int     input_fft_chains; //!fft信道数
            double  input_st;         //!相识度阈值Similarity threshold  8
}Recog_cfg;
/*系统状态参数*/
typedef struct {
            int        output_recog_state;  //!当前语音处理的内核线程状态        4
            double     output_blank_buffer; //!缓冲区剩余的空间                4
            double     output_ss;           //!相识度阈值Similarity state 8
}Recog_status;


class  ACOMMS{

    private:

/*调试用参数*/
        int             local_debug_lv;//!调试等级      4
        void *          local_core;    //!            4

/*系统配置参数*/
        Recog_cfg       local_cfg;     //!            32
/*系统状态参数*/
        Recog_status    local_status;  //!            12

    public :
/*证书管理*/
        string cer_encrypt(string input_cer);//!根据输入的证书结构体加密
        string cer_decrypt(string input_cer);//!根据输入的证书结构体解密
        int    cer_bind  (string input_cer,int input_cer_len);//!绑定指定长度的证书文件
        int    cer_unbind(string input_cer,int input_cer_len);//!解绑定指定长度的证书文件
        int    cer_isbind(string input_cer,int input_cer_len);//!检查指定长度的证书文件是否绑定
        int    cer_section_auth(string input_section);//!检查证书(是否绑定?)//authority==>auth


        /*声码生成*/
        short* genr_wave(string input_section,string input_data,int out_genr_result);//out_genr_result 状态值 0成功,非0错误,找状态

        int recog_stop(void);//!停止识别
        int recog_start(Recog_cfg input_cfg);//!开始识别
        int recog_setconfig(Recog_cfg input_cfg);//!运行中新设置参数
        int recog_getconfig(Recog_cfg input_cfg);//!获取系统的配置参数
        int recog_write(short *input_data,int input_data_len);//!写入获取到的音频数据到待分析的缓冲区
        int recog_status(Recog_status output_status);//!获取当前语音识别的状态

};

#endif
