



#include <unistd.h>		//sleep
#include <pthread.h>	//线程
#include<stdio.h>		//dlopen
#include<stdlib.h>
#include<math.h>

#include "dem.h"
#include "bch.h"
#include "buffer_c.h"
#include "corr_helper.h"
#include "./fft/Transformer.h"
#include "./fft/Complexs.h"
#include "rsa_use.h"

#include "./sab_code/CodeS0.h"
#include "./sab_code/CodeS1.h"
#include "./sab_code/CodeA00.h"
#include "./sab_code/CodeA01.h"
#include "./sab_code/CodeA10.h"
#include "./sab_code/CodeA11.h"
#include "./sab_code/CodeB00.h"
#include "./sab_code/CodeB01.h"
#include "./sab_code/CodeB10.h"
#include "./sab_code/CodeB11.h"

/******************************************************************************
名称：申明函数
功能：申明函数
******************************************************************************/

int test_func_real();
int data_init(int n_v, double st_v, int gap_v, double *sFeq_v, double *eFeq_v,int feq_size);

void * thread_1(void* arg);
void doStart();
void doStop();
void recognize();
bool haveSync();
Complexs *findFD(int type, int num);
Complexs *calcY(int offset) ;
double calcArea(Complexs *X, Complexs *Y) ;
int calcK(double feq);
int load_sab();
bool parseData() ;
bool analyze(int offset, int type) ;
int decode(bool *bits,int bits_size,int bits_one_len);
int bytes2int(unsigned char *b) ;
int sleepMS() ;
int data_close();

void synced();
void succRsv(char *data);
void succEnc(char *data);
void failure(int data);
/******************************************************************************
名称：声明常量
功能：声明常量
******************************************************************************/
#define STARTING   1
#define STARTED   2
#define STOPING   3
#define STOPED   4
#define FRAME_SYMBOL_LEN 31
#define Config_OFFSET_RATES_length 4 

int DEF_BUFFER_RATE = 400;
int RESERVE_SN_START = -65536;
int RESERVE_SN_END = 65536;
int SAMPLEING_RATE = 44100;
double SAMPLE_DURATION = 1000 / SAMPLEING_RATE;

double OFFSET_RATES[] = {0.25, 0.5, 0, -0.25};
int TLR = 8;
	
bool V00[] = {false, false};
bool V01[] = {false, true};
bool V10[] = {true, false};
bool V11[] = {true, true};
/******************************************************************************
名称：全局变量
功能：全局变量
******************************************************************************/
int times_=0,old_times_=0;				//! 调试打印次数
int status = STOPED;					//! 线程状态
int GAP;
int syncFail;
Complexs *fdMap[3][4];
int N;
int chCount;
short *regSignal;
int regLen;
int ch;
int *SKs;
int *EKs;
int symbolLen;
double ST;
bool bit[2];
int offsets[Config_OFFSET_RATES_length];
int parseSucc;
buffer_c *bc;
Complexs *calcY_fft_c;
Transformer *tsf ;
rsa_use *ru;
//! 线程状态-锁

#define thread_wait 		1
#define thread_run 			2
#define thread_stoping  	3
#define thread_stop   		4
int thread_1_stause=thread_wait;
int data_init_yn=thread_stop;

//JavaVM * gJavaVM_d;
//jobject  gJavaObj_d;
//jclass  gJavaClass_d;
/******************************************************************************
名称：max
功能：测试函数
参数： 
返回：
备注：
******************************************************************************/
int max(int x,int y,short buf_p[],int buf_size)
{
	char out_str[100];
	sprintf(out_str,"[dem][max] cpp x %d y %d\n",x,y);
//	LOGW(out_str);
	test_func_real();
	return 123;
}

 

