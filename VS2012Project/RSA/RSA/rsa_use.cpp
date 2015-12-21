

//#include <android/log.h>
//#define LOG_TAG "xx"
//#define LOGW(a )  __android_log_write(ANDROID_LOG_WARN,LOG_TAG,a)
#include "stdafx.h"

#include "rsa_use.h"
#include <stdio.h>


rsa_use::rsa_use()
{
	//公钥指数
	char str_e_v[]="10001";
	//模值
	char str_n_v[]="9A6733CBF797212EE9B0B88495D27B4EF5E100BB741DE76A5B34710292B7535F7C4C7FC9023D864E6B1B6229BA88BBD21A77BB8C85C2C3A3363074BD2FB1A04333088A01BC86F604673CE8404782AA087FE443C9689FC1EE55113BCEEBA4E6084A75CB60CA091E14F3886ABE48B1F4F59E82B1AE419BED4D25D7C2040F210DD1";
	//私钥指数
	char str_d_v[]="67E253E941D63F0F4F500575BF4FF482F5CFC32C5C3DDA7BA68DAC136D1F474B11586F0991264FFCE6F76F9E5FD0AB7DFB2CDFD4F6124A8ABF64EEC67316719A790FF58E4B89A9744408047D5D208662596A5A2D580101E6448DFC23EAE707C8C96F346AC9B16073CB04BBB87CF9CAB5EF41CA22014CC52641C4E2DCD1002A99";

	sprintf_s(str_e,"%s",str_e_v);
	sprintf_s(str_n,"%s",str_n_v);
	sprintf_s(str_d, "%s",str_d_v);
}

//解构大数对象
rsa_use::~rsa_use()
{

}

int rsa_use::test_rsa(){


	char inputstring1[124];//1024要加密的字符串

	for (int i = 0; i < 124; i++){
		inputstring1[i] = 'q';
	}

	char outdata[256];//输出要存放的字符位置


	char outputstring1[1024] = {0};//加密数据缓冲区

	int outsize;

	
	int outdatalength = 256;

	rsa_enc_str(inputstring1, 124, outputstring1, outsize);



	rsa_dnc_str(outputstring1, outsize, outdata, outdatalength);
	rsa_dnc_str(outputstring1, outsize, outdata, outdatalength);
	rsa_dnc_str(outputstring1, outsize, outdata, outdatalength);
	printf("数据长度%d", outdatalength);
	printf("加密124个%s", outdata);
	return 0;

}
/******************************************************************************/
//	名称：test_class_func
//	功能：test_class_func
//	参数：
//	返回：
//	备注：

