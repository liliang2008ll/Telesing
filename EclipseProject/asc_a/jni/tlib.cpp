#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <assert.h>
#include "net_telesing_tsdk_tlib_Acomms.h"
#include "tlib.h"
#include ".\core\dem.h"
#include ".\core\core.h"
#ifdef __cplusplus
extern "C" {
#endif

CORE core;
//***************************************************************************
//!������
#define JNIREG_CLASS "net/telesing/tsdk/tlib/Acomms"

//**************************************************************************

#define __START     __android_log_print(ANDROID_LOG_INFO, "NATIVE:","START:line %d, in function %s",__LINE__,__FUNCTION__);
#define __END 		__android_log_print(ANDROID_LOG_INFO, "NATIVE:","END  :line %d, in function %s",__LINE__,__FUNCTION__);
//**************************************************************************
//��ຯ������
//����ֵ			 JNIָ��ֵ
#define JINT		JNIEXPORT jint JNICALL
#define	JVOID		JNIEXPORT void JNICALL
#define	JOBJECT	 	JNIEXPORT jobject JNICALL
#define	JSTRING  	JNIEXPORT jstring JNICALL
#define JSHORTARRAY JNIEXPORT jshortArray JNICALL


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

	g_java->jvm->DetachCurrentThread();

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
//��	        ע ������ȫ�ֱ���,�����ص�����
//
//**************************************************************************
static void save_java_option(JNIEnv *input_env, jobject obj){

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
													g_cb_methods[i].signature);	}
	//����ص���������IDΪȫ�ֱ���
	g_java->cb_recog_result = *(--cb_method_ids);

	delete []cb_method_ids;

}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע �������û����ò���
//**************************************************************************
static int load_sdk_cfg(JNIEnv *input_env, jobject obj,jobject cfg){

	//��ȡJava�е�ʵ����
	jclass objectClass = input_env->FindClass("net/telesing/tsdk/tlib/RecogCfg");

	//��ȡ����ÿһ�������Ķ���
	jfieldID  jstart_feq   = input_env->GetFieldID(objectClass,"startFeq", "[I");
	jfieldID  jend_feq     = input_env->GetFieldID(objectClass,"endFeq"  , "[I");
	jfieldID  jbuffer_len = input_env->GetFieldID(objectClass, "bufferLen", "I");
	jfieldID  jgap        = input_env->GetFieldID(objectClass, "gap"      , "I");
	jfieldID  jfft_points = input_env->GetFieldID(objectClass, "fftPoints", "I");
	jfieldID  jfft_chains = input_env->GetFieldID(objectClass, "fftChains", "I");

	jfieldID  jst         = input_env->GetFieldID(objectClass, "st"        ,"D");

	//��ȡJava����������arrays�Ķ���
	 jintArray  istart_feq = (jintArray)( input_env )->GetObjectField( cfg, jstart_feq );
	 jintArray  iend_feq   = (jintArray)( input_env )->GetObjectField( cfg, jend_feq   );

	 //��������һ��Ҫ������
	 if( istart_feq == NULL|| iend_feq == NULL){
		 return 1;
	 }
	 //��ȡarrays�����ָ��
	 jint* int_start_feq = input_env->GetIntArrayElements(istart_feq,0);
	 jint* int_end_feq   = input_env->GetIntArrayElements(iend_feq  ,0);

	 //��ֵ����
	 for(int i = 0 ; i < 4; i ++){

		 g_java->sys_cfg.start_feq[i] = *(int_start_feq + i);
		 g_java->sys_cfg.end_feq[i]   = *(int_end_feq   + i);
	 }


	//int���ͱ���
	g_java->sys_cfg.gap			= input_env->GetIntField(cfg,jgap);
	g_java->sys_cfg.fft_points  = input_env->GetIntField(cfg,jfft_points);
	g_java->sys_cfg.fft_chains  = input_env->GetIntField(cfg,jfft_chains);
	g_java->sys_cfg.buffer_len  = input_env->GetIntField(cfg,jbuffer_len);

	//�������͵ı���
	g_java->sys_cfg.st  		= input_env->GetDoubleField(cfg,jst);

	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[0]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[0]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[1]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[1]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[2]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[2]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[3]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[3]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.gap        = %d", g_java->sys_cfg.gap);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.fft_points = %d", g_java->sys_cfg.fft_points);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.fft_chains = %d", g_java->sys_cfg.fft_chains);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.buffer_len = %d", g_java->sys_cfg.buffer_len);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.st         = %f", g_java->sys_cfg.st);

}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ������ϵͳĬ�����ò���
//**************************************************************************
static int load_default_cfg(void){

	int start_feq[4]={16, 12, 8, 4};//{16};
	int ent_Feq[4]  ={20, 16, 12, 8};//{20};

	for(int i = 0;i < 4 ;i++){
		g_java->sys_cfg.start_feq[i] =  start_feq[i];
		g_java->sys_cfg.end_feq[i]   =  ent_Feq[i];
	}
	g_java->sys_cfg.gap 		= 512;
	g_java->sys_cfg.st  		= 0.15;
	g_java->sys_cfg.fft_points  = 2048;
	g_java->sys_cfg.fft_chains  = 3;
	g_java->sys_cfg.buffer_len  = 44100*3;

	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[0]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[0]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[1]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[1]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[2]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[2]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[3]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[3]);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.gap        = %d", g_java->sys_cfg.gap       );
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.fft_points = %d", g_java->sys_cfg.fft_points);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.fft_chains = %d", g_java->sys_cfg.fft_chains);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.buffer_len = %d", g_java->sys_cfg.buffer_len);
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.st         = %f", g_java->sys_cfg.st        );

}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ������ϵͳ���ò���
//              	public int[] start_feq;	//��ʼƵ��
//					public int[] end_feq;	//��ֹƵ��
//					public int 		gap   ;		//��ת��϶
//					public double 	st ;		//���ƶ�
//					public int 		fft_points; //FFT����
//					public int 		fft_chains;	//FFT�ŵ���
//					public int 		fft_gap;	//ͬ��ʧ�ܺ���ת��϶
//**************************************************************************
static int save_sys_cfg(JNIEnv *input_env, jobject obj,jobject cfg){

	//�ж�����Ĭ�ϲ�������ϵͳ���ò���,���ݳ�ʼ��ʱ�����ڻ�������С
	if(0X01 ==g_java->sys_cfg.buffer_len){

		INFOR("LOAD DEFAULT ARGS------>");

		return load_default_cfg();
	}else{

		return load_sdk_cfg(input_env, obj,cfg);
	}
}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ��û����д�㷨,������TM�������ɶ,�Ҷ�TM�ķ���һ���涨��CER
//
//**************************************************************************
JOBJECT native_decrypt_cer(JNIEnv *env, jobject obj, jstring str, jint length){
__START
	jfieldID jfield_cer;
	jstring  jstringID;
	 char cer_data[255];
	 const char *c_str;
	 char *p_cer;
	//ָ�������Ż�����
	char *cer_char;
	memset(cer_data,0,255*sizeof(char));

	//��ȡ�ַ���,�ȴ�����
	c_str = env->GetStringUTFChars(str,NULL);
	//sprintf(cer_data,%s, c_str);

	//���ͽ������ַ����͵������㷨�н�ȥ����
	p_cer = core.cer_decrypt(p_cer);

	 jstring jstrTmp = env->NewStringUTF("ZZZ");
	//��ȡJAVA�е�ʵ����
	 jclass objectClass = env->FindClass("net/telesing/tsdk/tlib/CerInfor");

	 //���뵽���ڷ��ص�cer
	 jobject cer		= env->AllocObject(objectClass);

	 //�򵥸ı������뵽������׼�����ظ�JAVA��
	 for(int i = 0;i < sizeof(g_cer_infor)/sizeof(g_cer_infor[0]);i ++){

		//��ȡ��ǩ���洢
		 jfield_cer = env->GetFieldID(objectClass, g_cer_infor[i].name,g_cer_infor[i].signature);

		//ת����char *����ڱ���ָ��
		  cer_char  = (char*)env->GetStringUTFChars(jstringID,NULL);

		//���ӳ��ַ���,���������㷨�ں��еļӽ���
		  strcat(cer_data,cer_char);

		//��˼��,�޸��·���,��Ȼ��˵ûд
		 env->SetObjectField(cer, jfield_cer,jstrTmp);
	 }

	 env->ReleaseStringUTFChars(str, c_str);
 __END
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

	__START

	jfieldID jfield_cer;
	jstring  jstringID;
	 char cer_data[255];
	 char *p_cer;
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

		//���ӳ��ַ���,���������㷨�ں��еļӽ���
		  strcat(cer_data,cer_char);

	}
		//ģ�����ֱ�Ӱ�������������,ԭ�ⲻ������
		p_cer =  core.cer_encrypt(cer_char);

	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","cer_data    = %s", cer_data);
