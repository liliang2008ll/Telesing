#include <stdio.h>
#include "./sab_code/Code.h"
#include "mod.h"
#include <stdio.h>
#include <string.h>
#include "bch.h"
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
//��ȡʱ����
double *S_td[2],*A_td[4],*B_td[4];

//��ʷ����֡��ͷ
double last_frame[2432] = {0};
double *p_last_frame = last_frame;

//�����ݴ�ŵ� 62������
double wave_data[2432*70]= {0};
short byte_wave[2432*70*2] = {0};

static void init_td( void ){
	S_td[0] =  S0_TD;
	S_td[1] =  S1_TD;

	A_td[0] =  A00_TD;
	A_td[1] =  A01_TD;
	A_td[2] =  A10_TD;
	A_td[3] =  A11_TD;

	B_td[0] =  B00_TD;
	B_td[1] =  B01_TD;
	B_td[2] =  B10_TD;
	B_td[3] =  B11_TD;
}

//��������ͷ
static  void gern_head_frame( char td_code){

	//�ҵ����Ŷ�Ӧ�Ĳ���
	double *work_td  = S_td[td_code];

	//ͷ,����,����,β��
	double head[128]  = {0};
	double neck[128]  = {0};
	double body[2048] = {0};
	double tail[128]  = {0};

	//�ȴ�������
	memcpy(body,work_td,sizeof(double)*4);

	//����ͷ
	for(int i = 0 ; i < 128 ; i ++){
		head[i] = body[2048 - 128 * 2 + i] * (cos(M_PI * (i + 1) / (128 + 1) + M_PI) + 1) / 2;
	}

	//��������
    for (int i = 0; i < 128; i++) {
    	neck[i] = body[i] * (cos(M_PI * (i + 1) / (128 + 1)) + 1) / 2;
    }
    //�����Сβ��
    for (int i = 0; i < 128; i++) {
        tail[i] = body[i] * (cos(M_PI * (i + 1) / (128 + 1)) + 1) / 2;
    }

    //������һ��
     for(int i = 0;i < 128 ; i++ ){
    	 last_frame[i] =  head[i];
     }

     for(int i = 0 ;i < 128;i++){
    	 last_frame[128+i] = neck[i];
     }

     for(int i = 0 ;i < 2048;i++){
    	 last_frame[256+i] = body[i];
     }
     for(int i = 0;i < 128;i++){
    	 last_frame[2304+i] =  tail[i];
     }

}
//��������֡
static double* gern_data_frame( char td_code){

	//�ҵ����Ŷ�Ӧ�Ĳ���
	double *work_td  = A_td[td_code];

	//������,
	double frame[2432] = {0};

	//ͷ,����,����,β��
	double head[128]  = {0};
	double neck[128]  = {0};
	double body[2048] = {0};
	double tail[128]  = {0};

	//�ȴ�������
	memcpy(body,work_td,sizeof(double)*4);

	//����ͷ
	for(int i = 0 ; i < 128 ; i ++){
		head[i] = body[2048 - 128 * 2 + i] * (cos(M_PI * (i + 1) / (128 + 1) + M_PI) + 1) / 2;
	}

	//��������
    for (int i = 0; i < 128; i++) {
    	neck[i] = body[i] * (cos(M_PI * (i + 1) / (128 + 1)) + 1) / 2;
    }
    //�����Сβ��
    for (int i = 0; i < 128; i++) {
        tail[i] = body[i] * (cos(M_PI * (i + 1) / (128 + 1)) + 1) / 2;
    }

    //ͷ��Ҫ,ֱ�Ӳ��ӿ�ʼ
     for(int i = 0 ;i < 128;i++){
    	 frame[128+i] = neck[i];
     }
     //Ȼ��������
     for(int i = 0 ;i < 2048;i++){
    	 frame[256+i] = body[i];
     }
     //Ȼ����Сβ��
     for(int i = 0;i < 128;i++){
    	 frame[2304+i] =  tail[i];
     }

     //����Сβ�ͺ�ͷ���ӵ�����,ȫ�ֱ���last_frame�ŵ�����һ������������֡
     int offset = sizeof(last_frame) -128;

     for (int i = 0; i < 128; i++) {
     	frame[offset + i] = last_frame[offset + i] + head[i];//��һ�����ݵ�Сβ��+�����Լ���ͷ��
     }
     //������ϵ�����
     return frame;
}