/******************************************************************************/
int rsa_use::test_class_func()
{
	//加解密之后输出
	char out_str[2048];
		

	char str0[] ="aaaaaaaaaa";
	printf("[测试加解密过程]源字符串%s \n", str0);

	char *c_data=str0;//字符串指针
	
	int c_data_size=strlen(c_data);//测试字符串长度

	char c_data_hex[2048];

	int hex_size=0;

	//输入的字符串转换成16进制数
	data_to_16hex(c_data,      //输入字符串
				  c_data_size, //字符串数量
				  c_data_hex,  //转换后数据存放点
				  hex_size);   //转换后长度

	printf("[转换成16进制后的数长度%d内容为\n%s\n",hex_size,c_data_hex);

	//! rsa加密
	char rsa_enc_data[2048];//加密后的数据存放点

	int rsa_enc_size;//加密后数据大小存放点

	rsa_encryption(str_e,   //公钥指数
					str_n,	//公钥
					c_data_hex,//加密原文,被转换成16进制
					hex_size,  //16进制数据量
					rsa_enc_data,//加密后内容
					rsa_enc_size);//数据大小

	printf("[rsa_encryption] %d\n",rsa_enc_size);


	
	//! 16进制data转换为字符串
	char enc_str[2048];
	int enc_str_size=0;
	char temp_str[2048];		
	for(int i=0;i<rsa_enc_size;i++)
	{
		sprintf_s(temp_str, "%02x", rsa_enc_data[i]);
		memcpy(	enc_str+i*2,temp_str,2);	
	}
	enc_str_size=rsa_enc_size*2;	
	enc_str[enc_str_size]=0x00;

	printf("[hex16] %d %s\n",enc_str_size,enc_str);


	//! 16进制字符串还原为16进制data
	char dnc_str[2048];
	int dnc_str_size=enc_str_size/2;
	char temp_str2[2048];
	temp_str2[2]=0x00;
	for(int i=0;i<dnc_str_size;i++)
	{
		memcpy(temp_str2,enc_str+i*2,2);
		dnc_str[i]=convert(temp_str2);
		//printf("%02x, ",dnc_str[i]);
	}
	//printf("\n[dnc_str] %d\n",rsa_enc_size);

	//! RSA 解密
	char rsa_dnc_data[2048];
	int rsa_dnc_size;

	rsa_dncryption(str_d,
				   str_n,
				   dnc_str,
				   dnc_str_size,
				   rsa_dnc_data,
				   rsa_dnc_size);

	printf("[rsa_dncryption] %s\n ",rsa_dnc_data);

	sprintf_s(out_str, "[rsa_dncryption] %s\n ", rsa_dnc_data);
//		LOGW(out_str);
	
	//! 将十六进制字符串还原为数据
	char s_data_hex[2048];
	int s_data_size;
	hex_to_data(rsa_dnc_data,rsa_dnc_size,s_data_hex,s_data_size);
	
	sprintf_s(out_str, "%d %s\n", s_data_size, s_data_hex);
	printf("out_str = %s\n", out_str);


return 0; 
}
/******************************************************************************/
//	名称：convert
//	功能：字符串转16进制
//	参数：
//	返回：
//	备注：

/******************************************************************************/
unsigned long rsa_use::convert(char* str)
{
	unsigned long var=0;
	unsigned long t;
	int len = strlen(str);

	if (var > 8) //最长8位
	return -1;
	//strupr(str);//统一大写
	for (; *str; str++)
	{
	if (*str>='A' && *str <='F')
	t = *str-55;//a-f之间的ascii与对应数值相差55如'A'为65,65-55即为A
	else
	t = *str-48;
	var<<=4;
	var|=t;
	}
	return var;
} 


/******************************************************************************/
//	名称：rsa_encryption
//	功能：rsa_encryption
//	参数：
//	返回：
//	备注：rsa 公匙加密

/******************************************************************************/
int rsa_use::rsa_encryption(char *e,char *n,char *in,int in_size,char *out,int &out_size)
{
	//! 存入rsa的EN，加密，输出
	big_int P,Q,N,D,E;
	E.Get_c(e,(int)strlen(e));
	N.Get_c(n,(int)strlen(n));
	//! RSA加密
	P.Get_c(in,in_size);
	big_int bi=P.RsaTrans(E,N);
	Q.Mov(bi);
	int out_size_var=0;
	Q.Put_c(out,&out_size_var);
	out_size=out_size_var;
	out[out_size]=0x00;
	/*
	printf("[rsa_encryption] %d->%d\n",in_size,out_size);
	for(int i=0;i<out_size;i++)
	{
		printf("%02x, ",out[i]);
	}
	printf("\n");*/

	return 0;
}

/******************************************************************************/
//	名称：rsa_dncryption
//	功能：rsa_dncryption
//	参数：
//	返回：
//	备注：rsa 私匙解密

/******************************************************************************/
int rsa_use::rsa_dncryption(char *d,char *n,char *in,int in_size,char *out,int &out_size)
{
	//! RSA 解密 aes data
	big_int P,Q,N,D,E;
	D.Get_c(d,(int)strlen(d));
	N.Get_c(n,(int)strlen(n));
	Q.Get_c(in,in_size);
	big_int bi=Q.RsaTrans(D,N);
	P.Mov(bi);
	int out_size_var=0;
	P.Put_c(out,&out_size_var);
	out_size=out_size_var;
	out[out_size]=0x00;
	//printf("[rsa_dncryption] %d->%d\n%s\n",in_size,out_size_var,out);
	return 0;
}

/******************************************************************************/
//	名称：test_class_func
//	功能：rsa解密
//	参数：
//	返回：
//	备注：

