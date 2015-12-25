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
#include "rsa_use.h"
//!��������ֵ
//!         ���óɹ�
#define SUCCEED 0
//!         ����ʧ��
#define FAILED  1

#define MAX_CER 50
class  CORE{

    private:

	    int       cer_max_number;//���������֤�������

		int       cer_generate_offset;//֤��ƫ��
		int       cer_recognise_offset;
		int       cer_generate_recognise_offset;

    /*�����ò���*/
        int             local_debug_lv;//!���Եȼ�      4
        char            first_run;    //!��ʼ����־λ
        rsa_use         rsa1024;

    /*ϵͳ���ò���*/
        Recog_cfg 	  user_input_sys_cfg;	//�û���������ò���
        Recog_cfg 	  current_sys_cfg;	 	//ϵͳ��ǰ�����ò���
        Recog_cfg 	  default_sys_cfg;	 	//ϵͳĬ�ϵ����ò���


    /*ϵͳ״̬����*/
        Recog_status   status;              //ϵͳ���ڵ�״̬

   /*���֤��ԭ��*/
        CER		  cer_generate[MAX_CER]; 	      //����֤��
        CER		  cer_recognise[MAX_CER]; 	      //ʶ��֤��
        CER		  cer_generate_recognise[MAX_CER]; //ʶ��+����֤��

        //����ϵͳ�������
        void     updata_cfg(Recog_cfg cfg);
        void     updata_status(Recog_status status);
        char*    gen_random_string(int length); //��������ַ���
        char     check_cer_magic_code();        //���֤��ħ���ֶ�

        //ϵͳ�ڲ�ʹ�õ�С����


    public :

        char * cer_encrypt(char * cer);//!���������֤��ṹ�����
        char * cer_decrypt( char * cer);//!���������֤��ṹ�����

        int cer_bind  (char * cer);//!��ָ�����ȵ�֤���ļ�
        int cer_unbind(char * cer);//!���ָ�����ȵ�֤���ļ�
        int cer_isbind(char * cer);//!���ָ�����ȵ�֤���ļ��Ƿ��
        int checkSectionsBindState(char * input_section);//!���֤��(�Ƿ��?)//authority==>auth


        short* genr_wave(char * section,char * data,int genr_result);//out_genr_result ״ֵ̬ 0�ɹ�,��0����,��״̬


        int recog_stop(void);//!ֹͣʶ��
        int recog_start(Recog_cfg cfg);//!��ʼʶ��
        int recog_setconfig(Recog_cfg  cfg);//!�����������ò���
        int recog_getconfig(Recog_cfg cfg);//!��ȡϵͳ�����ò���
        int recog_write(short *data,int data_len);//!д���ȡ������Ƶ���ݵ��������Ļ�����
        int recog_status(Recog_status status);//!��ȡ��ǰ����ʶ���״̬

     //����ϵͳ�������
     //   void     updata_cfg(Recog_cfg cfg);
     //   void     updata_status(Recog_status status);
        CORE();
};


#endif