#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

#include <string>
#include "rsa_use.h"
using namespace std;
class ENCRYPT{

private :
		rsa_use rsa;
		int encrypt_src_data_length;
		int encrypt_dst_data_length;

		char *p_encrypt_src_data;
		char *p_encrypt_dst_data;

		int random_string_length;

		int cer_data_length;//


		char *p_decrypt_data;

		char magic_code[6];				//魔术字
		int  magic_code_position;

		char  *p_cer_encrypt_data;
		char* gen_random_string(int length);
		int   put_magic_code();
		int   get_magic_code_position(char *cer_data);//获取解密后的字符串的magic为止

		//int   check_magic_code_legal();

public:
		ENCRYPT();
		~ENCRYPT();
		char* do_encrypt(char *cer_data);
		char* do_decrypt(char *encry_cer);

		void  do_test(void);
};

#endif
