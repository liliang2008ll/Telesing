/*****************************************************************
 大数运算库头文件：big_int.h
 作者：afanty@vip.sina.com
 版本：1.2 (2003.5.13)
 说明：适用于MFC，1024位RSA运算
 *****************************************************************/

#ifndef BI_MAXLEN
#
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

class big_int
{

//允许生成1120位（二进制）的中间结果
#define BI_MAXLEN 35
#define DEC 10
#define HEX 16

public:
//大数在0x100000000进制下的长度    
	unsigned m_nLength;
//用数组记录大数在0x100000000进制下每一位的值
	unsigned long m_ulValue[BI_MAXLEN];

	big_int();
	~big_int();

	/*****************************************************************
	 基本操作与运算
	 Mov，赋值运算，可赋值为大数或普通整数，可重载为运算符“=”
	 Cmp，比较运算，可重载为运算符“==”、“!=”、“>=”、“<=”等
	 Add，加，求大数与大数或大数与普通整数的和，可重载为运算符“+”
	 Sub，减，求大数与大数或大数与普通整数的差，可重载为运算符“-”
	 Mul，乘，求大数与大数或大数与普通整数的积，可重载为运算符“*”
	 Div，除，求大数与大数或大数与普通整数的商，可重载为运算符“/”
	 Mod，模，求大数与大数或大数与普通整数的模，可重载为运算符“%”
	 *****************************************************************/
	//void Mov(unsigned __int64 A);
	void Mov(unsigned long long A);
	void Mov(big_int& A);
	big_int Add(big_int& A);
	big_int Sub(big_int& A);
	big_int Mul(big_int& A);
	big_int Div(big_int& A);
	big_int Mod(big_int& A);
	big_int Add(unsigned long A);
	big_int Sub(unsigned long A);
	big_int Mul(unsigned long A);
	big_int Div(unsigned long A);
	unsigned long Mod(unsigned long A);
	int Cmp(big_int& A);

	/*****************************************************************
	 输入输出
	 Get，从字符串按10进制或16进制格式输入到大数
	 Put，将大数按10进制或16进制格式输出到字符串
	 *****************************************************************/
	//void Get(CString& str, unsigned int system=HEX);
	//void Put(CString& str, unsigned int system=HEX);

	void Get_c(char *str, int str_size, unsigned int system = HEX);
	void Put_c(char *str, int *str_size, unsigned int system = HEX);

	/*****************************************************************
	 RSA相关运算
	 Rab，拉宾米勒算法进行素数测试
	 Euc，欧几里德算法求解同余方程
	 RsaTrans，反复平方算法进行幂模运算
	 GetPrime，产生指定长度的随机大素数
	 *****************************************************************/
	int Rab();
	big_int Euc(big_int& A);
	big_int RsaTrans(big_int& A, big_int& B);
	void GetPrime(int bits);
};
#endif
