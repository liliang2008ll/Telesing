#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <assert.h>
#include "net_telesing_tsdk_tlib_Acomms.h"
#include "tlib.h"

#ifdef __cplusplus
extern "C" {
#endif

//***************************************************************************
//!������
#define JNIREG_CLASS "net/telesing/tsdk/tlib/Acomms"

//**************************************************************************
#define INFOR(a)   __android_log_print(ANDROID_LOG_INFO, "NATIVE:",a);
//**************************************************************************
//��ຯ������
//����ֵ			 JNIָ��ֵ
#define JINT	 JNIEXPORT jint JNICALL
#define	JVOID	 JNIEXPORT void JNICALL
#define	JOBJECT	 JNIEXPORT jobject JNICALL
#define	JSTRING  JNIEXPORT jstring JNICALL
//**************************************************************************
//�����������ڴ��JAVA��ز���
typedef struct{

	JavaVM* 	  jvm;			//Java�߳�
	jobject 	  jobj;
	jclass  	  jclass;
	jmethodID     cb_recog_result; //�ص������б�

     jchar  	  first_start;  	//��ʼ������
     CER		  cer; 			//֤��
     Recog_status sys_status;	//״̬
     Recog_cfg 	  sys_cfg;	 	//����
}JAVA;

//�ص�����
typedef struct {
	const char* packagename;
	const char* name;
	const char* signature;
} JNICallBackMethod;

//֤��ǩ���ýṹ��
typedef struct {
	const char* name;
	const char* signature;
} CER_INFOR;
//ȫ�ֵ�javaѡ��
JAVA *g_java ;


static JNICallBackMethod g_cb_methods[] = {
		{"net/telesing/tsdk/tlib/Acomms","recogResult", "(Ljava/lang/String;Ljava/lang/String;I)V"},
};

//***************************************************************************
//�������ƣ�callback_java
//�������ܣ�ͬ���ص�����
//��������� char *section, 	֤���
//			char *data		������������
//			int times		��������ʱ�䵥λms
//��������� ��
//��	        ע ��
//
//
//**************************************************************************
void callback_java( char *section, char *data,int times){

	JNIEnv* input_env;

	g_java->jvm->AttachCurrentThread(&input_env, NULL);//��ȡ�̻߳���

	//���÷���ֵΪvoid�ķ���
	input_env->CallVoidMethod(  g_java->jobj, g_java->cb_recog_result,
								input_env->NewStringUTF(section),	//����֤����Ϣ
								input_env->NewStringUTF(data),	  	//����������Ϣ
								times);							  	//����ʱ����Ϣ
#ifdef _USED_IN_PTHREAD
	g_java->jvm->DetachCurrentThread();
#endif
}
#if 0
void ndk_test_call_fnx(char *debug_message){
	//@�½��ַ������ڲ���
	JNIEnv* env_;
	char str1[] = "test1,start....";
	char str2[] = "test2,start....";

	globe_JavaVM->AttachCurrentThread(&env_, NULL);

	//�ҵ�ָ���ĺ�����
	jmethodID mid = env_->GetMethodID(globe_JavaClass, "recogResult", "(Ljava/lang/String;Ljava/lang/String;I)V");
	//���÷���ֵΪvoid�ķ���
	env_->CallVoidMethod(globe_JavaObj, mid,env_->NewStringUTF(str1),env_->NewStringUTF(str2),100);

	globe_JavaVM->DetachCurrentThread();
	//״̬��ֵ
	recog_status.output_recog_state  = 100;
	recog_status.output_blank_buffer = 0.2;
	recog_status.output_ss           =0.99;
}



/*
 * Class:     net_telesing_tsdk_tlib_Acomms
 * Method:    startRecog
 * Signature: (Lnet/telesing/tsdk/tlib/Recog_cfg;)I
 */
//(Recog_cfg cfg)
//����
JNIEXPORT jint JNICALL Java_net_telesing_tsdk_tlib_Acomms_startRecog
  (JNIEnv *input_env, jobject input_obj, jobject recog_cfg){

	__android_log_print(ANDROID_LOG_INFO, "NDK:","START RECONG");

	//��ȡJava�е�ʵ����
	jclass objectClass = input_env->FindClass("net/telesing/tsdk/tlib/Recog_cfg");


	//��ȡ����ÿһ�������Ķ���
	jfieldID  jgap      = input_env->GetFieldID(objectClass, "gap", "I");
	jfieldID  jpoints   = input_env->GetFieldID(objectClass, "fft_points", "I");

	//�ı�ṹ������е���ֵ
	//input_env->SetIntField(recog_cfg,fft_gap,100);
	//��ȡ�ṹ�������int���͵ı���ֵ
	jint  temp_gap = input_env->GetIntField(recog_cfg,jgap);


	__android_log_print(ANDROID_LOG_INFO, "NDK:","gap = : %d", temp_gap);

	return 0;

}

/*
 * Class:     net_telesing_tsdk_tlib_Acomms
 * Method:    getRecogStatus
 * Signature: ()Lnet/telesing/tsdk/tlib/Recog_status;
 */
JNIEXPORT jobject JNICALL Java_net_telesing_tsdk_tlib_Acomms_getRecogStatus
  (JNIEnv *input_env, jobject obj)
{
	//��ȡJAVA�е�ʵ����
	 jclass objectClass = (input_env)->FindClass("net/telesing/tsdk/tlib/Recog_status");

	 //��ȡ����ÿһ�������Ķ���
	jfieldID j_ss			 = input_env->GetFieldID(objectClass, "ss", "D");
	jfieldID j_blank_buffer  = input_env->GetFieldID(objectClass, "blankBuffer", "D");
	jfieldID j_recog_stat    = input_env->GetFieldID(objectClass, "recogStat", "I");

	//�޸�״ֵ̬׼��JAVA��ȡ
	//״̬��ֵ
	recog_status.output_recog_state  = 100;
	recog_status.output_blank_buffer = 0.2;
	recog_status.output_ss           = 0.99;

	//�½�һ���������ڷ��ص�Java��
	jobject status = input_env->AllocObject(objectClass);

	//��ÿһ��ʵ���ı���
	input_env -> SetDoubleField(status, j_ss,recog_status.output_ss  );
	input_env -> SetDoubleField(status, j_blank_buffer,recog_status.output_blank_buffer);
	input_env -> SetIntField(status, j_recog_stat,recog_status.output_recog_state );

	return status;
}
#endif
//***************************************************************************
//�������ƣ�GetClassMethodID
//�������ܣ����ݺ������Ͳ����б��ҵ��ص�����ID
//��������� JNIEnv* env:						�̻߳���
//			const char* class_pathName,     ������
//			const char* function_Name,		��������
//			const char* arg_format			��������ǩ��
//
//������������ڱ���ķ�����ID������ֵ����������һ����ȫ�ֱ���
//��	         ע ��
//			����NDK��BUG���ú����ķ���ֵ��һֱ�����������������
//			 relese
//			ȥ��������ʾ��Ϣ
//**************************************************************************
static jmethodID get_class_method_id( JNIEnv* env,
								  	  const char* class_pathName,
									  const char* function_Name,
									  const char* arg_format
									)
{
	jclass clazz = env->FindClass(class_pathName); //classPathName�����İ���������

	jmethodID j_callback = env->GetMethodID(clazz, function_Name, arg_format); //��ȡjava�㷽��id

	return j_callback;
}

//***************************************************************************
//�������ƣ�native_getRecogStatus
//�������ܣ������ð󶨱��غ���
//���������
//���������
//��	         ע ��
//			1.ȥ��ϵͳ�Զ����ɵĺ�����ǰ׺
//			2.���Ա���ȫ�ֵĻ��������ͷ�����ID
//			3.����ȫ�ֻص�����
//			4.����ȫ�ֱ������ݽṹ��
//			5.����JAVA�ṹ���������,
//			6.����C�ṹ����Ϊ����ֵ
//
//**************************************************************************
JOBJECT native_get_recog_status(JNIEnv *input_env, jobject obj)
{

	//��������,ִ�г�ʼ������
	if(0 == g_java->first_start){

		//��ȡJVM��������ұ�����ȫ�ֱ�����
		input_env->GetJavaVM(&g_java->jvm);
		//@��ȡ����
		g_java->jobj = input_env->NewGlobalRef(obj);

		jclass javaClass = input_env->GetObjectClass(obj);
		//@����תΪȫ������
		g_java->jclass = (jclass)input_env->NewGlobalRef(javaClass);

		jmethodID *cb_method_ids = new jmethodID[sizeof(g_cb_methods) / sizeof(g_cb_methods[0])];

		//�ҵ�ϵͳ�����еĻص�����
		for( int i = 0 ; i < sizeof(g_cb_methods) / sizeof(g_cb_methods[0]);i ++){

			*(cb_method_ids++)  = get_class_method_id(  input_env,
														g_cb_methods[i].packagename,
													    g_cb_methods[i].name,
														g_cb_methods[i].signature);
		}
		//����ص���������IDΪȫ�ֱ���
		g_java->cb_recog_result = *(--cb_method_ids);

		delete cb_method_ids;

		g_java->first_start = 1;

	}
	//else
		{//�������� ���Ժ������÷���

			char str1[] = "test1,start....";
			char str2[] = "test2,start....";

			callback_java(str1,str2,100);
		}
	 INFOR("test C struct as return!");

	//��ȡJAVA�е�ʵ����
	 jclass objectClass = (input_env)->FindClass("net/telesing/tsdk/tlib/RecogStatus");

	//��ȡ����ÿһ�������Ķ���
	jfieldID j_ss			 = input_env->GetFieldID(objectClass, "ss", "D");
	jfieldID j_blank_buffer  = input_env->GetFieldID(objectClass, "blankBuffer", "D");
	jfieldID j_recog_stat    = input_env->GetFieldID(objectClass, "recogStat", "I");
#if 0
	//�޸�״ֵ̬׼��JAVA��ȡ
	//״̬��ֵ
	recog_status.output_recog_state  = 100;
	recog_status.output_blank_buffer = 0.2;
	recog_status.output_ss           = 0.99;

#endif
	//�½�һ���������ڷ��ص�Java��
	jobject status = input_env->AllocObject(objectClass);
#if 0
	//��ÿһ��ʵ���ı���
	input_env -> SetDoubleField(status, j_ss,recog_status.output_ss  );
	input_env -> SetDoubleField(status, j_blank_buffer,recog_status.output_blank_buffer);
	input_env -> SetIntField(status, j_recog_stat,recog_status.output_recog_state );
#endif
	return status;
}

static CER_INFOR g_cer_infor[] = {
		{"section"   ,"Ljava/lang/String;"},
		{"expiration","Ljava/lang/String;"},
		{"authority" ,"Ljava/lang/String;"},
//		{"appendData","Ljava/lang/String;"},���������ȡ��
		{"ID"        ,"Ljava/lang/String;"},
		{"sectSrc"   ,"Ljava/lang/String;"},
		{"orgID"     ,"Ljava/lang/String;"},
		{"genrTime"  ,"Ljava/lang/String;"},
		{"genrType"  ,"Ljava/lang/String;"},
		{"genrIp"    ,"Ljava/lang/String;"},
};
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ��û����д�㷨,������TM�������ɶ,�Ҷ�TM�ķ���һ���涨��CER
//
//**************************************************************************
JOBJECT native_decrypt_cer(JNIEnv *env, jobject obj, jstring str, jint length){

	INFOR("33333333native_decrypt_cer------>");
	jfieldID jfield_cer;
	jstring  jstringID;
	 jstring jstrTmp = env->NewStringUTF("ZZZ");
	//��ȡJAVA�е�ʵ����
	 jclass objectClass = env->FindClass("net/telesing/tsdk/tlib/CerInfor");

	 //���뵽���ڷ��ص�cer
	 jobject cer		= env->AllocObject(objectClass);

	 //�򵥸ı������뵽������׼�����ظ�JAVA��
	 for(int i = 0;i < sizeof(g_cer_infor)/sizeof(g_cer_infor[0]);i ++){

		//��ȡ��ǩ���洢
		 jfield_cer = env->GetFieldID(objectClass, g_cer_infor[i].name,g_cer_infor[i].signature);

		//��˼��,�޸��·���,��Ȼ��˵ûд
		 env->SetObjectField(cer, jfield_cer,jstrTmp);
	 }
	 return cer;
}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ��CerInfor �е�ÿһ���ֶ�һ��Ҫ��֤ȫ����д����,
//		   GetStringUTFChars���û����ȷ���ַ���������ڴ�Խ�����
//**************************************************************************
JSTRING native_encrypt_cer(JNIEnv *env, jobject obj, jobject cer, jint length){

	INFOR("[native_encrypt_cer]: ---->");

	jfieldID jfield_cer;
	jstring  jstringID;
	 char cer_data[255];
	//ָ�������Ż�����
	char *cer_char;
	memset(cer_data,0,255*sizeof(char));
	//��ȡJava�е�RecogCfgʵ����
	jclass objectClass  = env->FindClass("net/telesing/tsdk/tlib/CerInfor");

	//��ȡRecogCfg����ÿһ�������Ķ���
	for(int i = 0;i < sizeof(g_cer_infor)/sizeof(g_cer_infor[0]);i ++){

		//��ȡ��ǩ���洢
		 jfield_cer = env->GetFieldID(objectClass, g_cer_infor[i].name,g_cer_infor[i].signature);

		//ת����jstring
		 jstringID  = (jstring)env->GetObjectField(cer, jfield_cer);

		if( jstringID == NULL){
			return 	env->NewStringUTF("XXXXXXXXERROR CER IS NOT VAILD---->XXXXXXX");
		}

		//ת����char *����ڱ���ָ��
		  cer_char  = (char*)env->GetStringUTFChars(jstringID,NULL);

		//ģ�����ֱ�Ӱ�������������,ԭ�ⲻ������
		  strcat(cer_data,cer_char);
	}

	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","cer_data    = %s", cer_data);
	INFOR("CER OK");
#define 	 RELEASE
#ifndef RELEASE

	__android_log_print(ANDROID_LOG_INFO, "NATIVE:"," g_java->cer.expiration = %s :", g_java->cer.expiration);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:"," g_java->cer.authority  = %s :", g_java->cer.authority);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:"," g_java->cer.ID         = %s :", g_java->cer.id);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:"," g_java->cer.orgID      = %s :", g_java->cer.org_id);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:"," g_java->cer.sectSrc    = %s :", g_java->cer.sect_src);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:"," g_java->cer.genrTime   = %s :", g_java->cer.genr_time);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:"," g_java->cer.genrType   = %s :", g_java->cer.genr_type);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:"," g_java->cer.genrIp     = %s :", g_java->cer.genr_ip);

