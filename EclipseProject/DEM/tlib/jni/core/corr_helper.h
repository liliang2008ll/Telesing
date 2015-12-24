

#include<stdio.h>		//dlopen
#include<stdlib.h>

#ifndef corr_helper_H_
#define corr_helper_H_

#define correlation_data_size   2
	

struct  correlation
{
int offset;
double value;
bool data[correlation_data_size];
};
	
class corr_helper
{
public:
    	 explicit corr_helper();
	 ~corr_helper();
	 
	//correlation* min(correlation *data_v,int data_len);
	int min(correlation data_v[],int data_len);
	 
};


#endif /* corr_helper_H_ */