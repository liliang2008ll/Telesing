
#include <string.h>
#include "IdxGen.h"

IdxGen::IdxGen(void)
{
}

IdxGen::~IdxGen(void)
{
}

int IdxGen::genIdxs(int n,int *out,int &out_size) 
{
	int *idxs  = new int[n];
	for (int i = 0; i < n; i++) 
	{
		idxs[i] = i;
	}

	swap(idxs,n,out,out_size);
	delete []idxs;
	return 0;
}

int IdxGen::swap(int *idxs,int idxs_length,int *out,int &out_size) 
{
    int len = idxs_length;

    // base case
	if (len == 1) 
	{
		memcpy(out,idxs,sizeof(int)*idxs_length);
		out_size=idxs_length;
		return 1;
	}

    int half = len / 2;

    // swap of even terms
    int *tempE = new int[half];
    for (int k = 0; k < half; k++)
	{
        tempE[k] = idxs[2 * k];
	}

    int *e = new int[len];
	int e_size;
	swap(tempE,half,e,e_size);

    // swap of odd terms
    int *tempO = new int[half];
    for (int k = 0; k < half; k++)  
	{
        tempO[k] = idxs[2 * k + 1];
    }

    int *o = new int[len];
	int o_size;
	swap(tempO,half,o,o_size);

    // combine
    int *swappedIdxs = new int[len];
    for (int k = 0; k < half; k++)
	{
        swappedIdxs[k] = e[k];
        swappedIdxs[k + half] = o[k];
    }
	
	memcpy(out,swappedIdxs,sizeof(int)*len);
	out_size=len;

	delete []tempE;
	delete []e;
	delete []tempO;
	delete []o;
	delete []swappedIdxs;

    return 0;
}