#endif

	return (env->NewStringUTF(cer_data));
 }
//***************************************************************************
//
//							�Զ�ע�ắ���б�
//
//***************************************************************************

static JNINativeMethod g_methods[] = {

	{ "encryptCer"       , "(Lnet/telesing/tsdk/tlib/CerInfor;I)Ljava/lang/String;",(void*)native_encrypt_cer},
	{ "decryptCer"       , "(Ljava/lang/String;I)Lnet/telesing/tsdk/tlib/CerInfor;",(void*)native_decrypt_cer},
  //{ "bindCer"          , " ",(void*)native_binder},
  //{ "unbindCer"        , " ",(void*)native_unbinder},
  //{ "isBindCer"        , " ",(void*)native_is_bind_cer},
  //{ "getSectionsStatus", " ",(void*)native_get_sections_status},
  //{ "genrWave"         , " ",(void*)native_genr_wave},
  //{ "initRecog"        , " ",(void*)native_init_recog},
  //{ "startRecog"       , " ",(void*)native_start_recog},
  //{ "pauseRecog"       , " ",(void*)native_pause_recog},
  //{ "restartRecog"     , " ",(void*)native_restart_recog},
  //{ "stopRecog"        , " ",(void*)native_stop_recog},
  //{ "writeRecog"       , " ",(void*)native_wrire_recog},
  //{ "setRecogConfig"   , " ",(void*)native_set_recog_config},
  //{ "getRecogConfig"   , " ",(void*)native_get_recog_config},
	{ "getRecogStatus", "()Lnet/telesing/tsdk/tlib/RecogStatus;", (void*)native_get_recog_status },


};
//***************************************************************************
//�������ƣ�registerNativeMethods
//�������ܣ�ע�ᵥ�����ط������������
//�����������������
//���������
//��	         ע ��
//			clazz = env->FindClass(env, className); C�м�ʹ�õķ���
//			clazz = env->FindClass(className);//C++ʹ�õķ���
//			if (env->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
//			if (env->RegisterNatives( clazz, gMethods, numMethods) < 0) {
//**************************************************************************

