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
#include <jni.h>
#include <stdio.h>

#include <android/log.h>


#ifdef __cplusplus
extern "C" {
#endif

//算法中间强制要求这个结构体是按照字对齐方式存放的
#pragma pack(1)
/*证书帧结构*/
typedef  struct{

//!验证信息字段
    char   section[4+1];         //码段
    char   expiration[10+1];     //失效日期    YYYY-MM-DD
    char   authority[1+1];       //权限       ’1‘生成权限  ’2‘ 识别权限 ’3‘通用 =  生成 + 识别

//!附加信息字段
    char  id[10+1];               //编号       例如”232134234“ 正整数
    char  org_id[10+1];           //机构编号    例如”1123123123“ 正整数
    char  genr_time[19+1];        //生成时间 YYYY-MM-DD hh:mm:ss
    char  genr_type[1+1];         //生成方式 '1' 网站获取 ‘2’ 接口获取(API)
    char  genr_ip[15+1];          //192.168.1.100
    char  sect_src[64+1];         //码段来源    例如 ”智声E联“

//!辅助调试信息
//!
    //字符串保护,防止问题
     char magic_data[6+1];

}CER;

#pragma pack ()
/*系统配置参数*/
typedef struct {
            int     gap;        //!同步失败后系统FFT窗口跳转大小      4
            int     buffer_len; //!音频数据缓冲区大小               4
            int     start_feq[4];  //!开始频率                       4
            int     end_feq[4];    //!结束频率                       4
            int     fft_points; //!fft点数
            int     fft_chains; //!fft信道数
            double  st;         //!相识度阈值Similarity threshold  8
}Recog_cfg;

/*系统状态参数*/
typedef struct {
            int        recog_state;  //!当前语音处理的内核线程状态        4
            double     blank_buffer; //!缓冲区剩余的空间                4
            double     ss;           //!相识度阈值Similarity state 8
}Recog_status;

//定义数据用于存放JAVA相关参数
typedef struct{

	JavaVM* 	  jvm;			//Java线程
	jobject 	  jobj;
	jclass  	  jclass;

	jmethodID     cb_recog_result; //回调函数列表

     jchar  	  first_start;  	//初始化互斥

     Recog_status sys_status;	//状态

     Recog_cfg 	  sys_cfg;	 	//配置



}JAVA;

extern JAVA *g_java ;

//回调JAVA方法
void callback_java( char *section, char *data,int spend_times);
int  check_sections_state(char *section);
//#define _USED_IN_PTHREAD
#define INFOR(a)    __android_log_print(ANDROID_LOG_ERROR, "NATIVE:",a);

#ifdef __cplusplus
}
#endif
#endif
