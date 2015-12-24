
#include "net_telesing_scomm_android_core_DemI.h"
//#include <jni.h>
#include ".\core\dem.h"
/******************************************************************************
名称：申明函数
功能：申明函数
******************************************************************************/
int native_test_func();
void  test_thread();
/******************************************************************************
名称：全局变量
功能：全局变量
******************************************************************************/
/******************************************************************************
名称：test_thread
功能：test_thread
参数：
返回：
备注：

******************************************************************************/
void * test_thread_func(void* arg)
 {


	char out_str[1024];
 //	 LOGW("[test_thread_func] start");
 	 max(1,2,NULL,3);
	/*
	for(int i=0;i<5;i++)
	{
		JNIEnv* env_;
		gJavaVM_->AttachCurrentThread(&env_, NULL);
		jmethodID mid = env_->GetMethodID(gJavaClass_, "synced", "()V");
		env_->CallVoidMethod(gJavaObj_, mid);
		gJavaVM_->DetachCurrentThread();

		sleep(1);
	}*/
//	printf("[test_thread_func] over\n");
}
void  test_thread()
{
	pthread_t    g_thread_1;
	//synced();
	if  (pthread_create(&g_thread_1, NULL,  &test_thread_func, (void*)0) != 0 )
	{ }
}
/******************************************************************************
名称：native_test_func
功能：测试加载so调用它的函数
参数：
返回：
备注：
******************************************************************************/
int native_test_func()
{
	char str1[100];
    int a=44,b=176;
    short *c=(short*)malloc(sizeof(short)*1024);
    for(int i=0;i<1024;i++)
    {
    	c[i]=(short)i;
    }

    int d=1024;
    return 0;
}

