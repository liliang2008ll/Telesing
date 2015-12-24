

#include<stdio.h>		//dlopen
#include<stdlib.h>

#ifndef buffer_c_H_
#define buffer_c_H_

class buffer_c
{
public:
	
	short *data;
	int data_length;
	
	/**
	* �ջ���wPoint = rPoint
	* ������rPoint - wPoint = 1 or data.size - 1
	*/
	int wPoint;//дָ�룬ָ�򻺳�ռ�ĵ�һ����дλ�� 
	int rPoint;//��ָ�룬ָ�򻺳�ռ�ĵ�һ���ɶ�λ��
	
	 explicit buffer_c(int size);
	~buffer_c();
	
	bool write(short *wData,int len) ;
	bool read(short *rData,int size) ;
	bool preview(short *pData, int pData_length,int offset) ;
	bool skip(int count) ;
	void doSkip(int count) ;
	int emptySize(); 
	int fillSize() ;
	int size();
	int getData(short *data_v,int *data_size_v) ;
	int getwPoint() ;
	int getrPoint() ;
	void setBufferSize(int size);
	void clearBuffer();
};


#endif /* buffer_c_H_ */  