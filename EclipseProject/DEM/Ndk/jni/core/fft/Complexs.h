
#include <stdio.h>
#include <stdlib.h>
//#include "windows.h"

#pragma once

class Complexs
{
public:

	double *res;
	double *ims;
	int res_len;


	explicit Complexs(int n);
	~Complexs(void);

	explicit Complexs(double *res_v,int len) ;
	explicit Complexs(double *res_v,double *ims_v,int len) ;
	void set(double *res_v,double *ims_v,int len) ;
};
