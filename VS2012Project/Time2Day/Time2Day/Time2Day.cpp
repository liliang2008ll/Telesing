// Time2Day.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <stdio.h>
//计算任意两个日期之间的天数


//一个整月的天数
int daysOfMonth(int y, int m){
	if (m == 2){
		if (y % 4 == 0 && y % 100 != 0 || y % 400 == 0){
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

//输入年份已经过的天数
int daysBefore(int y, int m, int d){
	int i, result = 0;
	for (i = 1; i <= 12; i++){
		if (i == m){
			result += d;
			return result;

		}
		else{
			result += daysOfMonth(y, i);

		}

	}

}

//计算每年天数
int daysOfYear(int y){
	if (y % 4 == 0 && y % 400 != 0 || y % 400 == 0){
		return 366;

	}
	else return 365;

}

//判断输入有效
int valiDate(int y, int m, int d){
	if (y > 3000 || y<1900){
		printf("error year!\n");
		return 1;

	}
	if (m>12 || m<1){
		printf("error month!\n");
		return 1;

	}
	if (d>daysOfMonth(y, m) || d < 1){
		printf("error day!\n");
		return 1;

	}
	return 0;

}
//主函数
int main(){
	int y1, m1, d1, y2, m2, d2;
	printf("please input first date:yyyy-mm-dd\n");
	scanf("%d-%d-%d", &y1, &m1, &d1);
	while (valiDate(y1, m1, d1)){
		printf("please input first date:yyyy-mm-dd\n");
		scanf("%d-%d-%d", &y1, &m1, &d1);

	}
	printf("please input secend date:yyyy-mm-dd\n");
	scanf("%d-%d-%d", &y2, &m2, &d2);
	while (valiDate(y2, m2, d2)){
		printf("please input secend date:yyyy-mm-dd\n");
		scanf("%d-%d-%d", &y2, &m2, &d2);


	}
	int i, result = 0;
	for (i = y1; i < y2; i++){
		result += daysOfYear(i);
	}

	result = result - daysBefore(y1, m1, d1) + daysBefore(y2, m2, d2);
	printf("there are %d days\n", result);
	getchar();
	return 0;

}
