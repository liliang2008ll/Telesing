#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tlib.h"
#include <time.h>
#include "time_diff.h"

//判断输入有效
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
//计算每年天数
int time_diff_days:: days_of_year(int y){
	if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)){
		return 366;

	}
	else return 365;
}
//输入年份已经过的天数
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
//判断这个月份是多少天
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
//计算当前系统时间
int time_diff_days:: get_sys_time(){
	time_t timep;
	struct tm *p;
	time(&timep);
	p=gmtime(&timep);

	//系统当前时间
	sys_year  = 1900+ p->tm_year;
	sys_month = 1   + p->tm_mon;
	sys_day   = 0   + p->tm_mday;

}

//系统输入时间转换
//2012-1-1
//2012-12-12
int time_diff_days:: get_input_time(char *str){

	int offset;
	char str_data[5];
	memset(temp_str,0,11);
	memset(str_data,0,5);
	memset(time_data,0,3);

	//时间字符串缓存到当前变量
	memcpy(temp_str,str,strlen(str));

	offset = 0;
	//获取字符串长度
	//获取年份,
	for(int i = 0 ; i < 3 ; i++)
	{
		sscanf(&temp_str[offset],"%[0-9]",str_data);//去到里面的数据
		time_data[i] = atoi(str_data);
		offset += strlen(str_data);//计算偏移
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
//获取时间1和系统时间差，用天数作为单位
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

