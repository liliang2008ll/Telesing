#ifndef ___TIME_DIFF
#define ___TIME_DIFF


#endif

//计算任意两个日期之间的天数
class time_diff_days{

public:
	int time1_year;
	int time1_month;
	int time1_day;

	int time2_year;
	int time2_month;
	int time2_day;

	int sys_year;
	int sys_month;
	int sys_day;

	int sys_time_days;
	int time1_days;
	int time2_days;

	char temp_str[11];
	int  time_data[3];
public:
	int vali_date(int y, int m, int d);
	int days_of_year(int y);
	int days_before(int y, int m, int d);
	int days_of_month(int y, int m);
	int get_sys_time();
	int get_input_time(char *str);
	int get_time_diff_days();
	int get_sys_diff_days(char *str);


};

