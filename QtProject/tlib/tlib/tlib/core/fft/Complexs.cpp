
#include <stdio.h>
#include <string.h>
#include "Complexs.h"



Complexs::Complexs(int n)
{
	res = new double[n];
	ims = new double[n];
	memset(res,0,n*sizeof(double));
	memset(ims,0,n*sizeof(double));
	res_len=n;
}

Complexs::~Complexs(void)
{
	delete []res;
	delete []ims;
}


Complexs::Complexs(double *res_v,int len) 
{
	res = new double[len];
	ims = new double[len];
	memset(res,0,len*sizeof(double));
	memset(ims,0,len*sizeof(double));

	res_len=len;

	memcpy(res,res_v,len*sizeof(double));
}


Complexs::Complexs(double *res_v,double *ims_v,int len) 
{
	res = new double[len];
	ims = new double[len];
	memset(res,0,len*sizeof(double));
	memset(ims,0,len*sizeof(double));

	res_len=len;

	memcpy(res,res_v,len*sizeof(double));
	memcpy(ims,ims_v,len*sizeof(double));
}

void Complexs::set(double *res_v,double *ims_v,int len) 
{
	memset(res,0,len*sizeof(double));
	memset(ims,0,len*sizeof(double));
	res_len=len;
	memcpy(res,res_v,len*sizeof(double));
	memcpy(ims,ims_v,len*sizeof(double));
}
