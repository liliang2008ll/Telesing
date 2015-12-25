#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tlib.h"
#include <time.h>
#include "time_diff.h"

//�ж�������Ч
int time_diff_days:: vali_date(int y, int m, int d){
	if (y > 3000 || y<1900){
		printf("error year!\n");
		return 1;

	}
	if (m>12 || m<1){
		printf("error month!\n");
		return 1;

	}
	if (d>days_of_month(y, m) || d < 1){
		printf("error day!\n");
		return 1;

	}
	return 0;
}
//����ÿ������
int time_diff_days:: days_of_year(int y){
	if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)){
		return 366;

	}
	else return 365;
}
//��������Ѿ���������
int time_diff_days:: days_before(int y, int m, int d){
	int i, result = 0;
	for (i = 1; i <= 12; i++){
		if (i == m){
			result += d;
			return result;

		}
		else{
			result += days_of_month(y, i);

		}

	}
	return result;
}
//�ж�����·��Ƕ�����
int time_diff_days:: days_of_month(int y, int m){
	if (m == 2){
		if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)){
			return 29;

		}
		else{
			return 28;

		}

	}
	else if (m == 4 || m == 6 || m == 9 || m == 11){
		return 30;

	}
	else{
		return 31;

	}
}
//���㵱ǰϵͳʱ��
int time_diff_days:: get_sys_time(){
	time_t timep;
	struct tm *p;
	time(&timep);
	p=gmtime(&timep);

	//ϵͳ��ǰʱ��
	sys_year  = 1900+ p->tm_year;
	sys_month = 1   + p->tm_mon;
	sys_day   = 0   + p->tm_mday;

}

//ϵͳ����ʱ��ת��
//2012-1-1
//2012-12-12
int time_diff_days:: get_input_time(char *str){

	int offset;
	char str_data[5];
	memset(temp_str,0,11);
	memset(str_data,0,5);
	memset(time_data,0,3);

	//ʱ���ַ������浽��ǰ����
	memcpy(temp_str,str,strlen(str));

	offset = 0;
	//��ȡ�ַ�������
	//��ȡ���,
	for(int i = 0 ; i < 3 ; i++)
	{
		sscanf(&temp_str[offset],"%[0-9]",str_data);//ȥ�����������
		time_data[i] = atoi(str_data);
		offset += strlen(str_data);//����ƫ��
		memset(str_data,0,5);
		offset += 1;
	}

}

//huoqu
int time_diff_days::get_time_diff_days(){

	time1_days    = days_before(time1_year,time1_month,time1_day);
	time2_days    = days_before(time2_year,time2_month,time2_day);

	return (time1_days - time2_days);
}
//��ȡʱ��1��ϵͳʱ����������Ϊ��λ
int time_diff_days::get_sys_diff_days(char *str){

	int result = 0;
	get_input_time(str);
	get_sys_time();

	for(int i = sys_year;i <time_data[0]; i++)
	{
		result += days_of_year(i);
	}

	result = result + days_before(time_data[0],time_data[1],time_data[2])\
			        - days_before(sys_year,sys_month,sys_day);


	return result;
}

