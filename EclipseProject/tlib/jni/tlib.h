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

#include <stdio.h>
#include <string>
#include <android/log.h>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

//�㷨�м�ǿ��Ҫ������ṹ���ǰ����ֶ��뷽ʽ��ŵ�
#pragma pack(4)
/*֤��֡�ṹ*/
typedef  struct{

//!��֤��Ϣ�ֶ�
    char*  section;         //���
    char*  expiration;      //ʧЧ����    YYYY-MM-DD
    char*  authority;       //Ȩ��       ��1������Ȩ��  ��2�� ʶ��Ȩ�� ��3��ͨ�� =  ���� + ʶ��



//!������Ϣ�ֶ�
    char*  id;               //���       ���硱232134234�� ������
    char*  org_id;           //�������    ���硱1123123123�� ������
    char*  sect_src;         //�����Դ    ���� ������E����
    char*  genr_time;        //����ʱ�� YYYY-MM-DD hh:mm:ss
    char*  genr_type;        //���ɷ�ʽ '1' ��վ��ȡ ��2�� �ӿڻ�ȡ(API)

    char*  genr_ip;          //192.168.1.100

//!����������Ϣ
//!
    //�ַ�������,��ֹ����
    void* ptr;
    //@�ڲ�ʹ��,���Խӿ�
    char*  append_data;     //�����������
    int cer_length;  //�����ַ�������

}CER;

#pragma pack ()
/*ϵͳ���ò���*/
typedef struct {
            int     input_gap;        //!ͬ��ʧ�ܺ�ϵͳFFT������ת��С      4
            int     input_buffer_len; //!��Ƶ���ݻ�������С               4
            double  *input_start_feq;  //!��ʼƵ��                       4
            double  *input_end_feq;    //!����Ƶ��                       4
            int     input_fft_points; //!fft����
            int     input_fft_chains; //!fft�ŵ���
            double  input_st;         //!��ʶ����ֵSimilarity threshold  8
}Recog_cfg;
/*ϵͳ״̬����*/
typedef struct {
            int        output_recog_state;  //!��ǰ�����������ں��߳�״̬        4
            double     output_blank_buffer; //!������ʣ��Ŀռ�                4
            double     output_ss;           //!��ʶ����ֵSimilarity state 8
}Recog_status;


class  ACOMMS{

    private:

/*�����ò���*/
        int             local_debug_lv;//!���Եȼ�      4
        void *          local_core;    //!            4

/*ϵͳ���ò���*/
        Recog_cfg       local_cfg;     //!            32
/*ϵͳ״̬����*/
        Recog_status    local_status;  //!            12

    public :
/*֤�����*/
        char*  cer_encrypt(char*  input_cer);//!���������֤��ṹ�����
        char*  cer_decrypt(char*  input_cer);//!���������֤��ṹ�����
        int    cer_bind  (char*  input_cer,int input_cer_len);//!��ָ�����ȵ�֤���ļ�
        int    cer_unbind(char*  input_cer,int input_cer_len);//!���ָ�����ȵ�֤���ļ�
        int    cer_isbind(char*  input_cer,int input_cer_len);//!���ָ�����ȵ�֤���ļ��Ƿ��
        int    cer_section_auth(char*  input_section);//!���֤��(�Ƿ��?)//authority==>auth


        /*��������*/
        short* genr_wave(string input_section,string input_data,int out_genr_result);//out_genr_result ״ֵ̬ 0�ɹ�,��0����,��״̬

        int recog_stop(void);//!ֹͣʶ��
        int recog_start(Recog_cfg input_cfg);//!��ʼʶ��
        int recog_setconfig(Recog_cfg input_cfg);//!�����������ò���
        int recog_getconfig(Recog_cfg input_cfg);//!��ȡϵͳ�����ò���
        int recog_write(short *input_data,int input_data_len);//!д���ȡ������Ƶ���ݵ��������Ļ�����
        int recog_status(Recog_status output_status);//!��ȡ��ǰ����ʶ���״̬

};

//�ص�JAVA����
void callback_java( char *section, char *data,int times);
//#define _USED_IN_PTHREAD
#ifdef __cplusplus
}
#endif
#endif