static int register_native_methods(JNIEnv* env, const char* className,
								   JNINativeMethod* g_methods,
								   int num_methods)
{
	jclass clazz;

	clazz = env->FindClass(className);//C++ʹ�õķ���
	if (clazz == NULL) {
		return JNI_FALSE;
	}
	if (env->RegisterNatives(clazz, g_methods, num_methods) < 0) {
		return JNI_FALSE;
	}

	return JNI_TRUE;
}
//***************************************************************************
//�������ƣ�
//�������ܣ����ݱ��غ����б�ָ��,ע�ắ����VM��
//���������JVM�����
//���������
//��	         ע ��
//
// 			if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK)
//			if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
//**************************************************************************
static int register_natives(JNIEnv* env)
{
    if (!register_native_methods(env, JNIREG_CLASS, g_methods,
        sizeof(g_methods) / sizeof(g_methods[0])))

        return JNI_FALSE;

    return JNI_TRUE;
}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	         ע ��
//			!�Զ�ע�᱾�غ���
//			!����java�����ʹ�õĿռ�,һ���ṹ��
// 			if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK)
//			if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
//**************************************************************************

JINT JNI_OnLoad(JavaVM *vm, void *reserved)
 {
	 //���ڱ���ȫ�ֵ�JAVA���ݽṹ��
	 g_java= new JAVA;
	 g_java->first_start = 0;

	JNIEnv* env = NULL;

	 // ��ȡJNI�汾
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK){

		return JNI_FALSE;

	}
	 //ע�᱾�غ���
	if (!register_natives(env)){

		return JNI_FALSE;
	}

	return JNI_VERSION_1_4;

 }
 //***************************************************************************
 //�������ƣ�JNI_OnUnload
 //�������ܣ������������
 //���������
 //���������
 //��	 ע ��
 //
 //			���������ȫ�����ÿռ�
 //**************************************************************************
JVOID JNI_OnUnload(JavaVM* vm, void* reserved){

	 delete g_java;
 }

#ifdef __cplusplus
}
#endif
