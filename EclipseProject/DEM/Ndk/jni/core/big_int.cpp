/*****************************************************************
大数运算库源文件：big_int.cpp
作者：afanty@vip.sina.com
版本：1.2 (2003.5.13)
说明：适用于MFC，1024位RSA运算
*****************************************************************/
//#include "stdafx.h"
#include "big_int.h"

//小素数表
const static int PrimeTable[550]=
{   3,    5,    7,    11,   13,   17,   19,   23,   29,   31,
    37,   41,   43,   47,   53,   59,   61,   67,   71,   73,
    79,   83,   89,   97,   101,  103,  107,  109,  113,  127, 
    131,  137,  139,  149,  151,  157,  163,  167,  173,  179, 
    181,  191,  193,  197,  199,  211,  223,  227,  229,  233, 
    239,  241,  251,  257,  263,  269,  271,  277,  281,  283, 
    293,  307,  311,  313,  317,  331,  337,  347,  349,  353, 
    359,  367,  373,  379,  383,  389,  397,  401,  409,  419, 
    421,  431,  433,  439,  443,  449,  457,  461,  463,  467, 
    479,  487,  491,  499,  503,  509,  521,  523,  541,  547, 
    557,  563,  569,  571,  577,  587,  593,  599,  601,  607, 
    613,  617,  619,  631,  641,  643,  647,  653,  659,  661, 
    673,  677,  683,  691,  701,  709,  719,  727,  733,  739, 
    743,  751,  757,  761,  769,  773,  787,  797,  809,  811, 
    821,  823,  827,  829,  839,  853,  857,  859,  863,  877,
    881,  883,  887,  907,  911,  919,  929,  937,  941,  947, 
    953,  967,  971,  977,  983,  991,  997,  1009, 1013, 1019, 
    1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087,
    1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153, 
    1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223, 1229, 
    1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 1297, 
    1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381,
    1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 
    1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511, 1523,
    1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597, 
    1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 1663, 
    1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 1741, 
    1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 1823, 
    1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889, 1901, 
    1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993, 
    1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039, 2053, 2063,
    2069, 2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129, 2131, 
    2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207, 2213, 2221, 
    2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287, 2293,
    2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357, 2371,
    2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423, 2437, 
    2441, 2447, 2459, 2467, 2473, 2477, 2503, 2521, 2531, 2539, 
    2543, 2549, 2551, 2557, 2579, 2591, 2593, 2609, 2617, 2621, 
    2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683, 2687, 2689, 
    2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741, 2749, 
    2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819, 2833, 
    2837, 2843, 2851, 2857, 2861, 2879, 2887, 2897, 2903, 2909,
    2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999, 3001,
    3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079, 3083,
    3089, 3109, 3119, 3121, 3137, 3163, 3167, 3169, 3181, 3187, 
    3191, 3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257, 3259, 
    3271, 3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331, 3343,
    3347, 3359, 3361, 3371, 3373, 3389, 3391, 3407, 3413, 3433, 
    3449, 3457, 3461, 3463, 3467, 3469, 3491, 3499, 3511, 3517, 
    3527, 3529, 3533, 3539, 3541, 3547, 3557, 3559, 3571, 3581,
    3583, 3593, 3607, 3613, 3617, 3623, 3631, 3637, 3643, 3659, 
    3671, 3673, 3677, 3691, 3697, 3701, 3709, 3719, 3727, 3733, 
    3739, 3761, 3767, 3769, 3779, 3793, 3797, 3803, 3821, 3823, 
    3833, 3847, 3851, 3853, 3863, 3877, 3881, 3889, 3907, 3911, 
    3917, 3919, 3923, 3929, 3931, 3943, 3947, 3967, 3989, 4001
};

//构造大数对象并初始化为零
big_int::big_int()
{
    m_nLength=1;
    for(int i=0;i<BI_MAXLEN;i++)m_ulValue[i]=0;
}

//解构大数对象
big_int::~big_int()
{
}