/******************************************************************************
名称：start
功能：开始解调
参数： 
返回：
备注：
******************************************************************************/
void start()
{
//	LOGW("[dem][start]");
	
	switch (status) {
		case STARTING:
			while (status == STARTING) {
				usleep(100*1000);
			}
			doStop();
			while (status == STOPING) {
				usleep(100*1000);
			}
			doStart();
			break;
		case STARTED:
			doStop();
			while (status == STOPING) {
				usleep(100*1000);
			}
			doStart();
			break;
		case STOPING:
			while (status == STOPING) {
				usleep(100*1000);
			}
			doStart();
			break;
		case STOPED:
			doStart();
			break;
	}
}

/******************************************************************************
名称：stop
功能：停止解调
参数： 
返回：
备注：
******************************************************************************/
void stop()
{
//	LOGW("[dem][stop]");
	
	switch (status) {
		case STARTING:
			while (status == STARTING) {
				usleep(100*1000);
			}
			doStop();
			break;
		case STARTED:
			doStop();
			break;
		case STOPING:
			break;
		case STOPED:
			break;
	}
}

/******************************************************************************
名称：stop
功能：停止解调
参数： 
返回：
备注：
******************************************************************************/
//获得当前解调状态
int getStatus()
{
	//LOGW("[dem][getStatus]");
	return status;
}

/******************************************************************************
名称：getBufferSize
功能：获取缓冲区大小
参数： 
返回：
备注：
******************************************************************************/
int getBufferSize()
{
	//LOGW("[dem][getBufferSize]");
	//return signalBuffer.size();
	return bc->size();
}

/******************************************************************************
名称：getRegSignal
功能：获得当前解析的声码的采样
参数： 
返回：
备注：

******************************************************************************/
short* getRegSignal(int &len_v)
{
	//LOGW("[dem][getRegSignal]");
	len_v=regLen;
	short *ddd=regSignal;
	return ddd;
}

/******************************************************************************
名称：fillRate
功能：缓冲区填充率，0：空；1：满
参数： 
返回：
备注：

******************************************************************************/
double fillRate()
{
	double d=(double)bc->fillSize()/(double)bc->size();
	return d;
}

/******************************************************************************
名称：setBufferSize
功能：重置缓冲区大小
参数： 
返回：
备注：

******************************************************************************/
void setBufferSize(int size)
{
	//char str1[100];
	//sprintf(str1,"[dem][setBufferSize] size:%d",size);
	//LOGW(str1);
	bc->setBufferSize(size);
}

/******************************************************************************
名称：clearBuffer
功能：清空缓冲区
参数： 
返回：
备注：

******************************************************************************/
void clearBuffer()
{
	//char str1[100];
	//sprintf(str1,"[dem][clearBuffer] ");
	//LOGW(str1);
	bc->clearBuffer();
}


/******************************************************************************
名称：wirte
功能：向缓冲区写入数据
参数： 
返回：
备注：

******************************************************************************/
int wirte(short data[],int data_size)
{
	bc->write(data,data_size);
	return 0;
}

/******************************************************************************
名称：doStart
功能：启动一个业务线程
参数： 
返回：
备注：

******************************************************************************/
void  doStart()   
{  
	//LOGW("[dem][doStart]"); 
	//printf("[doStart] start\n");
	pthread_t    g_thread_1;
	if  (pthread_create(&g_thread_1, NULL,  &thread_1, (void*)0) != 0 ) 
	{ }
}

/******************************************************************************
名称：thread_1
功能：thread_1
参数： 
返回：
备注：

******************************************************************************/
void * thread_1(void* arg)
 {
 	//! 信号量防止多次同时启动该线程
 	if(thread_1_stause==thread_wait || thread_1_stause==thread_stop)
 	{
 			thread_1_stause=thread_run;
 	}
 	else
 	{		
	 	 return NULL;
 	}
 	

	status =STARTING;  
	status  =  STARTED;  
	while(status   ==  STARTED)  
	{  
		//! 计算等待时间
		int  sleepMS_   =sleepMS();
		if (sleepMS_  == 0) 
		{
			recognize();
		} 
		else 
		{
			usleep(sleepMS_*1000);
		}
	}
	status = STOPED; 
	
	//! 信号量防止多次同时启动该线程
	thread_1_stause=thread_stop;
	//printf("[thread_1] over\n");
//	LOGW("[dem][thread_1] over");
}

