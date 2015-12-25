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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "core.h"
#include  "dem.h"
#include "tlib.h"
#include "core.h"


//!
//! \brief CORE::cer_encrypt
//! \param input_cer
//! \return
//!
char * CORE::cer_encrypt(char *cer)
{

	  char cer_store[200];

	  char cer_rsa_code[1100];

	  //密文上半截
	  char cer_rsa_encode_first[600];
	  //密文下半截
	  char cer_rsa_encode_second[600];

	  int  length;

	   memset(cer_store,'\0',sizeof(char)*255);
	   memset(cer_rsa_code,'\0',sizeof(char)*1100);
	   memset(cer_rsa_encode_first,'\0',sizeof(char)*600);
	   memset(cer_rsa_encode_second,'\0',sizeof(char)*600);

	  //要加密的信息保存到本地
	  for(int i = 0 ; i < 200 ; i++){
		  cer_store[i] = *(cer + i);
	  }

	  //解析前面的100 放在0-512 里面,这个里面放的是字符,不是ASCII两个字符算一个ASCII码,
	  //所以这个地方使用512而不是512
	  rsa1024.rsa_enc_str(cer_store,100,cer_rsa_encode_first,length);

	  //加密后面的100个
	  rsa1024.rsa_enc_str(&cer_store[100],100,cer_rsa_encode_second,length);


	 //数据准备返还给SDK
	 strcat(cer_rsa_code,cer_rsa_encode_first);
	 strcat(cer_rsa_code,cer_rsa_encode_second);

	 return  cer_rsa_code;

}
//!
//! \brief CORE::cer_decrypt
//! \param output_cer
//! \return
//!
char * CORE::cer_decrypt( char *cer)
{
	char cer_store[200];

	char cer_store_first[100];
	char cer_store_second[100];

	char cer_rsa_encode[1024];

	//清理控件准备放数据
	memset(cer_store     ,0,200);
	memset(cer_store_first,0,100);
	memset(cer_store_second,0,100);
	int  length = 0;

	//要解密的信息保存到本地
	for(int i = 0 ; i < 1024 ; i++){
	  cer_rsa_encode[i] = *(cer + i);
	}

	//解密前半截
	rsa1024.rsa_dnc_str(cer_rsa_encode,512,cer_store_first,length);


	//解密后半截
	rsa1024.rsa_dnc_str(&cer_rsa_encode[512],512,cer_store_second,length);


	//准备返回给SDK
	for(int i = 0 ;i <100;i++){
		cer_store[i]     = cer_store_first[i];
		cer_store[i+100] = cer_store_second[i];
	}

	__android_log_print(ANDROID_LOG_INFO, "CORE:","cer_decrypt= %s", cer_store );
	return cer_store;
}
//!
//! \brief CORE::cer_bind
//! \param input_cer
//! \param input_cer_len
//! \return Yes or No
int CORE::cer_bind(char *  input_cer)
{

	char cer_store[200];
	int offset = 0;
	int cer_authority_position = 0;
	int cer_genr_time_position = 0;
	char cer_expiration_time[11];
	int  cer_expiration_days = 0;
	memset(cer_store,0,200);
	memset(cer_expiration_time,0,11);

	cer_authority_position =store_cer_section_length[0]+store_cer_section_length[1];

	cer_genr_time_position =store_cer_section_length[0];


	//证书信息保存到本地
	  for(int i = 0 ; i < 200 ; i++){
		  cer_store[i] = *(input_cer + i);
	  }
	__android_log_print(ANDROID_LOG_INFO, "BIND:","cer_decrypt= %s", cer_store );

	//查看证书生成日期和当前日期，防止修改本地时间，使用过期证书
	memcpy(cer_expiration_time,&cer_store[65+cer_genr_time_position],store_cer_section_length[1]);

	__android_log_print(ANDROID_LOG_INFO, "UPUPUUPUPUUPUP:","cer_expiration_time= %s", cer_expiration_time );

	//日期转换
//	diff_days.get_input_time(cer_expiration_time);
	cer_expiration_days = diff_days.get_sys_diff_days(cer_expiration_time);

	__android_log_print(ANDROID_LOG_INFO, "UPUPUUPUPUUPUP:","cer_expiration_YYY= %d", diff_days.time_data[0]);
	__android_log_print(ANDROID_LOG_INFO, "UPUPUUPUPUUPUP:","cer_expiration_MMM= %d", diff_days.time_data[1]);
	__android_log_print(ANDROID_LOG_INFO, "UPUPUUPUPUUPUP:","cer_expiration_DDD= %d", diff_days.time_data[2]);
	__android_log_print(ANDROID_LOG_INFO, "UPUPUUPUPUUPUP:","cer_expiration_days= %d", cer_expiration_days);

	//判定证书是不是在有效期之内
	if(cer_expiration_days < 0){ return - 3;}

	//判定当前需要绑定的证书的类型
	switch(cer_store[65+cer_authority_position]){

	//生成权限
	case '1':	//记录他的验证信息字段.其他的可以不需要记录
				memcpy(cer_generate[cer_generate_offset].section,\
						&cer_store[65+offset],store_cer_section_length[0]);
				offset+=store_cer_section_length[0];

				memcpy(cer_generate[cer_generate_offset].expiration,&cer_store[65+offset],store_cer_section_length[1]);
				offset+=store_cer_section_length[1];

				memcpy(cer_generate[cer_generate_offset].authority,&cer_store[65+offset],store_cer_section_length[2]);
				offset+=store_cer_section_length[2];

				//记录证书时间
				generate_days_left[cer_generate_offset] = cer_expiration_days;

				//允许绑定的证书最多只能是4本
				if(cer_generate_offset > cer_max_number) cer_generate_offset = 0;
				 cer_generate_offset++;

				 //记录此种证书的有效期,更新证书有效期
				 if(cer_expiration_days >cer_generate_days_left )
					 cer_generate_days_left = cer_expiration_days;

				__android_log_print(ANDROID_LOG_INFO, "BIND:","111111111111111111" );
				__android_log_print(ANDROID_LOG_INFO, "BIND:","%s", cer_generate[cer_generate_offset].section );
				__android_log_print(ANDROID_LOG_INFO, "BIND:","%s", cer_generate[cer_generate_offset].expiration );
				__android_log_print(ANDROID_LOG_INFO, "BIND:","%s", cer_generate[cer_generate_offset].authority );
		break;

	//识别权限
	case '2':
				__android_log_print(ANDROID_LOG_INFO, "BIND:","22222222222222" );

				//记录他的验证信息字段.其他的可以不需要记录
				memcpy(cer_recognise[cer_recognise_offset].section,&cer_store[65+offset],store_cer_section_length[0]);
				offset+=store_cer_section_length[0];
				memcpy(cer_recognise[cer_recognise_offset].expiration,&cer_store[65+offset],store_cer_section_length[1]);
				offset+=store_cer_section_length[1];
				memcpy(cer_recognise[cer_recognise_offset].authority,&cer_store[65+offset],store_cer_section_length[2]);
				offset+=store_cer_section_length[2];

				//记录证书时间
				recognise_days_left[cer_recognise_offset] = cer_expiration_days;

				//允许绑定的证书最多只能是cer_max_number本
				if(cer_recognise_offset > cer_max_number) cer_recognise_offset = 0;
				cer_recognise_offset++;

				 //记录此种证书的有效期,更新证书有效期
				 if(cer_expiration_days >cer_recognise_days_left )
					 cer_recognise_days_left = cer_expiration_days;

		break;


	//都有
	case '3':
				//记录他的验证信息字段.其他的可以不需要记录
				memcpy(cer_generate_recognise[cer_generate_recognise_offset].section,&cer_store[65+offset],store_cer_section_length[0]);
				cer_generate_recognise[cer_generate_recognise_offset].section[store_cer_section_length[0]]='\0';
				offset+=store_cer_section_length[0];

				memcpy(cer_generate_recognise[cer_generate_recognise_offset].expiration,&cer_store[65+offset],store_cer_section_length[1]);
				cer_generate_recognise[cer_generate_recognise_offset].expiration[store_cer_section_length[1]]='\0';
				offset+=store_cer_section_length[1];

				memcpy(cer_generate_recognise[cer_generate_recognise_offset].authority,&cer_store[65+offset],store_cer_section_length[2]);
				cer_generate_recognise[cer_generate_recognise_offset].authority[store_cer_section_length[2]]='\0';

				//记录证书时间
				generate_recognise_days_left[cer_generate_recognise_offset] = cer_expiration_days;

				//允许绑定的证书最多只能是cer_max_number本
				if(cer_generate_recognise_offset > cer_max_number) cer_generate_recognise_offset = 0;
				cer_generate_recognise_offset++;

				__android_log_print(ANDROID_LOG_INFO, "BIND:","33333333333333" );
				__android_log_print(ANDROID_LOG_INFO, "BIND:","MMMM=%s", cer_generate_recognise[cer_generate_recognise_offset].section );
				__android_log_print(ANDROID_LOG_INFO, "BIND:","MMMM=%s", cer_generate_recognise[cer_generate_recognise_offset].expiration);
				__android_log_print(ANDROID_LOG_INFO, "BIND:","MMMM=%s", cer_generate_recognise[cer_generate_recognise_offset].authority );

				//记录此种证书的有效期,更新证书有效期
				 if(cer_expiration_days > cer_generate_recognise_days_left )
					 cer_generate_recognise_days_left = cer_expiration_days;


		break;

	default:
				return -1;
		break;
	}
	updata_cer();
    return SUCCEED;
}
//!
//! \brief CORE::cer_unbind
//! \param input_cer
//! \param input_cer_len
//! \return
//!
int CORE::cer_unbind(char *  input_cer)
{

	char cer_store[200];
	char cer_expiration_time[11];
	int cer_genr_time_position = 0;
	int  cer_expiration_days = 0;
	int offset = 0;
	memset(cer_store,0,200);
	memset(cer_expiration_time,0,11);

	//证书信息保存到本地
	  for(int i = 0 ; i < 200 ; i++){
		  cer_store[i] = *(input_cer + i);
	  }
	__android_log_print(ANDROID_LOG_INFO, "UBIND:","cer_decrypt= %s", cer_store );


	offset =store_cer_section_length[1]+store_cer_section_length[0];


	cer_genr_time_position =store_cer_section_length[0];

	//查看证书生成日期和当前日期，防止修改本地时间，使用过期证书
	memcpy(cer_expiration_time,&cer_store[65+cer_genr_time_position],store_cer_section_length[1]);

	//日期转换
	cer_expiration_days = diff_days.get_sys_diff_days(cer_expiration_time);

	//判定当前需要绑定的证书的类型
	switch(cer_store[65+offset]){

	//去掉生成权限
	case '1':
		__android_log_print(ANDROID_LOG_INFO, "UBIND:","111111111111111" );
			//找到证书
			for(int i = 0;i < cer_max_number ;i++){

				//证书存在本地
				if(0==strncmp(cer_generate[i].section,&cer_store[65],store_cer_section_length[0])){

					//去掉他的验证信息字段
					memset(cer_generate[i].section,0,store_cer_section_length[0]);
					memset(cer_generate[i].expiration,0,store_cer_section_length[1]);
					memset(cer_generate[i].authority,0,store_cer_section_length[2]);

					//去掉时间参数
					generate_days_left[i] = 0;

					__android_log_print(ANDROID_LOG_INFO, "UBIND:","111111111111111111" );
					__android_log_print(ANDROID_LOG_INFO, "UBIND:","%s", cer_generate[i].section );
					__android_log_print(ANDROID_LOG_INFO, "UBIND:","%s", cer_generate[i].expiration);
					__android_log_print(ANDROID_LOG_INFO, "UBIND:","%s", cer_generate[i].authority );

					//去掉的证书是具有最高权限的,一定要找到其他证书时间来顶上去
					if(cer_expiration_days == cer_generate_days_left){

						cer_generate_days_left = 0;
						//遍历证书的时间,找到第二最大的时间
						for(int i = 0 ; i < cer_generate_offset;i++)
						{	//整理工作区间
							memset(cer_expiration_time,0,11);
							//找到证书字串
							memcpy(cer_expiration_time,cer_generate[i].expiration,store_cer_section_length[1]);
							//获取时间剩余
							cer_expiration_days = diff_days.get_sys_diff_days(cer_expiration_time);
							//冒泡简单找最大值
							if(cer_expiration_days >cer_generate_days_left ){
								cer_generate_days_left = cer_expiration_days;

							}
						__android_log_print(ANDROID_LOG_INFO, "UBIND====","11111=SECOND==>:%d" ,cer_generate_days_left);
						}

					}

					//去掉证书成功
					return 0;
				}
			}


		break;

	//去掉识别权限
	case '2':

			__android_log_print(ANDROID_LOG_INFO, "UBIND:","22222222222222222" );
			//找到证书
			for(int i = 0;i < cer_max_number ;i++){

				//证书存在本地
				if(0==strncmp(cer_recognise[i].section,&cer_store[65],store_cer_section_length[i])){

					//去掉他的验证信息字段
					memset(cer_recognise[i].section,0,store_cer_section_length[0]);
					memset(cer_recognise[i].expiration,0,store_cer_section_length[1]);
					memset(cer_recognise[i].authority,0,store_cer_section_length[2]);

					//去掉时间参数
					recognise_days_left[i] = 0;

					__android_log_print(ANDROID_LOG_INFO, "UBIND:","%s", cer_recognise[i].section );
					__android_log_print(ANDROID_LOG_INFO, "UBIND:","%s", cer_recognise[i].expiration);
					__android_log_print(ANDROID_LOG_INFO, "UBIND:","%s", cer_recognise[i].authority );
					//去掉的证书是具有最高权限的,一定要找到其他证书时间来顶上去
					if(cer_expiration_days == cer_generate_days_left){

						cer_generate_days_left = 0;
						//遍历证书的时间,找到第二最大的时间
						for(int i = 0 ; i < cer_recognise_offset;i++)
						{	//整理工作区间
							memset(cer_expiration_time,0,11);
							//找到证书字串
							memcpy(cer_expiration_time,cer_recognise[i].expiration,store_cer_section_length[1]);
							//获取时间剩余
							cer_expiration_days = diff_days.get_sys_diff_days(cer_expiration_time);
							//冒泡简单找最大值
							if(cer_expiration_days >cer_recognise_days_left ){
								cer_recognise_days_left = cer_expiration_days;

							}
						__android_log_print(ANDROID_LOG_INFO, "UBIND====","11111=SECOND==>:%d" ,cer_generate_days_left);
						}

					}
					//去掉证书成功
					return 0;
				}
			}
			break;


	//去掉所有
	case '3':
				__android_log_print(ANDROID_LOG_INFO, "UBIND:","3333333333333331" );
			//找到证书
			for(int i = 0;i < cer_max_number ;i++){

				//证书存在本地
				if(0==strncmp(cer_generate_recognise[i].section,&cer_store[65],store_cer_section_length[i])){

					//去掉他的验证信息字段
					memset(cer_generate_recognise[i].section,0,store_cer_section_length[0]);
					memset(cer_generate_recognise[i].expiration,0,store_cer_section_length[1]);
					memset(cer_generate_recognise[i].authority,0,store_cer_section_length[2]);

					generate_recognise_days_left[i] = 0;

					__android_log_print(ANDROID_LOG_INFO, "UBIND:","%s", cer_generate_recognise[i].section );
					__android_log_print(ANDROID_LOG_INFO, "UBIND:","%s", cer_generate_recognise[i].expiration);
					__android_log_print(ANDROID_LOG_INFO, "UBIND:","%s", cer_generate_recognise[i].authority );

					//去掉的证书是具有最高权限的,一定要找到其他证书时间来顶上去
					if(cer_expiration_days == cer_generate_days_left){

						cer_generate_days_left = 0;
						//遍历证书的时间,找到第二最大的时间
						for(int i = 0 ; i < cer_generate_recognise_offset;i++)
						{	//整理工作区间
							memset(cer_expiration_time,0,11);
							//找到证书字串
							memcpy(cer_expiration_time,cer_generate_recognise[i].expiration,store_cer_section_length[1]);
							//获取时间剩余
							cer_expiration_days = diff_days.get_sys_diff_days(cer_expiration_time);
							//冒泡简单找最大值
							if(cer_expiration_days >cer_generate_recognise_days_left ){
								cer_generate_recognise_days_left = cer_expiration_days;

							}
						__android_log_print(ANDROID_LOG_INFO, "UBIND====","11111=SECOND==>:%d" ,cer_generate_days_left);
						}

					}
					return 0;
				}
			}

		break;

	default:
			//	__android_log_print(ANDROID_LOG_INFO, "UBIND:","111111111111111" );
				return -2;
		break;
	}
    return -1;
}
//!
//! \brief CORE::cer_isbind
//! \param input_cer
//! \param input_cer_len
//! \return
//!
int CORE::cer_isbind(char *  input_cer)
{
	char cer_store[200];
	int offset = 0;
	int error = 0;
	memset(cer_store,0,200);
	int cer_authority_position = 0;
	cer_authority_position =store_cer_section_length[1]+store_cer_section_length[0];

	//证书信息保存到本地
	  for(int i = 0 ; i < 200 ; i++){
		  cer_store[i] = *(input_cer + i);
	  }
	__android_log_print(ANDROID_LOG_INFO, "BIND:","cer_decrypt= %s", cer_store );


	//判定当前需要检查的证书的类型
	switch(cer_store[65+cer_authority_position]){

	//生成权限
	case '1':

			 //找到证书
				for(int i = 0;i < cer_max_number ;i ++ ){

					//检查他的验证信息
					error = strncmp(cer_generate[i].section,&cer_store[65+offset],store_cer_section_length[0]);
					offset+=store_cer_section_length[0];
					error = strncmp(cer_generate[i].expiration,&cer_store[65+offset],store_cer_section_length[1]);
					offset+=store_cer_section_length[1];
					error = strncmp(cer_generate[i].authority,&cer_store[65+offset],store_cer_section_length[2]);

					__android_log_print(ANDROID_LOG_INFO, "BIND:","111111111111111111" );
					__android_log_print(ANDROID_LOG_INFO, "BIND:","%s", cer_generate[i].section );
					__android_log_print(ANDROID_LOG_INFO, "BIND:","%s", cer_generate[i].expiration );
					__android_log_print(ANDROID_LOG_INFO, "BIND:","%s", cer_generate[i].authority );

					//遍历完毕,找到证书
					if(!error) return 0 ;
				}
		break;

	//识别权限
	case '2':

		//找证书
				for(int i = 0;i < cer_max_number ;i ++ ){

					//检查他的验证信息
					error = strncmp(cer_recognise[i].section,&cer_store[65+offset],store_cer_section_length[0]);
					offset+=store_cer_section_length[0];
					error = strncmp(cer_recognise[i].expiration,&cer_store[65+offset],store_cer_section_length[1]);
					offset+=store_cer_section_length[1];
					error = strncmp(cer_recognise[i].authority,&cer_store[65+offset],store_cer_section_length[2]);

					__android_log_print(ANDROID_LOG_INFO, "BIND:","111111111111111111" );
					__android_log_print(ANDROID_LOG_INFO, "BIND:","%s", cer_recognise[i].section );
					__android_log_print(ANDROID_LOG_INFO, "BIND:","%s", cer_recognise[i].expiration );
					__android_log_print(ANDROID_LOG_INFO, "BIND:","%s", cer_recognise[i].authority );

					//遍历完毕,找到证书
					if(!error) return 0 ;
				}

	//所有
	case '3':

		//找证书
				for(int i = 0;i < cer_max_number ;i ++ ){

					//检查他的验证信息
					error = strncmp(cer_generate_recognise[i].section,&cer_store[65+offset],store_cer_section_length[0]);
					offset+=store_cer_section_length[0];
					error = strncmp(cer_generate_recognise[i].expiration,&cer_store[65+offset],store_cer_section_length[1]);
					offset+=store_cer_section_length[1];
					error = strncmp(cer_generate_recognise[i].authority,&cer_store[65+offset],store_cer_section_length[2]);

					__android_log_print(ANDROID_LOG_INFO, "BIND:","111111111111111111" );
					__android_log_print(ANDROID_LOG_INFO, "BIND:","%s", cer_generate_recognise[i].section );
					__android_log_print(ANDROID_LOG_INFO, "BIND:","%s", cer_generate_recognise[i].expiration );
					__android_log_print(ANDROID_LOG_INFO, "BIND:","%s", cer_generate_recognise[i].authority );

					//遍历完毕,找到证书
					if(!error) return 0 ;
				}
		break;

	default:
				return -1;
		break;
	}
    return -1;
}
//!
//! \brief CORE::cer_section_auth
//! \param input_section
//! \return 0，没有绑定； 1，绑定生成； 2，绑定识别； 3，全部绑定；
//!
int CORE::checkSectionsBindState(char *section)
{
	char cer_store[4];
	int  authority = 0;

	//证书信息保存到本地
	  for(int i = 0 ; i < 4 ; i++){
		  cer_store[i] = *(section + i);
	  }

	//证书缓冲区找断码绑定的权限
	for(int i= 0 ; i< cer_max_number; i ++){

		//通用权限
		if(0 == strncmp(cer_generate_recognise[i].section,cer_store,4)){

			//证书还未到期
			if(generate_recognise_days_left[i] >0){
				return 3;
			}
		}
		//识别权限
		if(0 == strncmp(cer_recognise[i].section,cer_store,4)){

			//证书还未到期
			if(recognise_days_left[i] > 0){
				return 2;
			}
		}
		//生成权限
		if(0 == strncmp(cer_generate[i].section,cer_store,4)){
			//证书还未到期
			if(generate_days_left[i] > 0){
				return 1;
			}
		}
	}


    return 0;
}
//!
//! \brief CORE::genr_wave
//! \param input_section
//! \param input_data
//! \param out_genr_result
//! \return
//!
short *CORE::genr_wave(char *  input_section, char *  input_data, int out_genr_result)
{
	int section = atoi(input_section);
	int data    = atoi(input_data);
	int create_data = section*10000 + data;
	out_genr_result = 1;

	return create_short_wave(create_data);
}
//!
//! \brief CORE::recog_stop
//! \return
//!
int CORE::recog_stop()
{
    stop();
}
//!
//! \brief CORE::recog_start
//! \param input_cfg
//! \return
//!
int CORE::recog_start(Recog_cfg input_cfg)
{
	//刚开机
	if(first_run == 0){

		int n_v=2048;
		double st_v=0.15;
		int gap_v=512;
		int feq_size=3;

		int sFeq_v[]={16, 12, 8, 4};//{5.5125};
		int eFeq_v[]={20, 16, 12, 8};//{11.025};

	    data_init(n_v,st_v,gap_v,sFeq_v,eFeq_v,feq_size);
		first_run = 1;
	}
	start();
}
//!
//! \brief CORE::recog_setconfig
//! \param input_cfg
//! \return
//!
int CORE::recog_setconfig(Recog_cfg input_cfg)
{


}
//!
//! \brief CORE::recog_getconfig
//! \param input_cfg
//! \return
//!
int CORE::recog_getconfig(Recog_cfg input_cfg)
{

}
//!
//! \brief CORE::recog_write
//! \param input_data
//! \param input_data_len
//! \return
//!
int CORE::recog_write(short *input_data, int input_data_len)
{
    wirte(input_data,input_data_len);
}
//!
//! \brief CORE::recog_status
//! \param output_status
//! \return
//!
int CORE::recog_status(Recog_status output_status)
{

      return 0;
}

//跟新参数,找到cer对应的最高时,或则最高权限
void CORE::updata_cer(){

	//遍历证书文件,找到文件中间时间最晚的

	//diff_days.get_input_time()
	diff_days.get_sys_time();
	__android_log_print(ANDROID_LOG_INFO, "UPDATACER:","sys_year  = %d ",diff_days.sys_year );
	__android_log_print(ANDROID_LOG_INFO, "UPDATACER:","sys_montd = %d ", diff_days.sys_month);
	__android_log_print(ANDROID_LOG_INFO, "UPDATACER:","sys_data  = %d ", diff_days.sys_day);
}
//!
//! \brief CORE::recog_status
//! \param output_status
//! \return
//!
CORE::CORE(){

	cer_generate_days_left = 0;
	cer_recognise_days_left = 0;
	cer_max_number = MAX_CER - 1;
	first_run = 0;

}
