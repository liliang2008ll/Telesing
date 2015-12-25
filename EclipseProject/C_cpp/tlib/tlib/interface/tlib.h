#ifndef tlib_h
#define tlib_h
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

//算法中间强制要求这个结构体是按照字对齐方式存放的
#pragma pack(1)
/*证书帧结构*/
typedef struct {

	//!验证信息字段
	char section[4 + 1];         //码段
	char expiration[10 + 1];     //失效日期    YYYY-MM-DD
	char authority[1 + 1];       //权限       ’1‘生成权限  ’2‘ 识别权限 ’3‘通用 =  生成 + 识别

	//!附加信息字段
	char id[10 + 1];               //编号       例如”232134234“ 正整数
	char org_id[10 + 1];           //机构编号    例如”1123123123“ 正整数
	char genr_time[19 + 1];        //生成时间    YYYY-MM-DD hh:mm:ss
	char genr_type[1 + 1];         //生成方式    '1' 网站获取 ‘2’ 接口获取(API)
	char genr_ip[15 + 1];          //           192.168.1.100
	char sect_src[64 + 1];         //码段来源    例如 ”智声E联“

} CER;

/*系统配置参数*/
typedef struct {
	int gap;        //!同步失败后系统FFT窗口跳转大小      4
	int buffer_len; //!音频数据缓冲区大小               4
	int start_feq[4];  //!开始频率                       4
	int end_feq[4];    //!结束频率                       4
	int fft_points; //!fft点数
	int fft_chains; //!fft信道数
	double st;         //!相识度阈值Similarity threshold  8
} Recog_cfg;

/*系统状态参数*/
typedef struct {
	int recog_state;  //!当前语音处理的内核线程状态        4
	double blank_buffer; //!缓冲区剩余的空间                4
	double ss;           //!相识度阈值Similarity state     8
} Recog_status;

//call core interface
class ACOMMS {
public:
	char * cer_encrypt(char * cer);           //!根据输入的证书结构体加密
	char * cer_decrypt(char * cer);           //!根据输入的证书结构体解密

	int cer_bind(char * cer);           //!绑定指定长度的证书文件
	int cer_unbind(char * cer);           //!解绑定指定长度的证书文件

	int cer_isbind(char * cer);           //!检查指定长度的证书文件是否绑定
	int checkSectionsBindState(char *section);  //!检查证书(是否绑定?)//authority==>auth

	short* genr_wave(char * section, char * data, int result); //out_genr_result 状态值 0成功,非0错误,找状态

	int recog_stop(void);           //!停止识别
	int recog_start(Recog_cfg cfg);           //!开始识别
	int recog_set_config(Recog_cfg cfg);           //!运行中新设置参数
	int recog_get_config(Recog_cfg cfg);           //!获取系统的配置参数
	int recog_write(short *data, int data_len);           //!写入获取到的音频数据到待分析的缓冲区
	int recog_status(Recog_status status);           //!获取当前语音识别的状态

	void *(callback)(int section, int data, int spend_time);
};

#ifdef __cplusplus
}
#endif
#endif /* tlib_h */

