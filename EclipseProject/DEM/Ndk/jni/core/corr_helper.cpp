
#include<.\core\corr_helper.h>


corr_helper::corr_helper()
{
}

corr_helper::~corr_helper(void)
{
}


int corr_helper::min(correlation data_v[],int data_len)
//correlation *corr_helper::min(correlation *data_v,int data_len) 
{
	correlation ct;
	double var1,min=1;
	int min_nu;
	for(int i=0;i<data_len;i++)
	{
		memcpy(&ct,&data_v[i],sizeof(correlation));
		//correlation *ct=&data_v[i];
		var1=ct.value;
		if(var1<min)
		{
			min=var1; 
			min_nu=i;
		}
	}
	//printf("[min] %d %lf\n",min_nu,data_v[min_nu].value);
	return min_nu;
	//return &data_v[min_nu];
}
