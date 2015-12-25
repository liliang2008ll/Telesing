#include <stdio.h>
#include "./sab_code/Code.h"
#include "mod.h"
#include <stdio.h>
#include <string.h>
#include "bch.h"
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
//获取时域波形
double *S_td[2],*A_td[4],*B_td[4];

//历史数据帧的头
double last_frame[2432] = {0};
double *p_last_frame = last_frame;

//总数据存放地 62个符号
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

//生成数据头
static  void gern_head_frame( char td_code){

	//找到符号对应的波形
	double *work_td  = S_td[td_code];

	//头,脖子,身体,尾巴
	double head[128]  = {0};
	double neck[128]  = {0};
	double body[2048] = {0};
	double tail[128]  = {0};

	//先处理身体
	memcpy(body,work_td,sizeof(double)*4);

	//产生头
	for(int i = 0 ; i < 128 ; i ++){
		head[i] = body[2048 - 128 * 2 + i] * (cos(M_PI * (i + 1) / (128 + 1) + M_PI) + 1) / 2;
	}

	//产生脖子
    for (int i = 0; i < 128; i++) {
    	neck[i] = body[i] * (cos(M_PI * (i + 1) / (128 + 1)) + 1) / 2;
    }
    //最后是小尾巴
    for (int i = 0; i < 128; i++) {
        tail[i] = body[i] * (cos(M_PI * (i + 1) / (128 + 1)) + 1) / 2;
    }

    //链接在一起
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
//生成数据帧
static double* gern_data_frame( char td_code){

	//找到符号对应的波形
	double *work_td  = A_td[td_code];

	//总数据,
	double frame[2432] = {0};

	//头,脖子,身体,尾巴
	double head[128]  = {0};
	double neck[128]  = {0};
	double body[2048] = {0};
	double tail[128]  = {0};

	//先处理身体
	memcpy(body,work_td,sizeof(double)*4);

	//产生头
	for(int i = 0 ; i < 128 ; i ++){
		head[i] = body[2048 - 128 * 2 + i] * (cos(M_PI * (i + 1) / (128 + 1) + M_PI) + 1) / 2;
	}

	//产生脖子
    for (int i = 0; i < 128; i++) {
    	neck[i] = body[i] * (cos(M_PI * (i + 1) / (128 + 1)) + 1) / 2;
    }
    //最后是小尾巴
    for (int i = 0; i < 128; i++) {
        tail[i] = body[i] * (cos(M_PI * (i + 1) / (128 + 1)) + 1) / 2;
    }

    //头不要,直接脖子开始
     for(int i = 0 ;i < 128;i++){
    	 frame[128+i] = neck[i];
     }
     //然后是身体
     for(int i = 0 ;i < 2048;i++){
    	 frame[256+i] = body[i];
     }
     //然后是小尾巴
     for(int i = 0;i < 128;i++){
    	 frame[2304+i] =  tail[i];
     }

     //处理小尾巴和头链接的问题,全局变量last_frame放的是上一个产生的数据帧
     int offset = sizeof(last_frame) -128;

     for (int i = 0; i < 128; i++) {
     	frame[offset + i] = last_frame[offset + i] + head[i];//上一个数据的小尾巴+加上自己的头部
     }
     //处理完毕的数据
     return frame;
}





//***************************************************************************
//函数名称：生成时域波形
//函数功能：
//输入参数：
//输出参数： 段+数据
//备	        注 ：
//
//**************************************************************************
double *create_double_wave(int data){

	//数据为整型,首先分解成16bit数据,用来给BCH编码
	char data_temp[4] = {0};
	for(int i = 0 ; i < 4;i++){
		data_temp[i] = (data>>(i*4))&&0xff;
	}

	//BCH编码
	unsigned long  encode_message_h,encode_message_l;
	bch_encode(data_temp[0],data_temp[1],&encode_message_l);
	bch_encode(data_temp[2],data_temp[3],&encode_message_h);

	//分解编码数据30-0 共计31bit
	char char_data_h[31]={0};
	char char_data_l[31]={0};

	for(int i = 0 ; i < 31;i++){
		char_data_l[i] = (encode_message_l >> i)&0x01;
		char_data_h[i] = (encode_message_h >> i)&0x01;
	}

	//初始化时域波形
	init_td();

	//先产生一个帧数据头,存放在last_frame数据里面
	gern_head_frame(0);

	//先做一个数据头
	memcpy(wave_data,last_frame,sizeof(last_frame));

	//然后继续下面的数据制作
	for(int i = 0 ;i < 31;i++){
		if( char_data_l[i]&&char_data_h[i]){//11符号
			p_last_frame = gern_data_frame(3);
			memcpy(&wave_data[2432 + 2304 * i],last_frame,2304*4);//更新符号到波形数组
		}else if(char_data_l[i]){//01
			p_last_frame = gern_data_frame(1);
			memcpy(&wave_data[2432 + 2304 * i],last_frame,2304*4);//更新符号到波形数组
		}else if(char_data_h[i]){//10
			p_last_frame = gern_data_frame(2);
			memcpy(&wave_data[2432 + 2304 * i],last_frame,2304*4);//更新符号到波形数组
		}else {//00
			p_last_frame = gern_data_frame(0);
			memcpy(&wave_data[2432 + 2304 * i],last_frame,2304*4);//更新符号到波形数组
		}

	}
	return wave_data;
}

short* create_short_wave(int data){

	//创建浮点类型的文件,存放在data数组中
	create_double_wave(data);

	//筛选峰值
	double wave_max_vaule = 0;
	for(int i = 0 ; i < sizeof(wave_data);i++){
		if(abs(wave_data[i])>wave_max_vaule )
			wave_max_vaule = wave_data[i];
	}
	//归一化
	short temp_wave_data= 0;
	for(int i = 0 ; i < sizeof(wave_data);i++){

		//四舍五入
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