/****************************************************************************************
大数比较
调用方式：N.Cmp(A)
返回值：若N<A返回-1；若N=A返回0；若N>A返回1
****************************************************************************************/
int big_int::Cmp(big_int& A)
{
    if(m_nLength>A.m_nLength)return 1;
    if(m_nLength<A.m_nLength)return -1;
    for(int i=m_nLength-1;i>=0;i--)
    {
        if(m_ulValue[i]>A.m_ulValue[i])return 1;
        if(m_ulValue[i]<A.m_ulValue[i])return -1;
    }
    return 0;
}

/****************************************************************************************
大数赋值
调用方式：N.Mov(A)
返回值：无，N被赋值为A
****************************************************************************************/
void big_int::Mov(big_int& A)
{
    m_nLength=A.m_nLength;
    for(int i=0;i<BI_MAXLEN;i++)m_ulValue[i]=A.m_ulValue[i];
}

void big_int::Mov(unsigned long long A)
//void big_int::Mov(unsigned __int64 A)
{
    if(A>0xffffffff)
    {
        m_nLength=2;
        m_ulValue[1]=(unsigned long)(A>>32);
        m_ulValue[0]=(unsigned long)A;
    }
    else
    {
        m_nLength=1;
        m_ulValue[0]=(unsigned long)A;
    }
    for(int i=m_nLength;i<BI_MAXLEN;i++)m_ulValue[i]=0;
}

/****************************************************************************************
大数相加
调用形式：N.Add(A)
返回值：N+A
****************************************************************************************/
big_int big_int::Add(big_int& A)
{
    big_int X;
    X.Mov(*this);
    unsigned carry=0;
    unsigned long long sum=0;
    if(X.m_nLength<A.m_nLength)X.m_nLength=A.m_nLength;
    for(unsigned i=0;i<X.m_nLength;i++)
    {
        sum=A.m_ulValue[i];
		sum=sum+X.m_ulValue[i]+carry;
        X.m_ulValue[i]=(unsigned long)sum;
        carry=(unsigned)(sum>>32);
    }
    X.m_ulValue[X.m_nLength]=carry;
    X.m_nLength+=carry;
    return X;
}

big_int big_int::Add(unsigned long A)
{
    big_int X;
    X.Mov(*this);
    unsigned long long sum;
    sum=X.m_ulValue[0];
	sum+=A;
    X.m_ulValue[0]=(unsigned long)sum;
    if(sum>0xffffffff)
    {
        unsigned i=1;
        while(X.m_ulValue[i]==0xffffffff){X.m_ulValue[i]=0;i++;}
        X.m_ulValue[i]++;
        if(m_nLength==i)m_nLength++;
    }
    return X;
}

/****************************************************************************************
大数相减
调用形式：N.Sub(A)
返回值：N-A
****************************************************************************************/
big_int big_int::Sub(big_int& A)
{
    big_int X;
    X.Mov(*this);
    if(X.Cmp(A)<=0){X.Mov(0);return X;}
    unsigned carry=0;
    unsigned long long num;
	unsigned i;
    for(i=0;i<m_nLength;i++)
    {
        if((m_ulValue[i]>A.m_ulValue[i])||((m_ulValue[i]==A.m_ulValue[i])&&(carry==0)))
        {
            X.m_ulValue[i]=m_ulValue[i]-carry-A.m_ulValue[i];
            carry=0;
        }
        else
        {
            num=0x100000000+m_ulValue[i];
            X.m_ulValue[i]=(unsigned long)(num-carry-A.m_ulValue[i]);
            carry=1;
        }
    }
    while(X.m_ulValue[X.m_nLength-1]==0)X.m_nLength--;
    return X;
}

big_int big_int::Sub(unsigned long A)
{
    big_int X;
    X.Mov(*this);
    if(X.m_ulValue[0]>=A){X.m_ulValue[0]-=A;return X;}
    if(X.m_nLength==1){X.Mov(0);return X;}
    unsigned long long num=0x100000000+X.m_ulValue[0];
    X.m_ulValue[0]=(unsigned long)(num-A);
    int i=1;
    while(X.m_ulValue[i]==0){X.m_ulValue[i]=0xffffffff;i++;}
    X.m_ulValue[i]--;
    if(X.m_ulValue[i]==0)X.m_nLength--;
    return X;
}

