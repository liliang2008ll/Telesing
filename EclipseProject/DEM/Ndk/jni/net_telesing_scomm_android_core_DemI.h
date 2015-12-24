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

#ifndef _Included_net_telesing_scomm_android_core_DemI
#define _Included_net_telesing_scomm_android_core_DemI
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    testFun1
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_telesing_scomm_android_core_DemI_testFun1
  (JNIEnv *, jobject);

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    testFun2
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_telesing_scomm_android_core_DemI_testFun2
  (JNIEnv *, jobject, jint);

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    testFun3
 * Signature: ([SI)Z
 */
JNIEXPORT jboolean JNICALL Java_net_telesing_scomm_android_core_DemI_testFun3
  (JNIEnv *, jobject, jshortArray, jint);

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    init
 * Signature: (IDI[D[DI)I
 */
JNIEXPORT jint JNICALL Java_net_telesing_scomm_android_core_DemI_init
  (JNIEnv *, jobject, jint, jdouble, jint, jdoubleArray, jdoubleArray, jint);

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    start
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_telesing_scomm_android_core_DemI_start
  (JNIEnv *, jobject);

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_telesing_scomm_android_core_DemI_stop
  (JNIEnv *, jobject);

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    getStatus
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_telesing_scomm_android_core_DemI_getStatus
  (JNIEnv *, jobject);

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    getBufferSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_telesing_scomm_android_core_DemI_getBufferSize
  (JNIEnv *, jobject);

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    resizeBuffer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_telesing_scomm_android_core_DemI_resizeBuffer
  (JNIEnv *, jobject, jint);

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    clearBuffer
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_telesing_scomm_android_core_DemI_clearBuffer
  (JNIEnv *, jobject);

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    wirte
 * Signature: ([SI)Z
 */
JNIEXPORT jboolean JNICALL Java_net_telesing_scomm_android_core_DemI_wirte
  (JNIEnv *, jobject, jshortArray, jint);

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    fillRate
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_net_telesing_scomm_android_core_DemI_fillRate
  (JNIEnv *, jobject);

/*
 * Class:     net_telesing_scomm_android_core_DemI
 * Method:    getRegSignal
 * Signature: ()[S
 */
JNIEXPORT jshortArray JNICALL Java_net_telesing_scomm_android_core_DemI_getRegSignal
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