/******************************************************************************
名称：doStop
功能：停止业务线程
参数： 
返回：
备注：

******************************************************************************/
void doStop()
{
	//LOGW("[dem][doStop]");
	status = STOPING;
}

/******************************************************************************
名称：dem_init
功能：回调初始化
参数： 
返回：
备注：回调方法,callback native_call.so

******************************************************************************/
//int dem_init(JavaVM * gJavaVM_v,jobject  gJavaObj_v,jclass  gJavaClass_v,
  int dem_init( int n_v, double st_v, int gap_v, double *sFeq_v, double *eFeq_v,int feq_size)
{
	//! 保存vm虚拟机 
//	gJavaVM_d=gJavaVM_v;
//	gJavaObj_d=gJavaObj_v;
//	gJavaClass_d=gJavaClass_v;
    //! 初始化数据 
    /*
	int n_v=2048;
	double st_v=0.15;						//! 相似度
	int gap_v=512;
	int feq_size=3;				//!  信道数
	//! 信道
	double sFeq[]={16, 12, 8, 4};//{5.5125};
	double eFeq[]={20, 16, 12, 8};//{11.025};
	*/
    data_init(n_v,st_v,gap_v,sFeq_v,eFeq_v,feq_size);
    
	
}

//当同步到声码时会回调用此方法
void synced()
{
	printf("[synced]\n");

}

//当同步到声码后解析成功时会回调用此方法
void succRsv(char *data)
{
	printf("[succRsv] %s\n",data);

	
}

//当同步到声码后解析成功时会回调用此方法
void succEnc(char *data)
{

	
}

//当同步到声码后解析失败时会回调用此方法  
void failure(int data)
{


}


/******************************************************************************
名称：test_func_reg
功能：测试各个方法是否正常
参数： 
返回：
备注：fft bch buffer 

******************************************************************************/
int test_func()
{

	return 0;
}
/******************************************************************************
名称：read_file
功能：读取文件
参数： 
返回：
备注：fft bch buffer 

******************************************************************************/
int read_file(char *filePath,char *file_p,int *file_size_v)
{
	int file_size=0;  
	FILE *fp; 
	// r+
    if((fp = fopen (filePath,"r+"))==NULL)   
    {   
        return -1;
    }   
    fseek(fp,0L,SEEK_END);  
    file_size=ftell(fp);  
    if(file_size==0)
    	{   
        fclose(fp);   
        return -2;
    }   
    fseek(fp,0L,SEEK_SET);
    fread(file_p,file_size,1,fp);  
    file_p[file_size]=0x00;    
    fclose(fp); 
    memcpy(file_size_v,&file_size,sizeof(int));

    return 1;
}