/****************************************************************************************
大数相乘
调用形式：N.Mul(A)
返回值：N*A
****************************************************************************************/
big_int big_int::Mul(big_int& A)
{
    if(A.m_nLength==1)return Mul(A.m_ulValue[0]);
	big_int X;
	unsigned long long sum,mul=0,carry=0;
	unsigned i,j;
	X.m_nLength=m_nLength+A.m_nLength-1;
    for(i=0;i<X.m_nLength;i++)
	{
		sum=carry;
		carry=0;
		for(j=0;j<A.m_nLength;j++)
		{
            if(((i-j)>=0)&&((i-j)<m_nLength))
			{
				mul=m_ulValue[i-j];
				mul*=A.m_ulValue[j];
			    carry+=mul>>32;
				mul=mul&0xffffffff;
				sum+=mul;
			}
        }
		carry+=sum>>32;
		X.m_ulValue[i]=(unsigned long)sum;
	}
	if(carry){X.m_nLength++;X.m_ulValue[X.m_nLength-1]=(unsigned long)carry;}
    return X;
}

big_int big_int::Mul(unsigned long A)
{
    big_int X;
    unsigned long long mul;
    unsigned long carry=0;
    X.Mov(*this);
    for(unsigned i=0;i<m_nLength;i++)
    {
        mul=m_ulValue[i];
        mul=mul*A+carry;
        X.m_ulValue[i]=(unsigned long)mul;
        carry=(unsigned long)(mul>>32);
    }
    if(carry){X.m_nLength++;X.m_ulValue[X.m_nLength-1]=carry;}
    return X;
}

/****************************************************************************************
大数相除
调用形式：N.Div(A)
返回值：N/A
****************************************************************************************/
big_int big_int::Div(big_int& A)
{
	big_int var_add,var_mul,var_sub;
	
    if(A.m_nLength==1)return Div(A.m_ulValue[0]);
    big_int X,Y,Z;
    unsigned i,len;
    unsigned long long num,div;
    Y.Mov(*this); 
    
    while(Y.Cmp(A)>=0)
    {    
		div=Y.m_ulValue[Y.m_nLength-1];
		num=A.m_ulValue[A.m_nLength-1];
		len=Y.m_nLength-A.m_nLength;
		if((div==num)&&(len==0))
		{
			var_add=X.Add(1);
			X.Mov(var_add);
			//X.Mov((big_int)X.Add(1));
			break;
		}
		if((div<=num)&&len){len--;div=(div<<32)+Y.m_ulValue[Y.m_nLength-2];}
		div=div/(num+1);
		Z.Mov(div);
		if(len)
		{
			Z.m_nLength+=len;
			for(i=Z.m_nLength-1;i>=len;i--)Z.m_ulValue[i]=Z.m_ulValue[i-len];
			for(i=0;i<len;i++)Z.m_ulValue[i]=0;
		}
    	var_add=X.Add(Z);
    	X.Mov(var_add);
    	var_mul=A.Mul(Z);
    	var_sub=Y.Sub(var_mul);
    	Y.Mov(var_sub);
		//X.Mov(X.Add(Z));
        //Y.Mov(Y.Sub(A.Mul(Z)));
		//X.Mov(X.Add(Z));
        //Y.Mov(Y.Sub(A.Mul(Z)));
    }
    return X;
}

big_int big_int::Div(unsigned long A)
{
    big_int X;
    X.Mov(*this);
    if(X.m_nLength==1){X.m_ulValue[0]=X.m_ulValue[0]/A;return X;}
    unsigned long long div,mul;
    unsigned long carry=0;
    for(int i=X.m_nLength-1;i>=0;i--)
    {
        div=carry;
        div=(div<<32)+X.m_ulValue[i];
        X.m_ulValue[i]=(unsigned long)(div/A);
        mul=(div/A)*A;
        carry=(unsigned long)(div-mul);
    }
    if(X.m_ulValue[X.m_nLength-1]==0)X.m_nLength--;
    return X;
}

