

//#include <android/log.h>
//#define LOG_TAG "xx"
//#define LOGW(a )  __android_log_write(ANDROID_LOG_WARN,LOG_TAG,a)
#include "stdafx.h"

#include "rsa_use.h"
#include <stdio.h>


rsa_use::rsa_use()
{
	//��Կָ��
	char str_e_v[]="10001";
	//ģֵ
	char str_n_v[]="9A6733CBF797212EE9B0B88495D27B4EF5E100BB741DE76A5B34710292B7535F7C4C7FC9023D864E6B1B6229BA88BBD21A77BB8C85C2C3A3363074BD2FB1A04333088A01BC86F604673CE8404782AA087FE443C9689FC1EE55113BCEEBA4E6084A75CB60CA091E14F3886ABE48B1F4F59E82B1AE419BED4D25D7C2040F210DD1";
	//˽Կָ��
	char str_d_v[]="67E253E941D63F0F4F500575BF4FF482F5CFC32C5C3DDA7BA68DAC136D1F474B11586F0991264FFCE6F76F9E5FD0AB7DFB2CDFD4F6124A8ABF64EEC67316719A790FF58E4B89A9744408047D5D208662596A5A2D580101E6448DFC23EAE707C8C96F346AC9B16073CB04BBB87CF9CAB5EF41CA22014CC52641C4E2DCD1002A99";

	sprintf_s(str_e,"%s",str_e_v);
	sprintf_s(str_n,"%s",str_n_v);
	sprintf_s(str_d, "%s",str_d_v);
}

//�⹹��������
rsa_use::~rsa_use()
{

}

int rsa_use::test_rsa(){


	char inputstring1[124];//1024Ҫ���ܵ��ַ���

	for (int i = 0; i < 124; i++){
		inputstring1[i] = 'q';
	}

	char outdata[256];//���Ҫ��ŵ��ַ�λ��


	char outputstring1[1024] = {0};//�������ݻ�����

	int outsize;

	
	int outdatalength = 256;

	rsa_enc_str(inputstring1, 124, outputstring1, outsize);



	rsa_dnc_str(outputstring1, outsize, outdata, outdatalength);
	rsa_dnc_str(outputstring1, outsize, outdata, outdatalength);
	rsa_dnc_str(outputstring1, outsize, outdata, outdatalength);
	printf("���ݳ���%d", outdatalength);
	printf("����124��%s", outdata);
	return 0;

}
/******************************************************************************/
//	���ƣ�test_class_func
//	���ܣ�test_class_func
//	������
//	���أ�
//	��ע��

/******************************************************************************/
int rsa_use::test_class_func()
{
	//�ӽ���֮�����
	char out_str[2048];
		

	char str0[] ="aaaaaaaaaa";
	printf("[���Լӽ��ܹ���]Դ�ַ���%s \n", str0);

	char *c_data=str0;//�ַ���ָ��
	
	int c_data_size=strlen(c_data);//�����ַ�������

	char c_data_hex[2048];

	int hex_size=0;

	//������ַ���ת����16������
	data_to_16hex(c_data,      //�����ַ���
				  c_data_size, //�ַ�������
				  c_data_hex,  //ת�������ݴ�ŵ�
				  hex_size);   //ת���󳤶�

	printf("[ת����16���ƺ��������%d����Ϊ\n%s\n",hex_size,c_data_hex);

	//! rsa����
	char rsa_enc_data[2048];//���ܺ�����ݴ�ŵ�

	int rsa_enc_size;//���ܺ����ݴ�С��ŵ�

	rsa_encryption(str_e,   //��Կָ��
					str_n,	//��Կ
					c_data_hex,//����ԭ��,��ת����16����
					hex_size,  //16����������
					rsa_enc_data,//���ܺ�����
					rsa_enc_size);//���ݴ�С

	printf("[rsa_encryption] %d\n",rsa_enc_size);


	
	//! 16����dataת��Ϊ�ַ���
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


	//! 16�����ַ�����ԭΪ16����data
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

	//! RSA ����
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
	
	//! ��ʮ�������ַ�����ԭΪ����
	char s_data_hex[2048];
	int s_data_size;
	hex_to_data(rsa_dnc_data,rsa_dnc_size,s_data_hex,s_data_size);
	
	sprintf_s(out_str, "%d %s\n", s_data_size, s_data_hex);
	printf("out_str = %s\n", out_str);


return 0; 
}
/******************************************************************************/
//	���ƣ�convert
//	���ܣ��ַ���ת16����
//	������
//	���أ�
//	��ע��

/******************************************************************************/
unsigned long rsa_use::convert(char* str)
{
	unsigned long var=0;
	unsigned long t;
	int len = strlen(str);

	if (var > 8) //�8λ
	return -1;
	//strupr(str);//ͳһ��д
	for (; *str; str++)
	{
	if (*str>='A' && *str <='F')
	t = *str-55;//a-f֮���ascii���Ӧ��ֵ���55��'A'Ϊ65,65-55��ΪA
	else
	t = *str-48;
	var<<=4;
	var|=t;
	}
	return var;
} 


/******************************************************************************/
//	���ƣ�rsa_encryption
//	���ܣ�rsa_encryption
//	������
//	���أ�
//	��ע��rsa ���׼���

/******************************************************************************/
int rsa_use::rsa_encryption(char *e,char *n,char *in,int in_size,char *out,int &out_size)
{
	//! ����rsa��EN�����ܣ����
	big_int P,Q,N,D,E;
	E.Get_c(e,(int)strlen(e));
	N.Get_c(n,(int)strlen(n));
	//! RSA����
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
//	���ƣ�rsa_dncryption
//	���ܣ�rsa_dncryption
//	������
//	���أ�
//	��ע��rsa ˽�׽���

/******************************************************************************/
int rsa_use::rsa_dncryption(char *d,char *n,char *in,int in_size,char *out,int &out_size)
{
	//! RSA ���� aes data
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
//	���ƣ�test_class_func
//	���ܣ�rsa����
//	������
//	���أ�
//	��ע��

/******************************************************************************/
int rsa_use::rsa_dnc_str(char *data_p,int data_size,char *out,int &out_size)
{
	//! 16�����ַ�����ԭΪ16����data
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

	//! RSA ����
	char rsa_dnc_data[1024];
	int rsa_dnc_size;
	rsa_dncryption(str_d,str_n,dnc_str,dnc_str_size,rsa_dnc_data,rsa_dnc_size);
	//printf("[rsa_dncryption] %s\n ",rsa_dnc_data);

	//! ��ʮ�������ַ�����ԭΪ����
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
//	���ƣ�rsa_enc_str
//	���ܣ�rsa����
//	������
//	���أ�
//	��ע��

/******************************************************************************/
int rsa_use::rsa_enc_str(char *data_p,int data_size,char *out,int &out_size)
{

		char out_str[1024];
		
		char *c_data=data_p;
		int c_data_size=data_size;
		char c_data_hex[1024*2];
		int hex_size=0;
		data_to_16hex(c_data,c_data_size,c_data_hex,hex_size);

		//! rsa����
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
//	���ƣ�data_to_16hex
//	���ܣ�data_to_16hex
//	������
//	���أ�
//	��ע������ת��Ϊ16����,ÿ���ַ�רΪ%02X
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
//	���ƣ�16hex_to_data
//	���ܣ�16hex_to_data
//	������
//	���أ�
//	��ע��16����ת��Ϊ����,ÿ���ַ�רΪ%02X

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