/******************************************************************************
名称：data_init
功能：数据初始化
参数： n_v FFT点数,st_v 相似度,
返回：
备注：
******************************************************************************/
int data_init(int n_v, double st_v, int gap_v, double *sFeq_v, double *eFeq_v,int feq_size)
{
	if(data_init_yn==thread_stop)
	{
		data_init_yn=thread_run;
		char str1[100];
		sprintf(str1,"[dem_init] successful\n");

	}
	else if(data_init_yn==thread_run)
	{

		data_init_yn=thread_stoping;
		data_close();
		data_init_yn=thread_run;
	}
	else if(data_init_yn==thread_stoping)
	{

		return -1;	
	}
	//! 初始化全局变量
	N=n_v;//2048
	ST=st_v;//0.15;						//! 相似度
	GAP=gap_v;//512;
	chCount=feq_size;				//!  信道数
	
	int maxOffest = 0;//1024;
	int minOffest = 0;//512;
	for (int i = 0; i < Config_OFFSET_RATES_length; i++) 
	{
		offsets[i] = (int) (N * OFFSET_RATES[i]);
		maxOffest = offsets[i] > maxOffest ? offsets[i] : maxOffest;
		minOffest = offsets[i] < minOffest ? offsets[i] : minOffest;
	}

	SKs = new int[chCount];
	EKs = new int[chCount];

	//4个信道
	for (int i = 0; i < chCount; i++) 
	{
		SKs[i] = calcK(sFeq_v[i]);//获取频率对应的点 //16K,12K,8K,4K
		EKs[i] = calcK(eFeq_v[i]); //20K,16K,12K,8K
	}	
	int tl = N / TLR; // 8
	symbolLen = tl * 2 + N;
	regLen = symbolLen * FRAME_SYMBOL_LEN + N + maxOffest;
	regSignal=(short*)malloc(sizeof(short)*regLen*2);
	parseSucc = symbolLen * FRAME_SYMBOL_LEN + tl; 
	//每一次同步失败后移动数据长度?
	syncFail=GAP;

	//! fft初始化
	tsf =new Transformer(N);
	load_sab();
    //! 函数专用返回类
	calcY_fft_c=new Complexs(N);
	//! 创建缓冲区
	bc=new buffer_c(N*DEF_BUFFER_RATE);
	//! rsa加密
	ru=new rsa_use();
	return 0;
}

/******************************************************************************
名称：data_close
功能：数据释放
参数： 
返回：
备注：
******************************************************************************/
int data_close()
{
	delete ru;
	delete []SKs;
	delete []EKs;
	free(regSignal);
	delete tsf;
	delete calcY_fft_c;
	delete bc;
	
	for(int i=0;i<3;i++)
	{
		for(int k=0;k<4;k++)
		{
			delete fdMap[i][k]	;
		}
	}
	
	return 0;	
}


/******************************************************************************
名称：test_func_real
功能：实际逻辑测试函数
参数： 
返回：
备注：fft bch buffer 

	//! 0.初始化
	//! 1.读取wav
	//! 2.创建缓冲区
	//! 3.写入缓冲区
	//! 4.线程开始
	//! 5.线程结束
	 
******************************************************************************/
int test_func_real()
{

	//! 读取 /sdcard/a1.wav
	char *file_p=(char*)malloc(1024*1024*2);
	int file_size=0;
	char file_path[]="/data/local/tmp/a2.wav";
	int err1=read_file(file_path,file_p,&file_size);
	printf("[file size:]%d \n",file_size); 
	char out_str[100];
	sprintf(out_str,"[file size:] filesize:%d :data_size%d  \n",file_size,(int)((file_size-44)/sizeof(short)));
//	LOGW(out_str);
	if(err1<0)
		return -1;
	//! wav头
	file_p+=44;
	file_size-=44;
	
	int n_v=2048;
	double st_v=0.15;						//! 相似度
	int gap_v=512;
	int feq_size=4;				//!  信道数
	//! 信道
	double sFeq[]={16, 12, 8, 4};//{5.5125};
	double eFeq[]={20, 16, 12, 8};//{11.025};
    data_init(n_v,st_v,gap_v,sFeq,eFeq,feq_size);
	
	//! 写入缓冲区
	short *short_file_p=(short *)malloc(1024*1024*sizeof(short));
	memset(short_file_p,0,1024*1024*sizeof(short));
	short temp_a;
	for(int i=0;i<(file_size/sizeof(short));i++)
	{
		memcpy(&temp_a,file_p+i*sizeof(short),sizeof(short));
		short_file_p[i]=temp_a;
	}
	printf("[init] write_buf_Len:%d\n",(int)(file_size/sizeof(short)));
	//bc->write(short_file_p,file_size/sizeof(short));
	
	//! dem线程开始  
	start(); 

	//sleep(10);
	short *var1=new short[1792];
	int for_ik=0;
	for(int k=0;k<1000;k++)
	{
		for(int i=0;i<290;i++)
		{
			memcpy(var1,file_p+i*1792*sizeof(short),1792*sizeof(short));
			bc->write(var1,1792);
			usleep(41*1000);
			for_ik++; 
		} 
		printf("------buf %lf---------------%d----------k:%d ----\n",fillRate(),for_ik,k);
	}
	printf("-------------------------------------[for_ik]%d-\n",for_ik); 

	stop();
	
	return 0;
}