/****************************************************************************************
大数求模
调用形式：N.Mod(A)
返回值：N%A
****************************************************************************************/
big_int big_int::Mod(big_int& A)
{
	big_int var_sub,var_mul;
    big_int X,Y;
	unsigned long long div,num;
    unsigned long carry=0;
	unsigned i,len;
    X.Mov(*this); 
    while(X.Cmp(A)>=0)
    {
		div=X.m_ulValue[X.m_nLength-1];
		num=A.m_ulValue[A.m_nLength-1];
		len=X.m_nLength-A.m_nLength;
		if((div==num)&&(len==0))
		{
			var_sub=X.Sub(A);
			X.Mov(var_sub);
			//X.Mov(X.Sub(A));
			break;
		}
		if((div<=num)&&len){len--;div=(div<<32)+X.m_ulValue[X.m_nLength-2];}
		div=div/(num+1);
		Y.Mov(div);
		var_mul=A.Mul(Y);
		Y.Mov(var_mul);
		//Y.Mov(A.Mul(Y));
		if(len)
		{
			Y.m_nLength+=len;
			for(i=Y.m_nLength-1;i>=len;i--)Y.m_ulValue[i]=Y.m_ulValue[i-len];
			for(i=0;i<len;i++)Y.m_ulValue[i]=0;
		}
		var_sub=X.Sub(Y);
		X.Mov(var_sub);
        //X.Mov(X.Sub(Y));
    }
    return X;
}

unsigned long big_int::Mod(unsigned long A)
{
    if(m_nLength==1)return(m_ulValue[0]%A);
    unsigned long long div;
    unsigned long carry=0;
    for(int i=m_nLength-1;i>=0;i--)
    {
        div=m_ulValue[i];
		div+=carry*0x100000000;
        carry=(unsigned long)(div%A);
    }
    return carry;
}

/****************************************************************************************
从字符串按10进制或16进制格式输入到大数
调用格式：N.Get(str,sys)
返回值：N被赋值为相应大数
sys暂时只能为10或16
***************************************************************************************
void big_int::Get(CString& str, unsigned int system)
{
    int len=str.GetLength(),k;
    Mov(0);
    for(int i=0;i<len;i++)
    {
       Mov(Mul(system));
       if((str[i]>='0')&&(str[i]<='9'))k=str[i]-48;
       else if((str[i]>='A')&&(str[i]<='F'))k=str[i]-55;
       else if((str[i]>='a')&&(str[i]<='f'))k=str[i]-87;
       else k=0;
       Mov(Add(k));
    }
}*/


/****************************************************************************************
从字符串按10进制或16进制格式输入到大数
调用格式：N.Get(str,sys)
返回值：N被赋值为相应大数
sys暂时只能为10或16
****************************************************************************************/
void big_int::Get_c(char *str,int str_size, unsigned int system)
{
	big_int var_mul,var_add;
    int len=str_size,k;
    Mov(0);
    for(int i=0;i<len;i++)
    {
    //printf("%d--",i);
    	var_mul=Mul(system);
    	Mov(var_mul);
       //Mov(Mul(system));
       if((str[i]>='0')&&(str[i]<='9'))k=str[i]-48;
       else if((str[i]>='A')&&(str[i]<='F'))k=str[i]-55;
       else if((str[i]>='a')&&(str[i]<='f'))k=str[i]-87;
       else k=0;
       var_add=Add(k);
       Mov(var_add);
       //Mov(Add(k));
    }
}

///****************************************************************************************
//将大数按10进制或16进制格式输出为字符串
//调用格式：N.Put(str,sys)
//返回值：无，参数str被赋值为N的sys进制字符串
//sys暂时只能为10或16
//***************************************************************************************
//void big_int::Put(CString& str, unsigned int system)
//{
//    if((m_nLength==1)&&(m_ulValue[0]==0)){str="0";return;}
//	str="";
//    CString t="0123456789ABCDEF";
//    int a;
//    char ch;
//    big_int X;
//    X.Mov(*this);
//    while(X.m_ulValue[X.m_nLength-1]>0)
//    {
//        a=X.Mod(system);
//        ch=t[a];
//        str.Insert(0,ch);
//        X.Mov(X.Div(system));
//    }
//}*/
//
/****************************************************************************************
将大数按10进制或16进制格式输出为字符串
调用格式：N.Put(str,sys)
返回值：无，参数str被赋值为N的sys进制字符串
sys暂时只能为10或16
****************************************************************************************/
void big_int::Put_c(char *str,int *str_size, unsigned int system)
{
	big_int var_div;
    if((m_nLength==1)&&(m_ulValue[0]==0)){return;}
	char temp_buf[1024];
	memset(temp_buf,0,1024);
	int out_size=0;
	char t[16];
	memcpy(t,"0123456789ABCDEF",16);
    int a;
    char ch;
    big_int X;
    X.Mov(*this);
    while(X.m_ulValue[X.m_nLength-1]>0)
    {
        a=X.Mod(system);
        ch=t[a];
		str[0]=ch;
		memcpy(str+1,temp_buf,strlen(temp_buf));
		out_size++;
		memset(temp_buf,0,1024);
		memcpy(temp_buf,str,out_size);
		//printf("输出加密:%d-%d %c %s\n",out_size,strlen(temp_buf),ch,str);
		var_div=X.Div(system);
		X.Mov(var_div);
        //X.Mov(X.Div(system));
    }
	memcpy(str_size,&out_size,sizeof(int));
}

