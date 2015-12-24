

//#include <android/log.h>
//#define LOG_TAG "xx"
//#define LOGW(a )  __android_log_write(ANDROID_LOG_WARN,LOG_TAG,a)

#include "rsa_use.h"

//����������󲢳�ʼ��Ϊ��
rsa_use::rsa_use()
{
	char str_e_v[]="10001";
	char str_n_v[]="9A6733CBF797212EE9B0B88495D27B4EF5E100BB741DE76A5B34710292B7535F7C4C7FC9023D864E6B1B6229BA88BBD21A77BB8C85C2C3A3363074BD2FB1A04333088A01BC86F604673CE8404782AA087FE443C9689FC1EE55113BCEEBA4E6084A75CB60CA091E14F3886ABE48B1F4F59E82B1AE419BED4D25D7C2040F210DD1";
	//char str_d_v[]="67E253E941D63F0F4F500575BF4FF482F5CFC32C5C3DDA7BA68DAC136D1F474B11586F0991264FFCE6F76F9E5FD0AB7DFB2CDFD4F6124A8ABF64EEC67316719A790FF58E4B89A9744408047D5D208662596A5A2D580101E6448DFC23EAE707C8C96F346AC9B16073CB04BBB87CF9CAB5EF41CA22014CC52641C4E2DCD1002A99";

	sprintf(str_e,"%s",str_e_v);
	sprintf(str_n,"%s",str_n_v);
	//sprintf(str_d,"%s",str_d_v);
}