/******************************************************************************
名称：read_sab
功能：加载样值
参数： 
返回：
备注：fft bch buffer  
******************************************************************************/
int load_sab()
{
	int c_size=sizeof(S0_TD)/sizeof(double);//16384;
	fdMap[0][0]=tsf->fft(S0_TD,c_size);
	c_size=sizeof(S1_TD)/sizeof(double);//16384;
	fdMap[0][1]=tsf->fft(S1_TD,c_size);
	c_size=sizeof(A00_TD)/sizeof(double);//16384;
	fdMap[1][0]=tsf->fft(A00_TD,c_size);
	c_size=sizeof(A01_TD)/sizeof(double);//16384;
	fdMap[1][1]=tsf->fft(A01_TD,c_size);
	c_size=sizeof(A10_TD)/sizeof(double);//16384;
	fdMap[1][2]=tsf->fft(A10_TD,c_size);
	c_size=sizeof(A11_TD)/sizeof(double);//16384;
	fdMap[1][3]=tsf->fft(A11_TD,c_size);
	c_size=sizeof(B00_TD)/sizeof(double);//16384;
	fdMap[2][0]=tsf->fft(B00_TD,c_size);
	c_size=sizeof(B01_TD)/sizeof(double);//16384;
	fdMap[2][1]=tsf->fft(B01_TD,c_size);
	c_size=sizeof(B10_TD)/sizeof(double);//16384;
	fdMap[2][2]=tsf->fft(B10_TD,c_size);
	c_size=sizeof(B11_TD)/sizeof(double);//16384;
	fdMap[2][3]=tsf->fft(B11_TD,c_size);
	return 0;
}

/******************************************************************************
名称：recognize
功能：实际逻辑测试函数
参数： 
返回：
备注：fft bch buffer 
******************************************************************************/
void recognize()
{
	char out_str[100];
	
	if (haveSync()) //找到头	S0
	{
		sprintf(out_str,"[haveSync]  successful!\n"); 

		
		if (parseData()) //分析数据
		{
			//sprintf(out_str,"[parseData] successful! doSkip:%d\n",parseSucc); 
			//printf("%s",out_str);
			//LOGW(out_str);
			bc->doSkip(parseSucc);//缓冲区仔寻找
		}
		else 
		{
			bc->doSkip(syncFail);
		}
	}
	else
	{
		bc->doSkip(syncFail);
	}
}

/******************************************************************************
名称：haveSync
功能：样本S头部
参数： 
返回：
备注：fft bch buffer 
******************************************************************************/
bool haveSync()
{
	Complexs *X = findFD(0, 1);//获取样本信息 //S01样本头?
	Complexs *Y = calcY(0);//计算样本开头的数据的FFT

	for (int i = 0; i < chCount; i++) //信道
	{
		ch = i;
		if (calcArea(X, Y) < ST) //计算相关性是否在给定的范围之内对比头数据S0
		{
			//! 调试输出
			printf("[regLen]%d\n",regLen); 
			bc->preview(regSignal,regLen, 0);//跳转一块继续计算
			return true;
		}
	}
	memset(regSignal,0,sizeof(short)*regLen*2); 
	return false;
}

