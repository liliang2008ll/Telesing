
#ifndef __hash_h
#define __hash_h
#include <stdio.h>
#include <iostream>
using namespace std;



typedef struct _MPQHASHTABLE {
	char bExists;
} MPQHASHTABLE;


class HashForCER {

		#define nTableSize  1000       //1000个
		#define nMaxStrLen  20         //最大字符长度
	private:
		unsigned long cryptTable[0x1000];
		MPQHASHTABLE HashTable[nTableSize];
		int HashATable[nTableSize];
		int HashBTable[nTableSize];
		char data[nTableSize][nMaxStrLen];
		unsigned long hash_string( char *lpszFileName, unsigned long dwHashType );

	public:
		int  do_insert_string( char *string_in);
		int  do_search_string( char *string_in);
		void do_prepare_crypt_table(void);
		void do_test_hash(void);
};

#endif