//
/****************************************************************************************
求不定方程ax-by=1的最小整数解
调用方式：N.Euc(A)
返回值：X,满足：NX mod A=1
****************************************************************************************/
big_int big_int::Euc(big_int& A)
{
	big_int var_div,var_mod,var_mul,var_sub,var_add;
	big_int M,E,X,Y,I,J;
    int x,y;
	M.Mov(A);
	E.Mov(*this);
	X.Mov(0);
	Y.Mov(1);
	x=y=1;
	while((E.m_nLength!=1)||(E.m_ulValue[0]!=0))
	{
		var_div=M.Div(E);
		I.Mov(var_div);
		//I.Mov(M.Div(E));
		var_mod=M.Mod(E);
		J.Mov(var_mod);
		//J.Mov(M.Mod(E));
		M.Mov(E);
		E.Mov(J);
		J.Mov(Y);
		var_mul=Y.Mul(I);
		Y.Mov(var_mul);
		//Y.Mov(Y.Mul(I));
		if(x==y)
		{
		    if(X.Cmp(Y)>=0)
		    {
		    	var_sub=X.Sub(Y);
		    	Y.Mov(var_sub);
		    	//Y.Mov(X.Sub(Y));
		    }
			else
			{
		    	var_sub=X.Sub(X);
		    	Y.Mov(var_sub);
				//Y.Mov(Y.Sub(X));
				y=0;
			}
		}
		else
		{
			var_add=X.Add(Y);
			Y.Mov(var_add);
			//Y.Mov(X.Add(Y));
			x=1-x;
			y=1-y;
		}
		X.Mov(J);
	}
	if(x==0)
	{
		var_sub=A.Sub(X);
		X.Mov(var_sub);
		//X.Mov(A.Sub(X));
	}
	return X;
}

/****************************************************************************************
求乘方的模
调用方式：N.RsaTrans(A,B)
返回值：X=N^A MOD B
****************************************************************************************/
big_int big_int::RsaTrans(big_int& A, big_int& B)
{
	big_int var_div,var_mod,var_mul,var_sub,var_add;
    big_int X,Y;
	int i,j,k;
	unsigned n;
	unsigned long num;
	k=A.m_nLength*32-32;
	num=A.m_ulValue[A.m_nLength-1];
	while(num){num=num>>1;k++;}
	X.Mov(*this);
	for(i=k-2;i>=0;i--)
	{
		var_mul=X.Mul(X.m_ulValue[X.m_nLength-1]);
		Y.Mov(var_mul);
		var_mod=Y.Mod(B);
		Y.Mov(var_mod);
		//Y.Mov(X.Mul(X.m_ulValue[X.m_nLength-1]));
		//Y.Mov(Y.Mod(B));
        for(n=1;n<X.m_nLength;n++)
		{          
			for(j=Y.m_nLength;j>0;j--)Y.m_ulValue[j]=Y.m_ulValue[j-1];
			Y.m_ulValue[0]=0;
			Y.m_nLength++;
			var_mul=X.Mul(X.m_ulValue[X.m_nLength-n-1]);
			var_add=Y.Add(var_mul);
			Y.Mov(var_add);
			var_mod=Y.Mod(B);
			Y.Mov(var_mod);
			//Y.Mov(Y.Add(X.Mul(X.m_ulValue[X.m_nLength-n-1])));
			//Y.Mov(Y.Mod(B));
		}
		X.Mov(Y);
		if((A.m_ulValue[i>>5]>>(i&31))&1)
		{
			var_mul=Mul(X.m_ulValue[X.m_nLength-1]);
			Y.Mov(var_mul);
			var_mod=Y.Mod(B);
			Y.Mov(var_mod);
		    //Y.Mov(Mul(X.m_ulValue[X.m_nLength-1]));
		    //Y.Mov(Y.Mod(B));
            for(n=1;n<X.m_nLength;n++)
			{          
			    for(j=Y.m_nLength;j>0;j--)Y.m_ulValue[j]=Y.m_ulValue[j-1];
			    Y.m_ulValue[0]=0;
			    Y.m_nLength++;
				var_mul=Mul(X.m_ulValue[X.m_nLength-n-1]);
				var_add=Y.Add(var_mul);
				Y.Mov(var_add);
				var_mod=Y.Mod(B);
				Y.Mov(var_mod);
			    //Y.Mov(Y.Add(Mul(X.m_ulValue[X.m_nLength-n-1])));
			   // Y.Mov(Y.Mod(B));
			}
			
		    X.Mov(Y);
		}
	}
    return X;
}

