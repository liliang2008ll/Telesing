
#ifndef RSA_USE

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "big_int.h"

//1024数据加密算法
class rsa_use
{
	char str_e[1024]; //指数
	char str_n[1024]; //公钥
	char str_d[1024]; //私钥

public:
	rsa_use();
	~rsa_use();
	int test_rsa();
	int test_class_func();
	unsigned long  convert(char *str);
	int rsa_encryption(char *e,char *n,char *in,int in_size,char *out,int &out_size);
	int rsa_dncryption(char *d,char *n,char *in,int in_size,char *out,int &out_size);	
	int rsa_dnc_str(char *data_p,int data_size,char *out,int &out_size);
	int data_to_16hex(char *in,int in_size,char *out,int &out_size);
	int hex_to_data(char *in,int in_size,char *out,int &out_size);
	int rsa_enc_str(char *data_p,int data_size,char *out,int &out_size);
};

#endif
