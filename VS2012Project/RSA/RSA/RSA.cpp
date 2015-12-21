// RSA.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include "rsa_use.h"


int _tmain(int argc, _TCHAR* argv[])
{
	rsa_use test;
	test.test_class_func();
	test.test_rsa();
	getchar();
	return 0;
}

