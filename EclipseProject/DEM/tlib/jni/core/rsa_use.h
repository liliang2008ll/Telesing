
#ifndef RSA_USE

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "big_int.h"


#define RSA_2048 2048


//RSA���ݼ����㷨
class rsa_use
{

char str_e[1024];
char str_n[1024];
char str_d[1024];

public:
	rsa_use();
	~rsa_use();

	//���Լӽ��ܹ���
	int test_class_func();
	//ִ�м����㷨
	int rsa_encryption(char *e,char *n,char *in,int in_size,char *out,int &out_size);
	//ִ�н����㷨
	int rsa_dncryption(char *d,char *n,char *in,int in_size,char *out,int &out_size);

	unsigned long  convert(char *str);
	int rsa_dnc_str(char *data_p,int data_size,char *out,int &out_size);
	int data_to_16hex(char *in,int in_size,char *out,int &out_size);
	int hex_to_data(char *in,int in_size,char *out,int &out_size);
	int rsa_enc_str(char *data_p,int data_size,char *out,int &out_size);
};

#endif