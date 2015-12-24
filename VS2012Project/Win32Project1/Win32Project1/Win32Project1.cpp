// Win32Project1.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

// TODO:  在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
#include "jni.h"
#include "net_telesing_tsdk_tlib_Acomms.h"
#include "jni_md.h"

JNIEXPORT jint JNICALL Java_net_telesing_tsdk_tlib_Acomms_bindCer
(JNIEnv *env, jobject obj, jstring str, jint p){

	return (p*p);
}

JNIEXPORT jint JNICALL Java_net_telesing_tsdk_tlib_Acomms_unbindCer
(JNIEnv *env, jobject obj, jstring str, jint p){

	return (p + p);
}

JNIEXPORT jint  Java_net_telesing_jni_demo_JNIDemo_getWord
(JNIEnv *env, jobject obj, jstring str, jint p){
	return 0xffff;
}
