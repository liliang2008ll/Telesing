
#include "ExpGen.h"

ExpGen::ExpGen(void)
{

}

ExpGen::~ExpGen(void)
{
}



void ExpGen::genExps(int n,Expression *expressions[],int &expressions_nu)
{
	if (n >= 8) 
	{
		genExpL(expressions,expressions_nu, 0, n);
	} 
	else if (n == 4) 
	{
		genExp4(expressions,expressions_nu, 0);
	} 
	else if (n == 2) 
	{
		genExp2(expressions,expressions_nu, 0);
	}
}

void ExpGen::genExpL(Expression *expressions[],int &expressions_nu, int start, int amount)
{
	Expression *ee=new Expression(OP_RADIX_L, start,amount);
	expressions[expressions_nu]=ee;
	expressions_nu++;
	//expressions.add(new Expression(Expression.OP_RADIX_L, start, amount));
	if (amount >= 32)
	{
		genExpL(expressions,expressions_nu, start, amount / 2);
		genExpL(expressions,expressions_nu, start + amount / 2, amount / 4);
		genExpL(expressions,expressions_nu, start + amount / 4 * 3, amount / 4);
	} 
	else if (amount == 16) 
	{
		genExpL(expressions,expressions_nu, start, 8);
		genExp4(expressions,expressions_nu, start + 8);
		genExp4(expressions,expressions_nu, start + 12);
	}
	else if (amount == 8) 
	{
		genExp4(expressions,expressions_nu, start);
		genExp2(expressions,expressions_nu, start + 4);
		genExp2(expressions,expressions_nu, start + 6);
	}
}

void ExpGen::genExp4(Expression *expressions[],int &expressions_nu, int start)
{
	Expression *ee=new Expression(OP_RADIX_4, start, 4);
	expressions[expressions_nu]=ee;
	expressions_nu++;
	//expressions.add(new Expression(Expression.OP_RADIX_4, start, 4));
}
void ExpGen::genExp2(Expression *expressions[],int &expressions_nu, int start) 
{
	Expression *ee=new Expression(OP_RADIX_2, start, 4);
	expressions[expressions_nu]=ee;
	expressions_nu++;
	//expressions.add(new Expression(Expression.OP_RADIX_2, start, 2));
}