/****************************************************************************************
拉宾米勒算法测试素数
调用方式：N.Rab() 
返回值：若N为素数，返回1，否则返回0
****************************************************************************************/
int big_int::Rab()
{
	big_int var_RsaTrans;
    unsigned i,j,pass;
    for(i=0;i<550;i++){if(Mod(PrimeTable[i])==0)return 0;}
    big_int S,A,I,K;
    K.Mov(*this);
	K.m_ulValue[0]--;
    for(i=0;i<5;i++)
    {
        pass=0;
        A.Mov(rand()*rand());
		S.Mov(K);
		/**/
        while((S.m_ulValue[0]&1)==0)
		{
            for(j=0;j<S.m_nLength;j++)
			{
			    S.m_ulValue[j]=S.m_ulValue[j]>>1;
			    if(S.m_ulValue[j+1]&1)S.m_ulValue[j]=S.m_ulValue[j]|0x80000000;
			}
		    if(S.m_ulValue[S.m_nLength-1]==0)S.m_nLength--;
		    var_RsaTrans=A.RsaTrans(S,*this);
		    I.Mov(var_RsaTrans);
			//I.Mov(A.RsaTrans(S,*this));
			if(I.Cmp(K)==0){pass=1;break;}
		}
		
		if((I.m_nLength==1)&&(I.m_ulValue[0]==1))pass=1;
		if(pass==0)return 0;
	}
    return 1;
}

/****************************************************************************************
产生随机素数
调用方法：N.GetPrime(bits)
返回值：N被赋值为一个bits位（0x100000000进制长度）的素数
****************************************************************************************/
void big_int::GetPrime(int bits)
{
	big_int var_RsaTrans,var_div;
    unsigned i;
    m_nLength=bits;
begin:
	for(i=0;i<m_nLength;i++)m_ulValue[i]=rand()*0x10000+rand();
    m_ulValue[0]=m_ulValue[0]|1;
	for(i=m_nLength-1;i>0;i--)
	{
		m_ulValue[i]=m_ulValue[i]<<1;
		if(m_ulValue[i-1]&0x80000000)m_ulValue[i]++;
	}
	m_ulValue[0]=m_ulValue[0]<<1;
	m_ulValue[0]++;
    for(i=0;i<550;i++){if(Mod(PrimeTable[i])==0)goto begin;}
    big_int S,A,I,K;
    K.Mov(*this);
	K.m_ulValue[0]--;
    for(i=0;i<5;i++)
	{
        A.Mov(rand()*rand());
        var_div=K.Div(2);
        S.Mov(var_div);
	    //S.Mov(K.Div(2));
		var_RsaTrans=A.RsaTrans(S,*this);
		I.Mov(var_RsaTrans);
	    //I.Mov(A.RsaTrans(S,*this));
	    if(((I.m_nLength!=1)||(I.m_ulValue[0]!=1))&&(I.Cmp(K)!=0))goto begin;
	}
}