// Win32Project1.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"

// TODO:  �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������
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
