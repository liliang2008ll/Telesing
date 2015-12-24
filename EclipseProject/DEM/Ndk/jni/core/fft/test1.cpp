

#include "Transformer.h"
#include "tchar.h"


int _tmain(int argc, _TCHAR* argv[])
{


	int len = 16;
	Transformer *t = new Transformer(len);


	double td[] = {8, 2, 4, -8, 0, 5, -9, 5, 0, 5, 1, 4, 7, 3, 6, 4};
	
	Complexs *fd = t->fft(td,len);
	
	for (int i = 0; i < len; i++) 
	{
		printf("[fft]  [ i:%d]  %lf %lf\n",i,fd->res[i] , fd->ims[i]);
	}
	printf("\n\n");

	double *tdBack =new double[len];
	int tdBack_len;
	t->ifft(fd,tdBack,tdBack_len);

	for (int i = 0; i < len; i++) 
	{
		printf("[ifft]  [ i:%d]  %lf\n",i,tdBack[i] / len);
	}
	
	delete fd;
	delete t; 
	
	return 0;
}

