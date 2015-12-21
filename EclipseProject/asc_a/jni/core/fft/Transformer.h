
#include <stdio.h>
#include <stdlib.h>
#include "IdxGen.h"
#include "Expression.h"
#include "ExpGen.h"

#pragma once

class Transformer
{
public:


	int N;
	int *INDEXS;
	int INDEXS_len;
    Expression *EXPRESSIONS[1024*4];
    int EXP_NUM;


	explicit Transformer(int n);
	~Transformer(void);
	void exec(Complexs *cs);
	Complexs* fft(short *td,int td_length);
	Complexs* fft(double *td,int td_length);
	int ifft(Complexs *fd,double* res,int &res_len);
};