__END
	return (env->NewStringUTF(cer_data));
 }
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ����һ��֤��,ֱ�ӷ��سɹ���
//
//**************************************************************************
JINT native_binder (JNIEnv * env, jobject obj, jobject cer, jint cer_length){
	__START

	//
//	core.cer_bind();
	return 0;
	__END
}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ���������Ƿ��,ֱ�ӷ��� 0 == OK
//
//**************************************************************************
JINT native_unbinder (JNIEnv * env, jobject obj, jobject cer, jint cer_length){
	__START
	return 0;
	__END
}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע �����֤���Ƿ��,ֱ�ӷ���
//
//**************************************************************************
JINT native_is_bind_cer (JNIEnv * env, jobject obj, jobject cer, jint cer_length){
	__START


	__END
	return 0;
}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע �����֤��Ȩ��,ֱ�ӷ���0��
//
//**************************************************************************
JINT native_get_sections_status (JNIEnv *env, jobject obj, jint cer_length){
	__START


	__END
	return 0;
}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ��Ŀǰ����Ҫʵ��
//
//**************************************************************************
JSHORTARRAY native_genr_wave (JNIEnv *env, jobject obj, jstring section, jstring data){
	__START

	__END
}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ��Ŀǰ����Ҫʵ��
//
//**************************************************************************
JINT native_init_recog  (JNIEnv *env, jobject obj){
	__START

	__END
}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ��
//
//**************************************************************************
JINT native_start_recog(JNIEnv *env, jobject obj, jobject cfg){
	__START
	int error = 0;
	int n_v=2048;
	double st_v=0.15;						//! ���ƶ�
	int gap_v=512;
	int feq_size=4;				//!  �ŵ���
	//! �ŵ�
	double sFeq[]={16, 12, 8, 4};//{5.5125};
	double eFeq[]={20, 16, 12, 8};//{11.025};
	//�ж�����״̬
	switch(g_java->first_start)
	{
		//��������,ִ�г�ʼ������
		case 0:
				save_java_option(env,obj);//����java���ݵ�ȫ�ֽṹ����
//				error =save_sys_cfg(env,obj,cfg);//�������װ������,׼������
				g_java->first_start = 1;

//				data_init(	g_java->sys_cfg.fft_points,\
//						    g_java->sys_cfg.st,\
//						    g_java->sys_cfg.gap,\
//						   (double*)g_java->sys_cfg.start_feq,\
//						   (double*)g_java->sys_cfg.end_feq,
//						    g_java->sys_cfg.fft_chains
//				);
				 data_init(n_v,st_v,gap_v,sFeq,eFeq,feq_size);

		case 1:
			//��ʼ��������
			start();
			break;

		default:
			error = 1;
		break;
	}

	__END
	return error;
}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ��
//
//**************************************************************************
JINT native_pause_recog(JNIEnv *env, jobject obj){
	__START

	 __END
}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ��
//
//**************************************************************************
JINT native_restart_recog(JNIEnv *env, jobject obj){
	__START

	 __END
}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ��
//
//**************************************************************************
JINT native_stop_recog(JNIEnv *env, jobject obj){
	__START
	stop();
	 __END
}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ��
//
//**************************************************************************
JINT native_wrire_recog (JNIEnv *env, jobject obj, jshortArray waves, jint waves_length){

		jshort *carr;
		carr = env->GetShortArrayElements( waves, NULL);

		int re=wirte(carr,waves_length);
		env->ReleaseShortArrayElements(waves,carr,0);
		return re ;

}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ��
//
//**************************************************************************
JINT native_set_recog_config(JNIEnv *env, jobject obj, jobject cfg){
	__START

	return save_sys_cfg(env,obj,cfg);//�������װ������,׼������
	__END
}
//***************************************************************************
//�������ƣ�
//�������ܣ�
//���������
//���������
//��	        ע ��
//
//**************************************************************************
JOBJECT native_get_recog_config(JNIEnv *input_env, jobject obj){
	__START

	//��ȡJava�е�ʵ����
	jclass objectClass = input_env->FindClass("net/telesing/tsdk/tlib/RecogCfg");

	//�½�һ������
	jobject cfg = input_env->AllocObject(objectClass);
	//��ȡ����ÿһ�������Ķ���
	jfieldID  jstart_feq   = input_env->GetFieldID(objectClass,"startFeq", "[I");
	jfieldID  jend_feq     = input_env->GetFieldID(objectClass,"endFeq"  , "[I");
	jfieldID  jbuffer_len  = input_env->GetFieldID(objectClass, "bufferLen", "I");
	jfieldID  jgap         = input_env->GetFieldID(objectClass, "gap"      , "I");
	jfieldID  jfft_points  = input_env->GetFieldID(objectClass, "fftPoints", "I");
	jfieldID  jfft_chains  = input_env->GetFieldID(objectClass, "fftChains", "I");

	jfieldID  jst          = input_env->GetFieldID(objectClass, "st"        ,"D");



	//��ȡJava����������arrays�Ķ���
	 jintArray  istart_feq = (jintArray)( input_env )->GetObjectField( cfg, jstart_feq );
	 jintArray  iend_feq   = (jintArray)( input_env )->GetObjectField( cfg, jend_feq   );


	 //��ȡarrays�����ָ��
	 jint* int_start_feq = input_env->GetIntArrayElements(istart_feq,0);
	 jint* int_end_feq   = input_env->GetIntArrayElements(iend_feq  ,0);

	 //��ֵ����
	 for(int i = 0 ; i < 4; i ++){

	//	 input_env->SetIntArrayElements()
	//	 g_java->sys_cfg.start_feq[i] = *(int_start_feq + i);
	//	 g_java->sys_cfg.end_feq[i]   = *(int_end_feq   + i);
	 }
	//int���ͱ���
	input_env->SetIntField(cfg,jgap,g_java->sys_cfg.gap	);
	input_env->SetIntField(cfg,jfft_points,g_java->sys_cfg.fft_points);
	input_env->SetIntField(cfg,jfft_chains,g_java->sys_cfg.fft_chains );
	input_env->SetIntField(cfg,jbuffer_len,g_java->sys_cfg.buffer_len);

	//�������͵ı���
	input_env->SetDoubleField(cfg,jst,g_java->sys_cfg.st );

	return cfg;
	__END
}
//***************************************************************************
//�������ƣ�native_getRecogStatus
//�������ܣ������ð󶨱��غ���
//���������
//���������
//��	         ע �� 0.���Ե�ʱ����Ϊϵͳ�Ĳ��Ժ���
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
	__START

	//��ȡJAVA�е�ʵ����
	 jclass objectClass = (input_env)->FindClass("net/telesing/tsdk/tlib/RecogStatus");

	//��ȡ����ÿһ�������Ķ���
	jfieldID j_ss			 = input_env->GetFieldID(objectClass, "ss", "D");
	jfieldID j_blank_buffer  = input_env->GetFieldID(objectClass, "blankBuffer", "D");
	jfieldID j_recog_stat    = input_env->GetFieldID(objectClass, "recogStat", "I");

	//�½�һ���������ڷ��ص�Java��
	jobject status = input_env->AllocObject(objectClass);

	//��ÿһ��ʵ���ı���
	g_java->sys_status.ss           = core_ss;
	g_java->sys_status.blank_buffer = fillRate();
	g_java->sys_status.recog_state  = thread_1_stause;

	input_env -> SetDoubleField(status, j_ss          ,g_java->sys_status.ss );
	input_env -> SetDoubleField(status, j_blank_buffer,g_java->sys_status.blank_buffer);
	input_env -> SetIntField( status, j_recog_stat    ,g_java->sys_status.recog_state );

	__END
	return status;
}
//***************************************************************************
//
//							�Զ�ע�ắ���б�
//
//***************************************************************************