/******************************************************************************
名称：calcArea
功能：对比 X Y, X为样本头数据,Y为需要对比的数据
参数： 
返回：
备注：fft bch buffer
******************************************************************************/
double calcArea(Complexs *X, Complexs *Y) 
{

	char out_str[1024];

	Complexs *Z = new Complexs(N);

	//16  12  8  4
	//
	//20  16  12 8
	//X共轭*Y
	for (int i = SKs[ch]; i <= EKs[ch]; i++) 
	{
		Z->res[i] = X->res[i] * Y->res[i] + X->ims[i] * Y->ims[i];//Z实部

		Z->ims[i] = X->res[i] * Y->ims[i] - Y->res[i] * X->ims[i];
	}

	//! ifft 
	double r[N];
	int r_len;
	tsf->ifft(Z,r,r_len);//FFT反变换求原信号,r里面存放的是给定信号的时阈信号
	delete Z;
	

//找时信号中幅度最小的?
	double peakValue = 0;
	for (int i = 0; i < N; i++) 
	{
		if (peakValue < r[i]) 
		{
			peakValue = r[i];
		}
	}

//求面积
	double area = 1;
	for (int i = 0; i < N; i++) 
	{
		//! abs返回绝对值
		area = area +fabs(r[i]);
	}

//均值算占有率
	area = area / peakValue / N;
	
	return area;
}

/******************************************************************************
名称：findFD
功能：获取样本数据
参数： 
返回：
备注：

******************************************************************************/
Complexs *findFD(int type, int num) 
{
	return  fdMap[type][num];
}

/******************************************************************************
名称：calcY
功能：获取缓冲区数据
参数： 
返回：
备注：

******************************************************************************/
Complexs *calcY(int offset) 
{
	//! 读取缓存
	short td[N];//first offset = 0; N=4096
	bc->preview(td,N, offset) ;//read 4096 point to td[N]
	
	//!fft
	double td_d[N];
	for(int i=0;i<N;i++)
	{
		td_d[i]=(double)td[i];
	}
	
	Complexs *fd = tsf->fft(td_d,N);
	calcY_fft_c->set(fd->res,fd->ims,N);
	delete fd;
	return calcY_fft_c;//fft data
}

/******************************************************************************
名称：calcK
功能：获取频率对应点数
参数： 
返回：
备注：

******************************************************************************/
int calcK(double feq)
 {
 	//! 需要精确并且四舍五入
	return (int) (feq * 1000 / 44100 * N);//采用4410采样率,2048个点 44100----->2048 个点 0 - 0
}

/******************************************************************************
名称：parseData
功能：如果声码样本对比成功,则返回傅立叶变换后的bch值
参数： 
返回：
备注：

******************************************************************************/
bool parseData() 
{
	//! 回调函数,找到头数据S0??
	synced();
	int bad = 0;
	bool dataBits[2][FRAME_SYMBOL_LEN];
	
	for (int i = 0; i < FRAME_SYMBOL_LEN; i++) 
	{
		if (!analyze((i + 1) * symbolLen, i % 2)) 
		{
			bad++;
			//todo:调整bad数值
			if (bad > 6) 
			{
				failure(0);
				return false;
			}
		}
		dataBits[0][i] = bit[0];
		dataBits[1][i] = bit[1];
	}

	bool tem_var1[2*FRAME_SYMBOL_LEN];
	int tem_var1_nu=0;
	for(int j=0;j<2;j++)
	{
		for(int i=0;i<FRAME_SYMBOL_LEN;i++)
		{
			tem_var1[tem_var1_nu]=dataBits[j][i];
			tem_var1_nu++;
		}
	}
	int data=decode(tem_var1,2*FRAME_SYMBOL_LEN,FRAME_SYMBOL_LEN);
	//! 调试输出
	//char out_str[100];
	//sprintf(out_str,"[parseData] %d",data);
	//LOGW(out_str);

	if (data >= RESERVE_SN_START && data <= RESERVE_SN_END)
	 {
		char data_str[100];
		sprintf(data_str,"%d",data);
		succRsv(data_str);//todo:加密
	} 
	else
	{
		//! java中此处传入的data为String
		char data_str[100];
		sprintf(data_str,"%d",data);
	//	succEnc(data_str);//todo:加密
		succRsv(data_str);//todo:加密
	}
	
	return true;
}

