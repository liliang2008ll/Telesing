// RSAKEY.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "string.h"
//ָ������,������Ž��յ��Ľ��ܺ���ַ�
char store_cer_section_length[] = {  4, 10, 1, 10, 10, 19, 1, 15, 64 };

char store_cer_string[9][66] = { "4444 ",      //���
							 "YYYY-MM-DD ",//ʧЧ����    YYYY-MM-DD
							 "1 ",          //Ȩ��       ��1������Ȩ��  ��2�� ʶ��Ȩ�� ��3��ͨ�� =  ���� + ʶ��
							 "1234567890 ", //���       ���硱232134234�� ������
							 "1234567890 ", //�������    ���硱1123123123�� ������
							 "2015-12-09 11:11:11 ",//����ʱ�� YYYY-MM-DD hh:mm:ss
							 "1 ",					//���ɷ�ʽ '1' ��վ��ȡ ��2�� �ӿڻ�ȡ(API)
							 "192.168.100.100 ",    //����IP
							 "1111111122222222333333334444444455555555666666667777777788888888 ",//�����Դ    ���� ������E����
};
int _tmain(int argc, _TCHAR* argv[])
{


	char cer_data[200];

	const  char *p = cer_data;
	int offset = 0;
	char   temp[64];
	memset(cer_data, '1', 200);
	int i, j;
	 //�滻����֤��,׼������
	for ( i = 0; i < sizeof(store_cer_section_length); i++){

		 for ( j = 0; j < store_cer_section_length[i]; j++){


				 store_cer_string[i][j] = cer_data[65 + offset + j];

				 
			 }
			store_cer_string[i][1 + j] = '\0';
			offset += store_cer_section_length[i];
			printf("%s\n", store_cer_string[i]);
		 }


	

	getchar();
	return 0;
}