/******************************************************************************/
int rsa_use::rsa_dnc_str(char *data_p,int data_size,char *out,int &out_size)
{
	//! 16进制字符串还原为16进制data
	char dnc_str[2048];
	int dnc_str_size=data_size/2;
	char temp_str2[8];
	temp_str2[2]=0x00;
	for(int i=0;i<dnc_str_size;i++)
	{
		memcpy(temp_str2,data_p+i*2,2);
		dnc_str[i]=convert(temp_str2);
		//printf("%02x, ",dnc_str[i]);
	}
	//printf("\n[dnc_str] %d\n",rsa_enc_size);

	//! RSA 解密
	char rsa_dnc_data[1024];
	int rsa_dnc_size;
	rsa_dncryption(str_d,str_n,dnc_str,dnc_str_size,rsa_dnc_data,rsa_dnc_size);
	//printf("[rsa_dncryption] %s\n ",rsa_dnc_data);

	//! 将十六进制字符串还原为数据
	char s_data_hex[1024*2];
	int s_data_size;
	hex_to_data(rsa_dnc_data,rsa_dnc_size,s_data_hex,s_data_size);
	//printf("[hex_to_data] %d %s\n",s_data_size,s_data_hex);
	
	memcpy(out,s_data_hex,s_data_size);
	out_size=s_data_size;
	out[out_size]=0x00;

	return 0;
}

/******************************************************************************/
//	名称：rsa_enc_str
//	功能：rsa加密
//	参数：
//	返回：
//	备注：

/******************************************************************************/
int rsa_use::rsa_enc_str(char *data_p,int data_size,char *out,int &out_size)
{

		char out_str[1024];
		
		char *c_data=data_p;
		int c_data_size=data_size;
		char c_data_hex[1024*2];
		int hex_size=0;
		data_to_16hex(c_data,c_data_size,c_data_hex,hex_size);

		//! rsa加密
		char rsa_enc_data[1024];
		int rsa_enc_size;
		rsa_encryption(str_e,str_n,c_data_hex,hex_size,rsa_enc_data,rsa_enc_size);


		char enc_str[2048];
		int enc_str_size=0;
		char temp_str[8];		
		for(int i=0;i<rsa_enc_size;i++)
		{
			sprintf_s(temp_str, "%02x", rsa_enc_data[i]);
			memcpy(	enc_str+i*2,temp_str,2);	

		}
		enc_str_size=rsa_enc_size*2;	
		enc_str[enc_str_size]=0x00;

		out_size=enc_str_size;
		memcpy(out,enc_str,out_size);
		out[out_size]=0x00;
}

/******************************************************************************/
//	名称：data_to_16hex
//	功能：data_to_16hex
//	参数：
//	返回：
//	备注：数据转换为16进制,每个字符专为%02X
/******************************************************************************/
int rsa_use::data_to_16hex(char *in,int in_size,char *out,int &out_size)
{
	int hex_size=0;
	char hex_buf_temp[8];
	for(int i=0;i<in_size;i++)
	{
		sprintf_s(hex_buf_temp, "%02X", in[i]);
		memcpy(out+i*2,hex_buf_temp,2);
	}
	hex_size+=in_size*2;
	out_size=hex_size;
	out[out_size]=0x00;
	//printf("[data_to_16hex] %d %s\n",out_size,out);
	return 0;
}

/******************************************************************************/
//	名称：16hex_to_data
//	功能：16hex_to_data
//	参数：
//	返回：
//	备注：16进制转换为数据,每个字符专为%02X

/******************************************************************************/
int rsa_use::hex_to_data(char *in,int in_size,char *out,int &out_size)
{
	int s_data_size;
	char temp_c[8];
	temp_c[2]=0x00;
	for(int i=0;i<in_size;i++)
	{
		memcpy(temp_c,in+i*2,2);
		out[i]=(char)convert(temp_c);//str_to_hex(temp_c);
	}
	s_data_size=in_size/2;
	out_size=s_data_size;
	out[out_size]=0x00;
	//printf("[_to_data] %d %s\n%s\n",out_size,out,out+32);
	return 0;
}

