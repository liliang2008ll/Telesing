

#include<stdio.h>		//dlopen
#include<stdlib.h>

#ifndef Complexs_H_
#define Complexs_H_

class Complexs
{
public:
	
    	int size;
    	//! 时域
    	double *td;
  	//! 频率(实部,虚部)
  	double *res;
    	double *ims;
    	
    	 explicit Complexs(int n);
	 ~Complexs();
	 
	void close_();
    	void set(double *td_v,int td_size, double *res_v,int res_size, double *ims_v,int ims_size) ;
	void set(double *res_v,int res_size, double *ims_v,int ims_size) ;
	void close();	
	void memset_data() ;
};


#endif /* Complexs_H_ */