//***************************************************************************
//�������ƣ�����ʱ����
//�������ܣ�
//���������
//��������� ��+����
//��	        ע ��
//
//**************************************************************************
double *create_double_wave(int data){

	//����Ϊ����,���ȷֽ��16bit����,������BCH����
	char data_temp[4] = {0};
	for(int i = 0 ; i < 4;i++){
		data_temp[i] = (data>>(i*4))&&0xff;
	}

	//BCH����
	unsigned long  encode_message_h,encode_message_l;
	bch_encode(data_temp[0],data_temp[1],&encode_message_l);
	bch_encode(data_temp[2],data_temp[3],&encode_message_h);

	//�ֽ��������30-0 ����31bit
	char char_data_h[31]={0};
	char char_data_l[31]={0};

	for(int i = 0 ; i < 31;i++){
		char_data_l[i] = (encode_message_l >> i)&0x01;
		char_data_h[i] = (encode_message_h >> i)&0x01;
	}

	//��ʼ��ʱ����
	init_td();

	//�Ȳ���һ��֡����ͷ,�����last_frame��������
	gern_head_frame(0);

	//����һ������ͷ
	memcpy(wave_data,last_frame,sizeof(last_frame));

	//Ȼ������������������
	for(int i = 0 ;i < 31;i++){
		if( char_data_l[i]&&char_data_h[i]){//11����
			p_last_frame = gern_data_frame(3);
			memcpy(&wave_data[2432 + 2304 * i],last_frame,2304*4);//���·��ŵ���������
		}else if(char_data_l[i]){//01
			p_last_frame = gern_data_frame(1);
			memcpy(&wave_data[2432 + 2304 * i],last_frame,2304*4);//���·��ŵ���������
		}else if(char_data_h[i]){//10
			p_last_frame = gern_data_frame(2);
			memcpy(&wave_data[2432 + 2304 * i],last_frame,2304*4);//���·��ŵ���������
		}else {//00
			p_last_frame = gern_data_frame(0);
			memcpy(&wave_data[2432 + 2304 * i],last_frame,2304*4);//���·��ŵ���������
		}

	}
	return wave_data;
}

short* create_short_wave(int data){

	//�����������͵��ļ�,�����data������
	create_double_wave(data);

	//ɸѡ��ֵ
	double wave_max_vaule = 0;
	for(int i = 0 ; i < sizeof(wave_data);i++){
		if(abs(wave_data[i])>wave_max_vaule )
			wave_max_vaule = wave_data[i];
	}
	//��һ��
	short temp_wave_data= 0;
	for(int i = 0 ; i < sizeof(wave_data);i++){

		//��������
		temp_wave_data      = (short)(wave_data[i]/wave_max_vaule*32767 + 0.5);
		byte_wave[i * 2]    = (char)(temp_wave_data&0xff);
		byte_wave[i * 2 + 1]= (char)((temp_wave_data>>8)&0xff);
	}
	return byte_wave;
}
WAVE::WAVE(){

	last_frame = new double[SYMBOL_FRAME_LENGHT];
	wave       = new double[WAVE_SIZE];
	s_td       = new double[S_SYMBOL_N];
	a_td       = new double[A_SYMBOL_N];
	b_td       = new double[B_SYMBOL_N];

}



WAVE::~WAVE(){

	delete [] last_frame;
	delete [] wave;
	delete [] a_td;
	delete [] b_td;

}
