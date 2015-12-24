


#include<stdio.h>
#include<stdlib.h>
#include<dlfcn.h>
#include <unistd.h>		//sleep
#include <pthread.h>	//Ïß³Ì

#include <string.h>
#include <jni.h>

#include <android/log.h>
#define LOG_TAG "xx"
#define LOGW(a )  __android_log_write(ANDROID_LOG_WARN,LOG_TAG,a)


#ifndef NATIVE_CALL
#define NATIVE_CALL
#ifdef __cplusplus
extern "C"
{
#endif

jint Java_net_telesing_scomm_android_core_DemI_testFun1(JNIEnv* env,jclass clazz);
jint Java_net_telesing_scomm_android_core_DemI_testFun2( JNIEnv* env,jobject thiz,jint i);
jboolean Java_net_telesing_scomm_android_core_DemI_testFun3(JNIEnv* env,jclass clazz,jshortArray data,jint data_size);

jint Java_net_telesing_scomm_android_core_DemI_init(JNIEnv* env, jobject obj,
																				jint n_v, jdouble st_v, jint gap_v, jdoubleArray sFeq_v, jdoubleArray eFeq_v,jint feq_size
																				);
void Java_net_telesing_scomm_android_core_DemI_start(JNIEnv* env,jclass clazz);
void Java_net_telesing_scomm_android_core_DemI_stop(JNIEnv* env,jclass clazz);
jint Java_net_telesing_scomm_android_core_DemI_getStatus(JNIEnv* env,jclass clazz);
jint Java_net_telesing_scomm_android_core_DemI_getBufferSize(JNIEnv* env,jclass clazz);
void Java_net_telesing_scomm_android_core_DemI_resizeBuffer(JNIEnv* env,jclass clazz,jint size);
void Java_net_telesing_scomm_android_core_DemI_clearBuffer(JNIEnv* env,jclass clazz);
jboolean Java_net_telesing_scomm_android_core_DemI_wirte(JNIEnv* env,jclass clazz,jshortArray data,jint data_size);
jdouble Java_net_telesing_scomm_android_core_DemI_fillRate(JNIEnv* env,jclass clazz);
jshortArray Java_net_telesing_scomm_android_core_DemI_getRegSignal(JNIEnv* env,jclass clazz);

#ifdef __cplusplus
}
#endif
#endif


