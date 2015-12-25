/*******************************************************************
 *  Copyright(c) 2015-2017 Nanjing telesing
 *  All rights reserved.
 *
 *  �ļ�����: core
 *  ��Ҫ����: acomms �����㷨��interface�ӿڷ�װ���
 *          1.2�޸ĺ������Ƶ��°汾
 *
 *  ��ǰ�汾:1.2
 *  ��   ��:Lee.liang
 *  ��   ��:
 *  ˵   ��:
 *
 *  ȡ���汾:1.1
 *  ��   ��:
 *  ��   ��:
 *  ˵   ��:
 ******************************************************************/
#ifndef TLIB_H
#define TLIB_H
#include <jni.h>
#include <stdio.h>

#include <android/log.h>


#ifdef __cplusplus
extern "C" {
#endif

//�㷨�м�ǿ��Ҫ������ṹ���ǰ����ֶ��뷽ʽ��ŵ�
#pragma pack(1)
/*֤��֡�ṹ*/
typedef  struct{

//!��֤��Ϣ�ֶ�
    char   section[4+1];         //���
    char   expiration[10+1];     //ʧЧ����    YYYY-MM-DD
    char   authority[1+1];       //Ȩ��       ��1������Ȩ��  ��2�� ʶ��Ȩ�� ��3��ͨ�� =  ���� + ʶ��

//!������Ϣ�ֶ�
    char  id[10+1];               //���       ���硱232134234�� ������
    char  org_id[10+1];           //�������    ���硱1123123123�� ������
    char  genr_time[19+1];        //����ʱ�� YYYY-MM-DD hh:mm:ss
    char  genr_type[1+1];         //���ɷ�ʽ '1' ��վ��ȡ ��2�� �ӿڻ�ȡ(API)
    char  genr_ip[15+1];          //192.168.1.100
    char  sect_src[64+1];         //�����Դ    ���� ������E����

//!����������Ϣ
//!
    //�ַ�������,��ֹ����
     char magic_data[6+1];

}CER;

#pragma pack ()
/*ϵͳ���ò���*/
typedef struct {
            int     gap;        //!ͬ��ʧ�ܺ�ϵͳFFT������ת��С      4
            int     buffer_len; //!��Ƶ���ݻ�������С               4
            int     start_feq[4];  //!��ʼƵ��                       4
            int     end_feq[4];    //!����Ƶ��                       4
            int     fft_points; //!fft����
            int     fft_chains; //!fft�ŵ���
            double  st;         //!��ʶ����ֵSimilarity threshold  8
}Recog_cfg;

/*ϵͳ״̬����*/
typedef struct {
            int        recog_state;  //!��ǰ�����������ں��߳�״̬        4
            double     blank_buffer; //!������ʣ��Ŀռ�                4
            double     ss;           //!��ʶ����ֵSimilarity state 8
}Recog_status;

//�����������ڴ��JAVA��ز���
typedef struct{

	JavaVM* 	  jvm;			//Java�߳�
	jobject 	  jobj;
	jclass  	  jclass;

	jmethodID     cb_recog_result; //�ص������б�

     jchar  	  first_start;  	//��ʼ������

     Recog_status sys_status;	//״̬

     Recog_cfg 	  sys_cfg;	 	//����



}JAVA;

extern JAVA *g_java ;

//�ص�JAVA����
void callback_java( char *section, char *data,int spend_times);

//#define _USED_IN_PTHREAD
#define INFOR(a)    __android_log_print(ANDROID_LOG_ERROR, "NATIVE:",a);

#ifdef __cplusplus
}
#endif
#endif