/******************************************************************************
名称：analyze
功能：智能动态选择开始位置
参数： 
返回：
备注：

******************************************************************************/
bool analyze(int offset, int type) 
{
	//! 与java版本-1 0 1sab不同,换为 0123
	type+=1;
	
	corr_helper ch;	
	correlation cl[4];
	correlation cl_all[Config_OFFSET_RATES_length];
	int cl_all_nu=0;
	
	for (int i = 0; i < Config_OFFSET_RATES_length; i++) 
	{
		Complexs *Y = calcY(offset + offsets[i]);
		
		double var0=calcArea(findFD(type, 0), Y);
		double var1=calcArea(findFD(type, 1), Y);
		double var2=calcArea(findFD(type, 2), Y);
		double var3=calcArea(findFD(type, 3), Y);
		
		//! 赋值
		cl[0].value=var0;
		cl[0].data[0]=V00[0];
		cl[0].data[1]=V00[1];
		cl[1].value=var1;
		cl[1].data[0]=V01[0];
		cl[1].data[1]=V01[1];
		cl[2].value=var2;
		cl[2].data[0]=V10[0];
		cl[2].data[1]=V10[1];
		cl[3].value=var3;
		cl[3].data[0]=V11[0];
		cl[3].data[1]=V11[1];
		//! 获取最小值
		//correlation *cl_var1=ch.min(cl,4);
		int min_nu=ch.min(cl,4);
		cl_all[cl_all_nu].value=cl[min_nu].value;
		cl_all[cl_all_nu].data[0]=cl[min_nu].data[0];
		cl_all[cl_all_nu].data[1]=cl[min_nu].data[1];
		cl_all_nu++;
		if (cl[min_nu].value < ST) 
		{
			bit[0]=cl[min_nu].data[0];
			bit[1]=cl[min_nu].data[1];
			return true;
		}
	}

	//! 找到数组16个数据里最小的那个
	int min_nu=ch.min(cl_all,Config_OFFSET_RATES_length);
	bit[0]=cl_all[min_nu].data[0];
	bit[1]=cl_all[min_nu].data[1];
	return false;
}

/******************************************************************************
名称：decode
功能：bch编码
参数： 
返回：
备注：

******************************************************************************/
int decode(bool *bits,int bits_size,int bits_one_len)
{
	bool bs[bits_one_len];
	unsigned char t[2] ;
	unsigned char bytes[4];
	for(int i=0;i<bits_size/bits_one_len;i++)
	{
		int msg = 0;
		memcpy(bs,bits+i*bits_one_len,bits_one_len);
		for(int j=0;j<bits_one_len;j++)
		{
			msg = msg | ((bs[j] ? 1 : 0) << bits_one_len - j - 1);
		}
		bch_decoder(msg,t);
		bytes[i * 2] = t[0];
		bytes[i * 2 + 1] = t[1];
	}
	int bi=bytes2int(bytes);
	return bi;
}

/******************************************************************************
名称：bytes2int
功能：bytes转int
参数： 
返回：
备注：

******************************************************************************/
int bytes2int(unsigned char *b) 
{
	int temp = 0;
	int res = 0;
	for (int x = 0; x < 4; x++) 
	{
		res <<= 8;
		temp = b[x] & 0xff;
		res |= temp;
	}
	//printf("[bytes2int] %d  \n",res);   
	return res;
}

/******************************************************************************
名称：sleepMS
功能：sleepMS
参数： 
返回：
备注：

******************************************************************************/
int sleepMS() 
{
	int delta = regLen - bc->fillSize();
	if (delta <= 0) 
	{
		return 0;
	}
	else
	{
		return (int) (delta * SAMPLE_DURATION + 1);
	}
}


/******************************************************************************
名称：test_new_fft
功能：test_new_fft
参数： 
返回：
备注：

******************************************************************************/
int test_new_fft()
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

/******************************************************************************
名称：main
功能：main
参数： 
返回：
备注：

******************************************************************************/
/*
int main()
{
	printf("[main] satrt\n");
	//test_new_fft();
	test_func_real(); 
	//demi de;
	printf("[main] over\n");
	return 0;  
}
*/
