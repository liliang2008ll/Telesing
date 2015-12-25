
#include <stdio.h>
#include <string.h>
#include "hash.h"
/*按照dwHashType定义的类型取得字符串lpszFileName的各项hash值*/
unsigned long HashForCER::hash_string(char *lpszFileName,
	unsigned long dwHashType) {
	unsigned char *key = (unsigned char *) lpszFileName;
	unsigned long seed1 = 0x7FED7FED;
	unsigned long seed2 = 0xEEEEEEEE;
	int ch;

	while (*key != 0) {
		ch = *key++;
		seed1 = cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}

	return seed1;
}
/*计算lpszString字符串经过解析后应该放入或者查询各表的数据*/
int HashForCER::do_search_string(char *string_in) {
	const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2; //定义哈希类型

	unsigned int nHash = hash_string(string_in, HASH_OFFSET); //生成要放入hash表中的数据
	unsigned int nHashA = hash_string(string_in, HASH_A); //要放入hashA表中的数据
	unsigned int nHashB = hash_string(string_in, HASH_B); //要放入hashB表中的数据
	unsigned int nHashStart = nHash % nTableSize; //计算起始查询位置
	unsigned int nHashPos = nHashStart; //初始化实际查询到的位置

	while (HashTable[nHashPos].bExists) //发生了碰撞
	{
		//校验碰撞原因
		if (HashATable[nHashPos] == nHashA && HashBTable[nHashPos] == nHashB)
			break; //原始数据发生了重复，可能的情况：1.发生了错误，2.我们执行的是查询
		else
			nHashPos = (nHashPos + 1) % nTableSize; //说明数据没有重复，但原来的位置上有数据，也就是发生了碰撞的情况，所以将实际位置偏移

		if (nHashPos == nHashStart)
			cout << "字符串" << string_in << "不在表中" << endl;
		return 0;
	}
	if (strlen(data[nHashPos])) {
		cout << "字符串" << string_in << "在表中" << nHashPos << endl;
		return nHashPos;
	} else

		cout << "字符串" << string_in << "不在表中" << endl;

}
//
//
///*生成密码表*/
//
//
void HashForCER::do_prepare_crypt_table(void) {
	unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;

	for (index1 = 0; index1 < 0x100; index1++) {
		for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100) {
			unsigned long temp1, temp2;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp1 = (seed & 0xFFFF) << 0x10;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp2 = (seed & 0xFFFF);
			cryptTable[index2] = (temp1 | temp2);
		}
	}
}
int HashForCER::do_insert_string( char *string_in) {
	const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2; //定义哈希类型
	/*计算lpszString字符串经过解析后应该放入或者查询各表的数据*/
	unsigned int nHash = hash_string(string_in, HASH_OFFSET); //生成要放入hash表中的数据
	unsigned int nHashA = hash_string(string_in, HASH_A); //要放入hashA表中的数据
	unsigned int nHashB = hash_string(string_in, HASH_B); //要放入hashB表中的数据
	unsigned int nHashStart = nHash % nTableSize; //计算起始查询位置
	unsigned int nHashPos = nHashStart; //初始化实际查询到的位置

	while (HashTable[nHashPos].bExists) //发生了碰撞
	{
//校验碰撞原因
		if (HashATable[nHashPos] == nHashA && HashBTable[nHashPos] == nHashB)
			break; //原始数据发生了重复，可能的情况：1.发生了错误，2.我们执行的是查询
		else
			nHashPos = (nHashPos + 1) % nTableSize; //说明数据没有重复，但原来的位置上有数据，也就是发生了碰撞的情况，所以将实际位置偏移

		if (nHashPos == nHashStart) {
			cout << "表已满，无法插入数据" << endl;
			return 0;
		}
	}
	/*插入的情况*/
	if (!HashTable[nHashPos].bExists && (strlen(string_in) < nMaxStrLen)) {
		HashATable[nHashPos] = nHashA;
		HashBTable[nHashPos] = nHashB;
		strcpy(data[nHashPos], string_in);
		HashTable[nHashPos].bExists = 1;
		cout << "字符串" << string_in << "插入成功，位于" << nHashPos << endl;
	} else {
		if (HashTable[nHashPos].bExists)
			cout << "字符串" << string_in << "已存在于表中，无法插入" << endl;
		else
			cout << "字符串" << string_in << "长度超标，无法插入" << endl;
	}
	return nHashPos;

}

//测试HASH函数
void HashForCER::do_test_hash(void) {

	char str1[]="100032012-12-12";
	char str2[]="100032012-12-13";
	char str3[]="100032012-12-14";
	char str4[]="100032012-12-15";
	char str5[]="100032012-12-16";

	char str6[]="100032012-12-17";

	do_prepare_crypt_table();
	do_insert_string(str1);
	do_insert_string(str2);
	do_insert_string(str3);
	do_insert_string(str4);
	do_insert_string(str5);

	do_search_string(str1);
	do_search_string(str2);
	do_search_string(str3);
	do_search_string(str4);
	do_search_string(str5);
	do_search_string(str6);

}