/******************************************************************************
名称：Java_net_telesing_scomm_android_core_DemI_init
功能：初始化
参数：
返回：
备注：

*加载动态库
*在java层使用System.loadLibrary("dem");加载,存放于apk\obj\local\armeabi\目录自动打包
******************************************************************************/
JavaVM* gJavaVM_;
jobject gJavaObj_;
jclass  gJavaClass_;
/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    testFun1
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_telesing_scomm_android_core_DemI_testFun1
  (JNIEnv *env, jobject thiz){

	//LOGW("[DemI][Java_net_telesing_scomm_android_core_DemI_testFun1]");

	//! 通过dem调用nactive调用回调函数
	//synced();
	test_thread();
/*
	JNIEnv* env_;
	gJavaVM_->AttachCurrentThread(&env_, NULL);
	jmethodID mid = env_->GetMethodID(gJavaClass_, "success", "(I)V");
	env_->CallVoidMethod(gJavaObj_, mid,12341234);

	jmethodID mid2 = env_->GetMethodID(gJavaClass_, "success_s", "(Ljava/lang/String;)V");
	if(mid2!=0)
	{
		char str1[]="adsfasdf";
		env_->CallVoidMethod(gJavaObj_, mid2,env_->NewStringUTF(str1));
	}
*/
	return 0;
}

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    testFun2
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_telesing_scomm_android_core_DemI_testFun2
  (JNIEnv * env, jobject thiz, jint i){

//	LOGW("Java_net_telesing_scomm_android_core_DemI_testFun2");
	test_thread();
	return 0;
}

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    testFun3
 * Signature: ([SI)Z
 */
JNIEXPORT jboolean JNICALL Java_net_telesing_scomm_android_core_DemI_testFun3(JNIEnv* env,\
												jobject thiz ,jshortArray data,jint data_size)
{
	//LOGW("[DemI][Java_net_telesing_scomm_android_core_DemI_testFun3]");

		JNIEnv* env_;
		gJavaVM_->AttachCurrentThread(&env_, NULL);
		jmethodID mid = env_->GetMethodID(gJavaClass_, "synced", "()V");
		env_->CallVoidMethod(gJavaObj_, mid);
		//gJavaVM_->DetachCurrentThread();

		return 1;
}
//JNIEnv* env,jclass clazz,jshortArray data,jint data_size
/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    init
 * Signature: (IDI[D[DI)I
 */
JNIEXPORT jint JNICALL Java_net_telesing_scomm_android_core_DemI_init(JNIEnv* env, jobject obj,
																	jint n_v, jdouble st_v, \
																	jint gap_v, jdoubleArray sFeq_v, \
																	jdoubleArray eFeq_v,jint feq_size)
{
	//LOGW("[DemI][JNICALL Java_net_telesing_scomm_android_core_DemI_init]");
	env->GetJavaVM(&gJavaVM_);

	gJavaObj_ = env->NewGlobalRef(obj);//change by lee

	jclass javaClass = env->GetObjectClass(obj);
	gJavaClass_ = (jclass)env->NewGlobalRef(javaClass);

	jdouble *carr1 = env->GetDoubleArrayElements( sFeq_v, NULL);
	jdouble *carr2 = env->GetDoubleArrayElements( eFeq_v, NULL);

	dem_init(gJavaVM_,gJavaObj_,gJavaClass_,n_v,st_v,gap_v,carr1,carr2,feq_size);

	env->ReleaseDoubleArrayElements(sFeq_v,carr1,0);
	env->ReleaseDoubleArrayElements(eFeq_v,carr2,0);
//	LOGW("[DemI][JNICALL Java_net_telesing_scomm_android_core_DemI_end]");
	return 1;
}
/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    start
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_telesing_scomm_android_core_DemI_start
(JNIEnv* env,jobject thiz){
//	LOGW("[DemI][JNICALL Java_net_telesing_scomm_android_core_DemI_start]");
	start();
}

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_telesing_scomm_android_core_DemI_stop(JNIEnv* env,jobject thiz)
{
	stop();
}

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    getStatus
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_telesing_scomm_android_core_DemI_getStatus(JNIEnv* env,jobject thiz)
{
	int re=getStatus();
	return re;
}

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    getBufferSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_telesing_scomm_android_core_DemI_getBufferSize(JNIEnv* env,jobject thiz)
{
//	LOGW("[DemI][Java_net_telesing_scomm_android_core_DemI_getBufferSize]");
	int re=getBufferSize();
	return re;
}

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    resizeBuffer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_telesing_scomm_android_core_DemI_resizeBuffer(JNIEnv* env,jobject thiz,jint size)
{
//	LOGW("[DemI][Java_net_telesing_scomm_android_core_DemI_resizeBuffer]");
	setBufferSize(size);
}

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    clearBuffer
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_telesing_scomm_android_core_DemI_clearBuffer(JNIEnv* env,jobject thiz)
{
//	LOGW("[DemI][Java_net_telesing_scomm_android_core_DemI_clearBuffer]");
	clearBuffer();
}

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    wirte
 * Signature: ([SI)Z
 */
JNIEXPORT jboolean JNICALL Java_net_telesing_scomm_android_core_DemI_wirte(JNIEnv* env,jobject thiz,jshortArray data,jint data_size)
{
//	LOGW("[DemI][Java_net_telesing_scomm_android_core_DemI_wirte]");

	//! 获取传入数组的长度
	//jsize len = (*env)->GetArrayLength(env, data);

	jshort *carr;
	carr = env->GetShortArrayElements( data, NULL);
	int re=wirte(carr,data_size);
	env->ReleaseShortArrayElements(data,carr,0);
	return re;
}
/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    fillRate
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_net_telesing_scomm_android_core_DemI_fillRate(JNIEnv* env,jobject thiz)
{
//	LOGW("[DemI][Java_net_telesing_scomm_android_core_DemI_fillRate]");
	jdouble re=fillRate();
	return re;
}
/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    getRegSignal
 * Signature: ()[S
 */
JNIEXPORT jshortArray JNICALL Java_net_telesing_scomm_android_core_DemI_getRegSignal(JNIEnv* env,jobject thiz)
{
	char out_str[100];
	int len=0;
	short *getRegSignal_p;
	getRegSignal_p=getRegSignal(len);//修改后的代码
	jshortArray ret_short_arr=env->NewShortArray(len);
	env->SetShortArrayRegion(ret_short_arr,0,len,getRegSignal_p);
	return ret_short_arr;
}



