/*****************************************************************
 ���������ͷ�ļ���big_int.h
 ���ߣ�afanty@vip.sina.com
 �汾��1.2 (2003.5.13)
 ˵����������MFC��1024λRSA����
 *****************************************************************/

#ifndef BI_MAXLEN
#
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

class big_int
{

//��������1120λ�������ƣ����м���
#define BI_MAXLEN 35
#define DEC 10
#define HEX 16

public:
//������0x100000000�����µĳ���    
	unsigned m_nLength;
//�������¼������0x100000000������ÿһλ��ֵ
	unsigned long m_ulValue[BI_MAXLEN];

	big_int();
	~big_int();

	/*****************************************************************
	 ��������������
	 Mov����ֵ���㣬�ɸ�ֵΪ��������ͨ������������Ϊ�������=��
	 Cmp���Ƚ����㣬������Ϊ�������==������!=������>=������<=����
	 Add���ӣ��������������������ͨ�����ĺͣ�������Ϊ�������+��
	 Sub�������������������������ͨ�����Ĳ������Ϊ�������-��
	 Mul���ˣ��������������������ͨ�����Ļ���������Ϊ�������*��
	 Div�������������������������ͨ�������̣�������Ϊ�������/��
	 Mod��ģ���������������������ͨ������ģ��������Ϊ�������%��
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
	 �������
	 Get�����ַ�����10���ƻ�16���Ƹ�ʽ���뵽����
	 Put����������10���ƻ�16���Ƹ�ʽ������ַ���
	 *****************************************************************/
	//void Get(CString& str, unsigned int system=HEX);
	//void Put(CString& str, unsigned int system=HEX);

	void Get_c(char *str, int str_size, unsigned int system = HEX);
	void Put_c(char *str, int *str_size, unsigned int system = HEX);

	/*****************************************************************
	 RSA�������
	 Rab�����������㷨������������
	 Euc��ŷ������㷨���ͬ�෽��
	 RsaTrans������ƽ���㷨������ģ����
	 GetPrime������ָ�����ȵ����������
	 *****************************************************************/
	int Rab();
	big_int Euc(big_int& A);
	big_int RsaTrans(big_int& A, big_int& B);
	void GetPrime(int bits);
};
#endif