//�⹹��������
rsa_use::~rsa_use()
{
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
	char out_str[1024];
	printf("[test_class_func] \n");
	int type=0;
	if(type == 0)
	{
		char str0[]="89704060";
	
		char *c_data=str0;
		int c_data_size=strlen(c_data);
		char c_data_hex[100*2];
		int hex_size=0;
		data_to_16hex(c_data,c_data_size,c_data_hex,hex_size);
		//printf("[data_to_16hex] %d %s\n",hex_size,c_data_hex);
		sprintf(out_str,"[data_to_16hex] %d %s\n",hex_size,c_data_hex);
//		LOGW(out_str);
		//! rsa����
		char rsa_enc_data[1024];
		int rsa_enc_size;
		rsa_encryption(str_e,str_n,c_data_hex,hex_size,rsa_enc_data,rsa_enc_size);
		for(int i=0;i<rsa_enc_size;i++)
		{
			//printf("%02x, ",rsa_enc_data[i]);
		}
		//printf("\n[rsa_encryption] %d\n",rsa_enc_size);
		sprintf(out_str,"\n[rsa_encryption] %d\n",rsa_enc_size);
//		LOGW(out_str);
	
		//! 16����dataת��Ϊ16�����ַ���
		char enc_str[2048];
		int enc_str_size=0;
		char temp_str[8];		
		for(int i=0;i<rsa_enc_size;i++)
		{
			sprintf(temp_str,"%02x",rsa_enc_data[i]);
			memcpy(	enc_str+i*2,temp_str,2);	
			//printf("%02x, ",rsa_enc_data[i]);
		}
		enc_str_size=rsa_enc_size*2;	
		enc_str[enc_str_size]=0x00;
		printf("[hex16] %d %s\n",enc_str_size,enc_str);
		sprintf(out_str,"[hex16] %d %s\n",enc_str_size,enc_str);
//		LOGW(out_str);

		//! 16�����ַ�����ԭΪ16����data
		char dnc_str[2048];
		int dnc_str_size=enc_str_size/2;
		char temp_str2[8];
		temp_str2[2]=0x00;
		for(int i=0;i<dnc_str_size;i++)
		{
			memcpy(temp_str2,enc_str+i*2,2);
			dnc_str[i]=convert(temp_str2);
			//printf("%02x, ",dnc_str[i]);
		}
		//printf("\n[dnc_str] %d\n",rsa_enc_size);

		//! RSA ����
		char rsa_dnc_data[1024];
		int rsa_dnc_size;
		rsa_dncryption(str_d,str_n,dnc_str,dnc_str_size,rsa_dnc_data,rsa_dnc_size);
		//printf("[rsa_dncryption] %s\n ",rsa_dnc_data);
		sprintf(out_str,"[rsa_dncryption] %s\n ",rsa_dnc_data);
//		LOGW(out_str);
	
		//! ��ʮ�������ַ�����ԭΪ����
		char s_data_hex[100*2];
		int s_data_size;
		hex_to_data(rsa_dnc_data,rsa_dnc_size,s_data_hex,s_data_size);
		//printf("[hex_to_data] %d %s\n",s_data_size,s_data_hex);
		sprintf(out_str,"[hex_to_data] %d %s\n",s_data_size,s_data_hex);
//		LOGW(out_str);

	}
	if(type == 1)
	{
char str1[]="372312332392302312362352372302392442372312312392322452382442332372342332442372432382442412462442452302422432382302412372412442372392302452452312302422312322322352422432372412302442382462342392372452392372302382352452342362332302362452352382372412332382442342362452352452372452392422382332332382392322412332302372332462442392372392352362432332422312452462412342362442442342412422392302432442302332442312462332332392452352432352452422432382432342352302442442342322392432452432322342312302462432332362442422322382362302462442352312442392352312362382322332342462412322392332312462442432432372432312342332452382392352432302332442412302332412322412462442442382442352422352352422442362372382332382432302352362422412432312342342322352302372372302422372442312342452332332462312";
		//! �ַ�����ԭΪ16����

		char s_data_hex_str[1024*2];
		int s_data_size_str;
		hex_to_data(str1,strlen(str1),s_data_hex_str,s_data_size_str);
		for(int i=0;i<s_data_size_str;i++)
		{
			printf("%08x, ",s_data_hex_str[i]);
		}
		
		char *rsa_enc_data=s_data_hex_str;
		int rsa_enc_size=s_data_size_str;
		//! RSA ����
		char rsa_dnc_data[1024];
		int rsa_dnc_size;
		rsa_dncryption(str_d,str_n,rsa_enc_data,rsa_enc_size,rsa_dnc_data,rsa_dnc_size);
		printf("[rsa_dncryption] %s\n ",rsa_dnc_data);
		/*
	
		//! ��ʮ�������ַ�����ԭΪ����
		char s_data_hex[100*2];
		int s_data_size;
		hex_to_data(rsa_dnc_data,rsa_dnc_size,s_data_hex,s_data_size);
		printf("[hex_to_data] %d %s\n",s_data_size,s_data_hex);
		*/
	}
	/*
	char str1[]="33,37,30,33,45,33,38,46,39,41,32,31,45,30,46,30,31,36,35,30,32,39,37,36,44,45,35,41,38,44,35,34,42,35,31,44,46,31,41,30,34,33,35,42,42,44,44,39,43,39,34,31,32,32,33,34,30,39,41,31,42,42,35,34,33,43,45,38,44,43,38,33,43,44,43,32,46,37,34,43,42,46,39,36,31,33,35,31,43,38,42,32,36,38,41,33,41,34,43,34,34,39,38,31,46,30,44,32,30,41,37,36,36,37,44,46,31,34,44,36,38,42,42,37,43,36,41,38,45,36,37,41,30,38,38,44,42,38,32,39,36,39,31,34,38,30,37,45,33,32,41,38,44,45,30,43,30,33,34,38,43,31,32,31,34,46,34,35,30,32,45,30,31,33,33,41,42,32,34,33,46,41,45,30,41,37,34,32,38,34,43,43,35,46,30,38,37,46,39,39,34,46,45,43,43,46,34,30,33,44,37,43,36,44,46,30,46,41,37,45,36,42,33,46,35,37,43,30,31,35,36,37,33,42,44,34,41,41,36,31,46,41,33,42,44,42,46,41,30,46,43,30,44,44,33,45,";
	
	//! ���ַ�������ת��Ϊʮ�����ƣ�����󷵻ظ�java����
	char str2[3];
	char rsa_enc_data[1024];
	int rsa_enc_size=strlen(str1)/3;
	printf("len:%d %d\n",strlen(str1),rsa_enc_size);
	for(int i=0;i<rsa_enc_size;i++)
	{
		memcpy(str2,str1+i*3,2);
		str2[2]=0x00;
		//! �ַ���ת16����
		//printf("[%s,%c %c], ",str2,str1[i],str1[i+1]);
		rsa_enc_data[i]=convert(str2);
		//printf("%02x,",rsa_enc_data[i]);
	}
	
	//! RSA ����
	char rsa_dnc_data[1024];
	int rsa_dnc_size;
	rsa_dncryption(str_d,str_n,rsa_enc_data,rsa_enc_size,rsa_dnc_data,rsa_dnc_size);
	printf("[rsa_dncryption] %s\n ",rsa_dnc_data);
	*/
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
		//char str0[]="adfwqercvxbxcb";
		char out_str[1024];
		
		char *c_data=data_p;
		int c_data_size=data_size;
		char c_data_hex[1024*2];
		int hex_size=0;
		data_to_16hex(c_data,c_data_size,c_data_hex,hex_size);
		//sprintf(out_str,"[data_to_16hex] %d %s\n",hex_size,c_data_hex);
		//LOGW(out_str);
		//! rsa����
		char rsa_enc_data[1024];
		int rsa_enc_size;
		rsa_encryption(str_e,str_n,c_data_hex,hex_size,rsa_enc_data,rsa_enc_size);
		for(int i=0;i<rsa_enc_size;i++)
		{
			//printf("%02x, ",rsa_enc_data[i]);
		}
		//sprintf(out_str,"\n[rsa_encryption] %d\n",rsa_enc_size);
		//LOGW(out_str);
		//! 16����dataת��Ϊ16�����ַ���
		char enc_str[2048];
		int enc_str_size=0;
		char temp_str[8];		
		for(int i=0;i<rsa_enc_size;i++)
		{
			sprintf(temp_str,"%02x",rsa_enc_data[i]);
			memcpy(	enc_str+i*2,temp_str,2);	
			//printf("%02x, ",rsa_enc_data[i]);
		}
		enc_str_size=rsa_enc_size*2;	
		enc_str[enc_str_size]=0x00;
		//sprintf(out_str,"[hex16] %d %s\n",enc_str_size,enc_str);	
		//LOGW(out_str);
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
		sprintf(hex_buf_temp,"%02X",in[i]);
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

