
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Complexs.h"

#define  OP_RADIX_L   0
#define  OP_RADIX_4   1
#define  OP_RADIX_2   2

#pragma once

class Expression
{
public:

    int operator_;
    int start;
    int amount;

    double *wRe_ks;
    double *wIm_ks;
    double *wRe_3ks;
    double *wIm_3ks;


	explicit Expression(int operator_v, int start_v, int amount_v);
	~Expression(void);

	void calcWs();
	void exec(Complexs *cs);
	void radixL(Complexs *cs) ;
	void radix4(Complexs *cs);
	void radix2(Complexs *cs);
};
