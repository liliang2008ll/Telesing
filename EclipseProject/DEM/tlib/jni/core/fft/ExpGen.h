
#include <stdio.h>
#include <stdlib.h>
#include "Expression.h"


#pragma once

class ExpGen
{
public:

	explicit ExpGen(void);
	~ExpGen(void);

	void genExps(int n,Expression *expressions[],int &expressions_nu);
	void genExpL(Expression *expressions[],int &expressions_nu, int start, int amount);
	void genExp4(Expression *expressions[],int &expressions_nu, int start);
	void genExp2(Expression *expressions[],int &expressions_nu, int start) ;
};
