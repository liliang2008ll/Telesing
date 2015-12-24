
#include <stdio.h>
#include <stdlib.h>
//#include "windows.h"


#pragma once

class IdxGen
{
public:
	explicit IdxGen(void);
	~IdxGen(void);

	int genIdxs(int n,int *out,int &out_size) ;
	int swap(int *idxs,int idxs_length,int *out,int &out_size) ;
};
