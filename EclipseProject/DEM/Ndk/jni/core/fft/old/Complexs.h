
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


	Complexs(int n);
	~Complexs(void);

	Complexs::Complexs(double *res_v,int len) ;
};
