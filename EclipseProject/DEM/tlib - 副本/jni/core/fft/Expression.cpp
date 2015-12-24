

#include "Expression.h"

Expression::Expression(int operator_v, int start_v, int amount_v)
{
    operator_=operator_v;
    start=start_v;
    amount=amount_v;
	wRe_ks=NULL;
	wIm_ks=NULL;
	wRe_3ks=NULL;
	wIm_3ks=NULL;

	if (operator_v == OP_RADIX_L) 
	{
		calcWs();
	}
}

Expression::~Expression(void)
{
	if(wRe_ks!=NULL)
		delete []wRe_ks;
	if(wIm_ks!=NULL)
		delete []wIm_ks;
	if(wRe_3ks!=NULL)
		delete []wRe_3ks;
	if(wIm_3ks!=NULL)
		delete []wIm_3ks;
}


void Expression::calcWs()
{
	double PI = 3.14159265358979323846;
	int quarter = amount / 4;
	double TPOA = PI * 2 / amount;
	wRe_ks = new double[quarter];
	wIm_ks = new double[quarter];
	wRe_3ks = new double[quarter];
	wIm_3ks = new double[quarter];
	for (int k = 0; k < quarter; k++) 
	{
		wRe_ks[k] = cos(TPOA * k);
		wIm_ks[k] = -sin(TPOA * k);
		wRe_3ks[k] = cos(TPOA * k * 3);
		wIm_3ks[k] = -sin(TPOA * k * 3);
	}
}

void Expression::exec(Complexs *cs)
{
	switch (operator_) 
	{
	case OP_RADIX_L:
		radixL(cs);
	break;
	case OP_RADIX_4:
		radix4(cs);
	break;
	case OP_RADIX_2:
		radix2(cs);
	break;
	default:
	break;
	}	
}

void Expression::radixL(Complexs *cs) 
{
    int half = amount / 2;
    int quarter = amount / 4;
    int start_v = start;

    start_v += half;
    for (int k = 0; k < quarter; k++) 
	{
        // * w
        double re0Old = cs->res[start_v + k];
        double im0Old = cs->ims[start_v + k];
        double reQOld = cs->res[start_v + quarter + k];
        double imQOld = cs->ims[start_v + quarter + k];
        cs->res[start_v + k] = re0Old * wRe_ks[k] - im0Old * wIm_ks[k];
        cs->ims[start_v + k] = re0Old * wIm_ks[k] + im0Old * wRe_ks[k];
        cs->res[start_v + quarter + k] = reQOld * wRe_3ks[k] - imQOld * wIm_3ks[k];
        cs->ims[start_v + quarter + k] = reQOld * wIm_3ks[k] + imQOld * wRe_3ks[k];

        // Wave 1
        double reQNew = cs->ims[start_v + k] - cs->ims[start_v + quarter + k];//*-i
        double imQNew = cs->res[start_v + quarter + k] - cs->res[start_v + k];//*-i
        cs->res[start_v + k] = cs->res[start_v + k] + cs->res[start_v + quarter + k];
        cs->ims[start_v + k] = cs->ims[start_v + k] + cs->ims[start_v + quarter + k];
        cs->res[start_v + quarter + k] = reQNew;
        cs->ims[start_v + quarter + k] = imQNew;
    }

    // Wave 2
    start_v -= half;
    for (int k = 0; k < half; k++)
	{
        double reHNew = cs->res[start_v + k] - cs->res[start_v + half + k];
        double imHNew = cs->ims[start_v + k] - cs->ims[start_v + half + k];
        cs->res[start_v + k] = cs->res[start_v + k] + cs->res[start_v + half + k];
        cs->ims[start_v + k] = cs->ims[start_v + k] + cs->ims[start_v + half + k];
        cs->res[start_v + half + k] = reHNew;
        cs->ims[start_v + half + k] = imHNew;
    }
}

void Expression::radix4(Complexs *cs)
{
	double re0Old = cs->res[start];
	double im0Old = cs->ims[start];
	double re1Old = cs->res[start + 1];
	double im1Old = cs->ims[start + 1];
	double re2Old = cs->res[start + 2];
	double im2Old = cs->ims[start + 2];
	double re3Old = cs->res[start + 3];
	double im3Old = cs->ims[start + 3];

	// Wave 1
	cs->res[start] = re0Old + re1Old;
	cs->ims[start] = im0Old + im1Old;
	cs->res[start + 1] = re0Old - re1Old;
	cs->ims[start + 1] = im0Old - im1Old;
	cs->res[start + 2] = re2Old + re3Old;
	cs->ims[start + 2] = im2Old + im3Old;
	cs->res[start + 3] = im2Old - im3Old;//*-i
	cs->ims[start + 3] = re3Old - re2Old;//*-i

	// Wave 2
	double re2New = cs->res[start] - cs->res[start + 2];
	double im2New = cs->ims[start] - cs->ims[start + 2];
	double re3New = cs->res[start + 1] - cs->res[start + 3];
	double im3New = cs->ims[start + 1] - cs->ims[start + 3];
	cs->res[start] = cs->res[start] + cs->res[start + 2];
	cs->ims[start] = cs->ims[start] + cs->ims[start + 2];
	cs->res[start + 1] = cs->res[start + 1] + cs->res[start + 3];
	cs->ims[start + 1] = cs->ims[start + 1] + cs->ims[start + 3];
	cs->res[start + 2] = re2New;
	cs->ims[start + 2] = im2New;
	cs->res[start + 3] = re3New;
	cs->ims[start + 3] = im3New;
}

void Expression::radix2(Complexs *cs)
{
	double re1New = cs->res[start] - cs->res[start + 1];
	double im1New = cs->ims[start] - cs->ims[start + 1];
	cs->res[start] = cs->res[start] + cs->res[start + 1];
	cs->ims[start] = cs->ims[start] + cs->ims[start + 1];
	cs->res[start + 1] = re1New;
	cs->ims[start + 1] = im1New;
}