static JNINativeMethod g_methods[] = {
	{ "encryptCer"       , "(Lnet/telesing/tsdk/tlib/CerInfor;I)Ljava/lang/String;",(void*)native_encrypt_cer},
	{ "decryptCer"       , "(Ljava/lang/String;I)Lnet/telesing/tsdk/tlib/CerInfor;",(void*)native_decrypt_cer},
	{ "bindCer"          , "(Lnet/telesing/tsdk/tlib/CerInfor;I)I"  			   ,(void*)native_binder},
	{ "unbindCer"        , "(Lnet/telesing/tsdk/tlib/CerInfor;I)I"  			   ,(void*)native_unbinder},
	{ "isBindCer"        , "(Lnet/telesing/tsdk/tlib/CerInfor;I)I"                 ,(void*)native_is_bind_cer},
	{ "getSectionsStatus", "(Lnet/telesing/tsdk/tlib/CerInfor;)I"                  ,(void*)native_get_sections_status},
	{ "genrWave"         , "(Ljava/lang/String;Ljava/lang/String;)[S"              ,(void*)native_genr_wave},
	{ "initRecog"        , "()I"                                                   ,(void*)native_init_recog},
	{ "startRecog"       , "(Lnet/telesing/tsdk/tlib/RecogCfg;)I"                  ,(void*)native_start_recog},
	{ "pauseRecog"       , "()I"                                                   ,(void*)native_pause_recog},
	{ "restartRecog"     , "()I"								                   ,(void*)native_restart_recog},
	{ "stopRecog"        , "()I"                                                   ,(void*)native_stop_recog},
	{ "writeRecog"       , "([SI)I"                                                ,(void*)native_wrire_recog},
	{ "setRecogConfig"   , "(Lnet/telesing/tsdk/tlib/RecogCfg;)I"                  ,(void*)native_set_recog_config},
	{ "getRecogConfig"   , "()Lnet/telesing/tsdk/tlib/RecogCfg;"                   ,(void*)native_get_recog_config},
	{ "getRecogStatus"   , "()Lnet/telesing/tsdk/tlib/RecogStatus;"                ,(void*)native_get_recog_status },
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
	//�ж�ϵͳ����start������ʱ
	 g_java->first_start = 0;

	 //С����,�����ж��û��Ƿ��������ò���,
	 //�û����ٵĻ�������������0X01��С��
	 //ϵͳĬ�ϵĳ�ʼ��Ҳ������OX01��ҪôOXOO.Ҫô0XFF

	 g_java->sys_cfg.buffer_len = 0;
	 g_java->sys_status.ss = 1.0;
	 g_java->sys_status.blank_buffer = 1.0;
	 g_java->sys_status.recog_state  = 0;
	JNIEnv* env = NULL;

	 // ��ȡJNI�汾
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK){
		INFOR("ERROR IN JNI_VERSION_1_4");
		return JNI_FALSE;

	}
	 //ע�᱾�غ���
	if (!register_natives(env)){
		INFOR("ERROR IN regedit");
		return JNI_FALSE;
	}

	return JNI_VERSION_1_4;

 }
 //***************************************************************************
 //�������ƣ�JNI_OnUnload
 //�������ܣ��������������
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