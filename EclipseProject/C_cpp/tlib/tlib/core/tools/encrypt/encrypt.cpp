#include <time.h>
#include "encrypt.h"

//***************************************************************************
//函数名称：产生长度为length的随机字符串
//函数功能：
//输入参数：
//输出参数： 无
//备	        注 ：
//
//**************************************************************************
 char* ENCRYPT::gen_random_string(int length)
{
	int flag, i;
	char* string;
	srand((unsigned) time(NULL ));
	if ((string = (char*) malloc(length)) == NULL )
	{
		return NULL ;
	}

	for (i = 0; i < length - 1; i++)
	{
		flag = rand() % 3;
		switch (flag)
		{
			case 0:
				string[i] = 'A' + rand() % 26;
				break;
			case 1:
				string[i] = 'a' + rand() % 26;
				break;
			case 2:
				string[i] = '0' + rand() % 10;
				break;
			default:
				string[i] = 'x';
				break;
		}
	}
	string[length - 1] = '\0';
	return string;
}
 //
 //证书加密
 //
 //
 char* ENCRYPT::do_encrypt(char *cer_data){

	 //获取证书数据长度
	 cer_data_length = strlen(cer_data);

	 //随机产生encrypt_data_length个字符串
	 p_encrypt_src_data = gen_random_string(encrypt_src_data_length);

	 //计算魔术字位置
	 magic_code_position = encrypt_src_data_length - cer_data_length;

	 //魔术字节位置放在证书的头
	 sprintf(p_encrypt_src_data, "%03d", magic_code_position); //

	 //加入魔术字节
	 memcpy((p_encrypt_src_data + magic_code_position) ,magic_code,sizeof(magic_code));

	 //加入证书信息
	 memcpy((p_encrypt_src_data + magic_code_position +sizeof(magic_code)),cer_data,cer_data_length);

	 //执行上半截和下半截加
	 rsa.rsa_enc_str(p_encrypt_src_data,100,p_encrypt_dst_data,encrypt_dst_data_length);

//	 rsa.rsa_enc_str( (p_encrypt_src_data + ( encrypt_src_data_length >>1)), encrypt_src_data_length >>1,
//					  (p_encrypt_dst_data +   encrypt_dst_data_length)     , encrypt_dst_data_length);

	 return p_encrypt_dst_data;
 }

void ENCRYPT::do_test(void){

		char cer[]="11111111112222222222333333333344444444445555555555666666666677777777778888888888";
		char string[2048];
		int length;
		memset(string,0,1024);
		int cer_length = strlen(cer);
		rsa.test_class_func();
	}
 ENCRYPT::ENCRYPT(){

	 //开辟工作区间
	 encrypt_src_data_length = 200;
	 encrypt_dst_data_length = 2048;

	 p_encrypt_src_data = new char[encrypt_src_data_length];
	 p_encrypt_dst_data = new char[encrypt_dst_data_length];

	 //p_cer_encrypt_data = new char[]
	 //初始化魔术字
	 char string[] = "QUEYIN";
	 for(int i = 0 ; i < strlen(string);i++){
		 magic_code[i] = string[i];
	 }
}
 ENCRYPT::~ENCRYPT(){
	 delete [] p_encrypt_src_data;
	 delete [] p_encrypt_dst_data;
}
