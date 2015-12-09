// Telesing.cpp : Defines the exported functions for the DLL application.
//

#include "pch.h"
#include "jni.h"
#include "net_telesing_tsdk_tlib_Acomms.h"
#include "jni_md.h"

JNIEXPORT jint JNICALL Java_net_telesing_tsdk_tlib_Acomms_bindCer
(JNIEnv *env , jobject obj, jstring str,  jint p){

	return (p*p);
}

JNIEXPORT jint JNICALL Java_net_telesing_tsdk_tlib_Acomms_unbindCer
(JNIEnv *env, jobject obj, jstring str, jint p){

	return (p+p);
}

