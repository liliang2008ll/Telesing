

#include <android/log.h>
#define LOG_TAG "xx"
#define LOGW(a )  __android_log_write(ANDROID_LOG_WARN,LOG_TAG,a)

#include "Transformer.h"

Transformer::Transformer(int n)
{
char out_str[100];
	if (n < 2 || n % 2 != 0)
	{
		printf("N is not a POWER of 2!");
	}

	N = n;
	INDEXS=new int[n];
	IdxGen *ig=new IdxGen();
	ig->genIdxs(n,INDEXS,INDEXS_len);

	EXP_NUM=0;
	ExpGen *eg=new ExpGen();
	eg->genExps(n,EXPRESSIONS,EXP_NUM);
	
sprintf(out_str,"[EXP_NUM] %d\n",EXP_NUM); 
LOGW(out_str);
}

Transformer::~Transformer(void)
{
	delete []INDEXS;


	for(int i=0;i<EXP_NUM;i++)
	{
		if(EXPRESSIONS[i]!=NULL)
		{
			delete EXPRESSIONS[i];
		}
	}
}

void Transformer::exec(Complexs *cs)
{
	for (int i = EXP_NUM - 1; i >= 0; i--)
	{
		Expression *exp = EXPRESSIONS[i];
		exp->exec(cs);
	}
	
	return;
}


//! 该函数使用完应该手动释放Complexs
Complexs* Transformer::fft(short *td,int td_length)
{
	if (td_length != N) 
	{
		printf("LENGTH of Complexs is not a EQUAL N!");
	}
	double *res = new double[td_length];
	for (int i = 0; i < N; i++) 
	{
		res[i] = td[INDEXS[i]];
	}
	Complexs *cp=new Complexs(res,td_length);
	exec(cp);
	delete []res;
	return cp;
}

//! 该函数使用完应该手动释放Complexs
Complexs* Transformer::fft(double *td,int td_length)
{
	if (td_length != N) 
	{
		printf("LENGTH of Complexs is not a EQUAL N!");
	}

	double *res = new double[td_length];
	for (int i = 0; i < N; i++) 
	{
		res[i] = td[INDEXS[i]];
	}
	Complexs *cp=new Complexs(res,td_length);
	exec(cp);
	delete []res;
	return cp;
}

int Transformer::ifft(Complexs *fd,double* res,int &res_len)
{
	if (fd->res_len != N) 
	{
		printf("LENGTH of Complexs is not a EQUAL N!");
	}

	Complexs *td = new Complexs(N);
	for (int i = 0; i < N; i++) 
	{
		td->res[i] = fd->res[INDEXS[i]];
		td->ims[i] = -fd->ims[INDEXS[i]];//共轭
	}
	exec(td);
	for (int i = 0; i < N; i++)
	{
		td->ims[i] = -td->ims[i];//共轭
	}
	memcpy(res,td->res,sizeof(double)*td->res_len);
	res_len=td->res_len;
	delete td;
	return 0;
} 
 