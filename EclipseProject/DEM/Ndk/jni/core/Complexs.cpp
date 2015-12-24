
#include<Complexs.h>


Complexs::Complexs(int n)
{
	td = new double[n];
	res = new double[n];
	ims = new double[n];
	memset(td,0,sizeof(double)*n);
	memset(res,0,sizeof(double)*n);
	memset(ims,0,sizeof(double)*n);
	size=n;
}


Complexs::~Complexs(void)
{
}

void Complexs::close_()
{
	delete[] td;
	delete[] res;
	delete[] ims;
}
void Complexs::set(double *td_v,int td_size, double *res_v,int res_size, double *ims_v,int ims_size) 
{
	memcpy(td,td_v,td_size*sizeof(double));
	memcpy(res,res_v,res_size*sizeof(double));
	memcpy(ims,ims_v,ims_size*sizeof(double));
}

void Complexs::set(double *res_v,int res_size, double *ims_v,int ims_size) 
{
	memcpy(res,res_v,res_size*sizeof(double));
	memcpy(ims,ims_v,ims_size*sizeof(double));
}

void Complexs::memset_data() 
{
	memset(td,0,sizeof(double)*size);
	memset(res,0,sizeof(double)*size);
	memset(ims,0,sizeof(double)*size);
}

void Complexs::close()
{
	if(res!=NULL)
	delete []res;
	if(ims!=NULL)
	delete []ims;
}