/*******************************************************************
 *  Copyright(c) 2015-2017 Nanjing telesing
 *  All rights reserved.
 *
 *  �ļ�����: core
 *  ��Ҫ����: acomms �����㷨��interface�ӿڷ�װ���
 *
 *  ��ǰ�汾:1.1
 *  ��   ��:Lee.liang
 *  ��   ��:
 *  ˵   ��:
 *
 *  ȡ���汾:1.0
 *  ��   ��:
 *  ��   ��:
 *  ˵   ��:
 ******************************************************************/
#ifndef CORE_H
#define CORE_H
#include <stdio.h>
#include "tlib.h"

//!��������ֵ
//!         ���óɹ�
#define SUCCEED 0
//!         ����ʧ��
#define FAILED  1


class  CORE{

    private:

    /*�����ò���*/
        int             local_debug_lv;//!���Եȼ�      4
        int             local_init;    //!��ʼ����־λ

    /*ϵͳ���ò���*/
        Recog_cfg       local_cfg;     //!            32
    /*ϵͳ״̬����*/
        Recog_status    local_status;  //!            12

        CER             local_cer;
    public :
    /*֤�����*/
        char * cer_encrypt(char * cer);//!���������֤��ṹ�����
        char * cer_decrypt( char * cer);//!���������֤��ṹ�����

        int cer_bind  (char * cer,int cer_len);//!��ָ�����ȵ�֤���ļ�
        int cer_unbind(char * cer,int cer_len);//!���ָ�����ȵ�֤���ļ�
        int cer_isbind(char * cer,int cer_len);//!���ָ�����ȵ�֤���ļ��Ƿ��
        int cer_section_auth(char * input_section);//!���֤��(�Ƿ��?)//authority==>auth

        /*��������*/
        short* genr_wave(char * section,char * data,int genr_result);//out_genr_result ״ֵ̬ 0�ɹ�,��0����,��״̬

    /*����ʶ��*/
//@      int recog_init(void);//!ʶ��ǰ��ʼ��
//@      int recog_pause(void);//!��ͣʶ��
//@      int recog_res(void);//!����ʶ��

        int recog_stop(void);//!ֹͣʶ��
        int recog_start(Recog_cfg cfg);//!��ʼʶ��
        int recog_setconfig(Recog_cfg  cfg);//!�����������ò���
        int recog_getconfig(Recog_cfg cfg);//!��ȡϵͳ�����ò���
        int recog_write(short *data,int data_len);//!д���ȡ������Ƶ���ݵ��������Ļ�����
        int recog_status(Recog_status status);//!��ȡ��ǰ����ʶ���״̬

};


#endif
