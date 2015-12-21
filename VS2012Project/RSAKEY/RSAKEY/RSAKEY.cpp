// RSAKEY.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "string.h"
//指针数组,用来存放接收到的解密后的字符
char store_cer_section_length[] = {  4, 10, 1, 10, 10, 19, 1, 15, 64 };

char store_cer_string[9][66] = { "4444 ",      //码段
							 "YYYY-MM-DD ",//失效日期    YYYY-MM-DD
							 "1 ",          //权限       ’1‘生成权限  ’2‘ 识别权限 ’3‘通用 =  生成 + 识别
							 "1234567890 ", //编号       例如”232134234“ 正整数
							 "1234567890 ", //机构编号    例如”1123123123“ 正整数
							 "2015-12-09 11:11:11 ",//生成时间 YYYY-MM-DD hh:mm:ss
							 "1 ",					//生成方式 '1' 网站获取 ‘2’ 接口获取(API)
							 "192.168.100.100 ",    //生成IP
							 "1111111122222222333333334444444455555555666666667777777788888888 ",//码段来源    例如 ”智声E联“
};
int _tmain(int argc, _TCHAR* argv[])
{


	char cer_data[200];

	const  char *p = cer_data;
	int offset = 0;
	char   temp[64];
	memset(cer_data, '1', 200);
	int i, j;
	 //替换本地证书,准备返回
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

