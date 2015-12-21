#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <assert.h>
#include "net_telesing_tsdk_tlib_Acomms.h"
#include "tlib.h"
#include ".\core\dem.h"
#ifdef __cplusplus
extern "C" {
#endif

//***************************************************************************
//!鍖呭悕绉�
#define JNIREG_CLASS "net/telesing/tsdk/tlib/Acomms"

//**************************************************************************
#define INFOR(a)    __android_log_print(ANDROID_LOG_INFO, "NATIVE:",a);
#define __START     __android_log_print(ANDROID_LOG_INFO, "NATIVE:","START:line %d, in function %s",__LINE__,__FUNCTION__);
#define __END 		__android_log_print(ANDROID_LOG_INFO, "NATIVE:","END  :line %d, in function %s",__LINE__,__FUNCTION__);
//**************************************************************************
//绠�娲佸嚱鏁板悕绉�
//杩斿洖鍊�			 JNI鎸囧畾鍊�
#define JINT		JNIEXPORT jint JNICALL
#define	JVOID		JNIEXPORT void JNICALL
#define	JOBJECT	 	JNIEXPORT jobject JNICALL
#define	JSTRING  	JNIEXPORT jstring JNICALL
#define JSHORTARRAY JNIEXPORT jshortArray JNICALL
//**************************************************************************
//瀹氫箟鏁版嵁鐢ㄤ簬瀛樻斁JAVA鐩稿叧鍙傛暟
typedef struct{

    JavaVM* 	  jvm;			//Java绾跨▼
    jobject 	  jobj;
    jclass  	  jclass;
    jmethodID     cb_recog_result; //鍥炶皟鍑芥暟鍒楄〃

     jchar  	  first_start;  	//鍒濆鍖栦簰鏂�
     CER		  cer; 			//璇佷功
     Recog_status sys_status;	//鐘舵��
     Recog_cfg 	  sys_cfg;	 	//閰嶇疆
}JAVA;

//鍥炶皟鍑芥暟
typedef struct {
    const char* packagename;
    const char* name;
    const char* signature;
} JNICallBackMethod;

//璇佷功绛惧悕鐢ㄧ粨鏋勪綋
typedef struct {
    const char* name;
    const char* signature;
} CER_INFOR;
//鍏ㄥ眬鐨刯ava閫夐」
JAVA *g_java ;


static JNICallBackMethod g_cb_methods[] = {
        {"net/telesing/tsdk/tlib/Acomms","recogResult", "(Ljava/lang/String;Ljava/lang/String;I)V"},
};

//***************************************************************************
//鍑芥暟鍚嶇О锛歝allback_java
//鍑芥暟鍔熻兘锛氬悓姝ュ洖鎺夊嚱鏁�
//杈撳叆鍙傛暟锛� char *section, 	璇佷功娈�
//			char *data		瑙ｆ瀽鍒扮殑鏁版嵁
//			int times		瑙ｆ瀽鑺辫垂鏃堕棿鍗曚綅ms
//杈撳嚭鍙傛暟锛� 鏃�
//澶�	        娉� 锛�
//
//
//**************************************************************************
void callback_java( char *section, char *data,int times){

    JNIEnv* input_env;

    g_java->jvm->AttachCurrentThread(&input_env, NULL);//鑾峰彇绾跨▼鐜

    //璋冪敤杩斿洖鍊间负void鐨勬柟娉�
    input_env->CallVoidMethod(  g_java->jobj, g_java->cb_recog_result,
                                input_env->NewStringUTF(section),	//浼犲叆璇佷功淇℃伅
                                input_env->NewStringUTF(data),	  	//浼犲叆鏁版嵁淇℃伅
                                times);							  	//浼犲叆鏃堕棿淇℃伅
#ifdef _USED_IN_PTHREAD
    g_java->jvm->DetachCurrentThread();
#endif
}
#if 0
void ndk_test_call_fnx(char *debug_message){
    //@鏂板缓瀛楃涓茬敤浜庢祴璇�
    JNIEnv* env_;
    char str1[] = "test1,start....";
    char str2[] = "test2,start....";

    globe_JavaVM->AttachCurrentThread(&env_, NULL);

    //鎵惧埌鎸囧畾鐨勫嚱鏁板悕
    jmethodID mid = env_->GetMethodID(globe_JavaClass, "recogResult", "(Ljava/lang/String;Ljava/lang/String;I)V");
    //璋冪敤杩斿洖鍊间负void鐨勬柟娉�
    env_->CallVoidMethod(globe_JavaObj, mid,env_->NewStringUTF(str1),env_->NewStringUTF(str2),100);

    globe_JavaVM->DetachCurrentThread();
    //鐘舵�佽祴鍊�
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
//璋冭瘯
JNIEXPORT jint JNICALL Java_net_telesing_tsdk_tlib_Acomms_startRecog
  (JNIEnv *input_env, jobject input_obj, jobject recog_cfg){

    __android_log_print(ANDROID_LOG_INFO, "NDK:","START RECONG");

    //鑾峰彇Java涓殑瀹炰緥绫�
    jclass objectClass = input_env->FindClass("net/telesing/tsdk/tlib/Recog_cfg");


    //鑾峰彇绫讳腑姣忎竴涓彉閲忕殑瀹氫箟
    jfieldID  jgap      = input_env->GetFieldID(objectClass, "gap", "I");
    jfieldID  jpoints   = input_env->GetFieldID(objectClass, "fft_points", "I");

    //鏀瑰彉缁撴瀯浣撳彉閲忎腑鐨勬暟鍊�
    //input_env->SetIntField(recog_cfg,fft_gap,100);
    //鑾峰彇缁撴瀯浣撳彉閲忎腑int绫诲瀷鐨勫彉閲忓��
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
    //鑾峰彇JAVA涓殑瀹炰緥绫�
     jclass objectClass = (input_env)->FindClass("net/telesing/tsdk/tlib/Recog_status");

     //鑾峰彇绫讳腑姣忎竴涓彉閲忕殑瀹氫箟
    jfieldID j_ss			 = input_env->GetFieldID(objectClass, "ss", "D");
    jfieldID j_blank_buffer  = input_env->GetFieldID(objectClass, "blankBuffer", "D");
    jfieldID j_recog_stat    = input_env->GetFieldID(objectClass, "recogStat", "I");

    //淇敼鐘舵�佸�煎噯澶嘕AVA鑾峰彇
    //鐘舵�佽祴鍊�
    recog_status.output_recog_state  = 100;
    recog_status.output_blank_buffer = 0.2;
    recog_status.output_ss           = 0.99;

    //鏂板缓涓�涓璞＄敤浜庤繑鍥炲埌Java涓�
    jobject status = input_env->AllocObject(objectClass);

    //缁欐瘡涓�涓疄渚嬬殑鍙橀噺
    input_env -> SetDoubleField(status, j_ss,recog_status.output_ss  );
    input_env -> SetDoubleField(status, j_blank_buffer,recog_status.output_blank_buffer);
    input_env -> SetIntField(status, j_recog_stat,recog_status.output_recog_state );

    return status;
}
#endif

//***************************************************************************
//鍑芥暟鍚嶇О锛欸etClassMethodID
//鍑芥暟鍔熻兘锛氭牴鎹嚱鏁板悕鍜屽弬鏁板垪琛ㄦ壘鍒板洖璋冨嚱鏁癐D
//杈撳叆鍙傛暟锛� JNIEnv* env:						绾跨▼鐜
//			const char* class_pathName,     绫诲悕绉�
//			const char* function_Name,		鍑芥暟鍚嶇О
//			const char* arg_format			鍑芥暟鍙傛暟绛惧悕
//
//杈撳嚭鍙傛暟锛氱敤浜庝繚瀛樼殑鏂规硶鐨処D锛岃繑鍥炲�间繚瀛樹笅鏉ョ殑涓�瀹氭槸鍏ㄥ眬鍙橀噺
//澶�	         娉� 锛�
//			鐢变簬NDK鐨凚UG锛岃鍑芥暟鐨勮繑鍥炲�煎洖涓�鐩存暣涓▼搴忕殑鐢熷懡鍛ㄦ湡
//			 relese
//			鍘绘帀閿欒鎻愮ず淇℃伅
//**************************************************************************
static jmethodID get_class_method_id( JNIEnv* env,
                                      const char* class_pathName,
                                      const char* function_Name,
                                      const char* arg_format
                                    )
{
    jclass clazz = env->FindClass(class_pathName); //classPathName瀹屾暣鐨勫寘鍚嶅姞绫诲悕

    jmethodID j_callback = env->GetMethodID(clazz, function_Name, arg_format); //鑾峰彇java灞傛柟娉昳d

    return j_callback;
}

static CER_INFOR g_cer_infor[] = {
        {"section"   ,"Ljava/lang/String;"},
        {"expiration","Ljava/lang/String;"},
        {"authority" ,"Ljava/lang/String;"},
//		{"appendData","Ljava/lang/String;"},涓嶈兘澶栭潰鑾峰彇鐨�
        {"ID"        ,"Ljava/lang/String;"},
        {"sectSrc"   ,"Ljava/lang/String;"},
        {"orgID"     ,"Ljava/lang/String;"},
        {"genrTime"  ,"Ljava/lang/String;"},
        {"genrType"  ,"Ljava/lang/String;"},
        {"genrIp"    ,"Ljava/lang/String;"},
};
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛氫繚瀛樺叏灞�鍙橀噺,澶勭悊鍥炴帀鍑芥暟
//
//**************************************************************************
static void save_java_option(JNIEnv *input_env, jobject obj){

    //鑾峰彇JVM铏氭嫙鏈哄苟涓斾繚瀛樺湪鍏ㄥ眬鍙橀噺涓�
    input_env->GetJavaVM(&g_java->jvm);
    //@鑾峰彇瀵硅薄
    g_java->jobj = input_env->NewGlobalRef(obj);

    jclass javaClass = input_env->GetObjectClass(obj);
    //@瀵硅薄杞负鍏ㄥ眬寮曠敤
    g_java->jclass = (jclass)input_env->NewGlobalRef(javaClass);

    jmethodID *cb_method_ids = new jmethodID[sizeof(g_cb_methods) / sizeof(g_cb_methods[0])];

    //鎵惧埌绯荤粺涓墍鏈夌殑鍥炴帀鏂规硶
    for( int i = 0 ; i < sizeof(g_cb_methods) / sizeof(g_cb_methods[0]);i ++){

        *(cb_method_ids++)  = get_class_method_id(  input_env,
                                                    g_cb_methods[i].packagename,
                                                    g_cb_methods[i].name,
                                                    g_cb_methods[i].signature);	}
    //淇濆瓨鍥炴帀鍑芥暟鏂规硶ID涓哄叏灞�鍙橀噺
    g_java->cb_recog_result = *(--cb_method_ids);

    delete []cb_method_ids;

}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛氳浇鍏ョ敤鎴烽厤缃弬鏁�
//**************************************************************************
static int load_sdk_cfg(JNIEnv *input_env, jobject obj,jobject cfg){

    //鑾峰彇Java涓殑瀹炰緥绫�
    jclass objectClass = input_env->FindClass("net/telesing/tsdk/tlib/RecogCfg");

    //鑾峰彇绫讳腑姣忎竴涓彉閲忕殑瀹氫箟
#if 1
    jfieldID  jstart_feq   = input_env->GetFieldID(objectClass, "startFeq", "[I");
    jfieldID  jend_feq     = input_env->GetFieldID(objectClass, "endFeq"  , "[I");
#endif
#if 1
    jfieldID  jbuffer_len = input_env->GetFieldID(objectClass, "bufferLen", "I");
    jfieldID  jgap        = input_env->GetFieldID(objectClass, "gap"      , "I");
    jfieldID  jfft_points = input_env->GetFieldID(objectClass, "fftPoints", "I");
    jfieldID  jfft_chains = input_env->GetFieldID(objectClass, "fftChains", "I");
#endif
    jfieldID  jst         = input_env->GetFieldID(objectClass, "st"        ,"D");

    //鑾峰彇Java涓暟缁勫睘鎬rrays鐨勫璞�
     jintArray  istart_feq = (jintArray)( input_env )->GetObjectField( cfg, jstart_feq );
     jintArray  iend_feq   = (jintArray)( input_env )->GetObjectField( cfg, jend_feq   );

     //鏁扮粍閲岄潰涓�瀹氳鏈夋暟鎹�
     if( istart_feq == NULL|| iend_feq == NULL){
         return 1;
     }
     //鑾峰彇arrays瀵硅薄鐨勬寚閽�
     jint* int_start_feq = input_env->GetIntArrayElements(istart_feq,0);
     jint* int_end_feq   = input_env->GetIntArrayElements(iend_feq  ,0);

     //璧嬪�艰繃鏉�
     for(int i = 0 ; i < 4; i ++){

         g_java->sys_cfg.start_feq[i] = *(int_start_feq + i);
         g_java->sys_cfg.end_feq[i]   = *(int_end_feq   + i);
     }

#if 1
    //int绫诲瀷鍙橀噺
    g_java->sys_cfg.gap			= input_env->GetIntField(cfg,jgap);
    g_java->sys_cfg.fft_points  = input_env->GetIntField(cfg,jfft_points);
    g_java->sys_cfg.fft_chains  = input_env->GetIntField(cfg,jfft_chains);
    g_java->sys_cfg.buffer_len  = input_env->GetIntField(cfg,jbuffer_len);
#endif
    //娴偣绫诲瀷鐨勫彉閲�
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
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛氳浇鍏ョ郴缁熼粯璁ら厤缃弬鏁�
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
    __android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.gap        = %d", g_java->sys_cfg.gap);
    __android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.fft_points = %d", g_java->sys_cfg.fft_points);
    __android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.fft_chains = %d", g_java->sys_cfg.fft_chains);
    __android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.buffer_len = %d", g_java->sys_cfg.buffer_len);
    __android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.st         = %f", g_java->sys_cfg.st);

}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛氫繚瀛樼郴缁熼厤缃弬鏁�
//              	public int[] start_feq;	//鍚棰戠巼
//					public int[] end_feq;	//鎴棰戠巼
//					public int 		gap   ;		//璺宠浆闂撮殭
//					public double 	st ;		//鐩镐技搴�
//					public int 		fft_points; //FFT鐐规暟
//					public int 		fft_chains;	//FFT淇￠亾鏁�
//					public int 		fft_gap;	//鍚屾澶辫触鍚庤烦杞棿闅�
//**************************************************************************
static int save_sys_cfg(JNIEnv *input_env, jobject obj,jobject cfg){

    //鍒ゅ畾杞藉叆榛樿鍙傛暟杩樻槸绯荤粺閰嶇疆鍙傛暟,鏍规嵁鍒濆鍖栨椂鍊欑敤浜庣紦鍐插尯澶у皬
    if(0X01 ==g_java->sys_cfg.buffer_len){

        INFOR("LOAD DEFAULT ARGS------>");

        return load_default_cfg();
    }else{

        return load_sdk_cfg(input_env, obj,cfg);
    }
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛氭病鏈夊～鍐欑畻娉�,涓嶇浣燭M杈撳叆鐨勬槸鍟�,鎴戦兘TM鐨勮繑鍥炰竴涓瀹氱殑CER
//
//**************************************************************************
JOBJECT native_decrypt_cer(JNIEnv *env, jobject obj, jstring str, jint length){
__START
    jfieldID jfield_cer;
    jstring  jstringID;
     jstring jstrTmp = env->NewStringUTF("ZZZ");
    //鑾峰彇JAVA涓殑瀹炰緥绫�
     jclass objectClass = env->FindClass("net/telesing/tsdk/tlib/CerInfor");

     //鐢宠鍒扮敤浜庤繑鍥炵殑cer
     jobject cer		= env->AllocObject(objectClass);

     //绠�鍗曟敼鍙樹笅鐢宠鍒扮殑鏁版嵁鍑嗗杩斿洖缁橨AVA鐢�
     for(int i = 0;i < sizeof(g_cer_infor)/sizeof(g_cer_infor[0]);i ++){

        //鑾峰彇鏍囩骞跺瓨鍌�
         jfield_cer = env->GetFieldID(objectClass, g_cer_infor[i].name,g_cer_infor[i].signature);

        //鎰忔�濅笅,淇敼涓嬭繑鍥�,涓嶇劧浼氳娌″啓
         env->SetObjectField(cer, jfield_cer,jstrTmp);
     }
 __END
     return cer;
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛欳erInfor 涓殑姣忎竴涓瓧娈典竴瀹氳淇濊瘉鍏ㄩ儴濉啓瀹屾暣,
//		   GetStringUTFChars濡傛灉娌℃湁姝ｇ‘鐨勫瓧绗︿覆浼犲叆鍥炲唴瀛樿秺鐣岄敊璇�
//**************************************************************************
JSTRING native_encrypt_cer(JNIEnv *env, jobject obj, jobject cer, jint length){

    __START

    jfieldID jfield_cer;
    jstring  jstringID;
     char cer_data[255];
    //鎸囬拡鏁扮粍瀛樻斁鍥炴潵鐨�
    char *cer_char;
    memset(cer_data,0,255*sizeof(char));
    //鑾峰彇Java涓殑RecogCfg瀹炰緥绫�
    jclass objectClass  = env->FindClass("net/telesing/tsdk/tlib/CerInfor");

    //鑾峰彇RecogCfg绫讳腑姣忎竴涓彉閲忕殑瀹氫箟
    for(int i = 0;i < sizeof(g_cer_infor)/sizeof(g_cer_infor[0]);i ++){

        //鑾峰彇鏍囩骞跺瓨鍌�
         jfield_cer = env->GetFieldID(objectClass, g_cer_infor[i].name,g_cer_infor[i].signature);

        //杞崲鎴恓string
         jstringID  = (jstring)env->GetObjectField(cer, jfield_cer);

        if( jstringID == NULL){
            return 	env->NewStringUTF("XXXXXXXXERROR CER IS NOT VAILD---->XXXXXXX");
        }

        //杞崲鎴恈har *瀛樻斁鍦ㄦ湰鍦版寚閽�
          cer_char  = (char*)env->GetStringUTFChars(jstringID,NULL);

        //妯℃嫙鍔犲瘑鐩存帴鎶婃暟鎹繛鎺ヨ捣鏉�,鍘熷皝涓嶅姩杩斿洖
          strcat(cer_data,cer_char);
    }

    __android_log_print(ANDROID_LOG_INFO, "NATIVE:","cer_data    = %s", cer_data);
__END
    return (env->NewStringUTF(cer_data));
 }
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛氱粦瀹氫竴涓瘉涔�,鐩存帴杩斿洖鎴愬姛缁戝畾
//
//**************************************************************************
JINT native_binder (JNIEnv * env, jobject obj, jobject cer, jint cer_length){
    __START

    return 0;
    __END
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛氭鏌ュ弬鏁版槸鍚︾粦瀹�,鐩存帴杩斿洖 0 == OK
//
//**************************************************************************
JINT native_unbinder (JNIEnv * env, jobject obj, jobject cer, jint cer_length){
    __START
    return 0;
    __END
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛氭鏌ヨ瘉涔︽槸鍚︾粦瀹�,鐩存帴杩斿洖
//
//**************************************************************************
JINT native_is_bind_cer (JNIEnv * env, jobject obj, jobject cer, jint cer_length){
    __START


    __END
    return 0;
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛氭鏌ヨ瘉涔︽潈闄�,鐩存帴杩斿洖0琛�
//
//**************************************************************************
JINT native_get_sections_status (JNIEnv *env, jobject obj, jint cer_length){
    __START


    __END
    return 0;
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛氱洰鍓嶄笉闇�瑕佸疄鐜�
//
//**************************************************************************
JSHORTARRAY native_genr_wave (JNIEnv *env, jobject obj, jstring section, jstring data){
    __START

    __END
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛氱洰鍓嶄笉闇�瑕佸疄鐜�
//
//**************************************************************************
JINT native_init_recog  (JNIEnv *env, jobject obj){
    __START

    __END
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛�
//
//**************************************************************************
JINT native_start_recog(JNIEnv *env, jobject obj, jobject cfg){
    __START
    int error = 0;

    //鍒ゅ畾鍚姩鐘舵��
    switch(g_java->first_start)
    {
        //寮�鏈鸿繍琛�,鎵ц鍒濆鍖栧姩浣�
        case 0:
                save_java_option(env,obj);//淇濆瓨java鏁版嵁鍒板叏灞�缁撴瀯浣撲腑
                error =save_sys_cfg(env,obj,cfg);//淇濆瓨鎴栧垯瑁呭湪鏁版嵁,鍑嗗鍚姩
                g_java->first_start = 1;

//	            int     gap;        //!鍚屾澶辫触鍚庣郴缁烣FT绐楀彛璺宠浆澶у皬      4
//	            int     buffer_len; //!闊抽鏁版嵁缂撳啿鍖哄ぇ灏�               4
//	            int     start_feq[4];  //!寮�濮嬮鐜�                       4
//	            int     end_feq[4];    //!缁撴潫棰戠巼                       4
//	            int     fft_points; //!fft鐐规暟
//	            int     fft_chains; //!fft淇￠亾鏁�
//	            double  st;         //!鐩歌瘑搴﹂槇鍊糞imilarity threshold  8

//鍒濆鍖栨暟鎹�          int data_init(int n_v, double st_v, int gap_v, double *sFeq_v, double *eFeq_v,int feq_size)
                data_init(	g_java->sys_cfg.fft_points,\
                            g_java->sys_cfg.st,\
                            g_java->sys_cfg.gap,\
                           (double*)g_java->sys_cfg.start_feq,\
                           (double*)g_java->sys_cfg.end_feq,
                            g_java->sys_cfg.fft_chains
                );
                 setBufferSize( g_java->sys_cfg.buffer_len);
                //寮�濮嬪鐞嗘暟鎹�
                start();
            break;

        case 1:
                //绗簩娆¤繘鍏tart
                //閰嶇疆涓嬪弬鏁�
                //鐩存帴鍚姩
                data_init(	g_java->sys_cfg.fft_points,\
                            g_java->sys_cfg.st,\
                            g_java->sys_cfg.gap,\
                           (double*)g_java->sys_cfg.start_feq,\
                           (double*)g_java->sys_cfg.end_feq,
                            g_java->sys_cfg.fft_chains
                );
                setBufferSize( g_java->sys_cfg.buffer_len);
                start();
            break;

        case 2:
            break;

        default:
            error = 1;
        break;
    }

    __END
    return error;
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛�
//
//**************************************************************************
JINT native_pause_recog(JNIEnv *env, jobject obj){
    __START

     __END
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛�
//
//**************************************************************************
JINT native_restart_recog(JNIEnv *env, jobject obj){
    __START

     __END
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛�
//
//**************************************************************************
JINT native_stop_recog(JNIEnv *env, jobject obj){
    __START
    stop();
     __END
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛�
//
//**************************************************************************
JINT native_wrire_recog (JNIEnv *env, jobject obj, jshortArray waves, jint waves_length){
    __START
        jshort *carr;
        carr = env->GetShortArrayElements( waves, NULL);
        int re=wirte(carr,waves_length);
        env->ReleaseShortArrayElements(waves,carr,0);
        return re ;
    __END
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛�
//
//**************************************************************************
JINT native_set_recog_config(JNIEnv *env, jobject obj, jobject cfg){
    __START


    __END
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	        娉� 锛�
//
//**************************************************************************
JOBJECT native_get_recog_config(JNIEnv *env, jobject obj){
    __START

    __END
}
//***************************************************************************
//鍑芥暟鍚嶇О锛歯ative_getRecogStatus
//鍑芥暟鍔熻兘锛氭祴璇曠敤缁戝畾鏈湴鍑芥暟
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	         娉� 锛� 0.璋冭瘯鐨勬椂鍊欎綔涓虹郴缁熺殑娴嬭瘯鍑芥暟
//			1.鍘绘帀绯荤粺鑷姩鐢熸垚鐨勫嚱鏁板悕鍓嶇紑
//			2.娴嬭瘯淇濆瓨鍏ㄥ眬鐨勭幆澧冨彉閲忓拰鏂规硶鐨処D
//			3.娴嬭瘯鍏ㄥ眬鍥炶皟鍑芥暟
//			4.娴嬭瘯鍏ㄥ眬鍙橀噺鏁版嵁缁撴瀯浣�
//			5.娴嬭瘯JAVA缁撴瀯浣撳弬鏁拌緭鍏�,
//			6.娴嬭瘯C缁撴瀯浣撲綔涓鸿繑鍥炲��
//
//**************************************************************************
JOBJECT native_get_recog_status(JNIEnv *input_env, jobject obj)
{
    __START

    //鑾峰彇JAVA涓殑瀹炰緥绫�
     jclass objectClass = (input_env)->FindClass("net/telesing/tsdk/tlib/RecogStatus");

    //鑾峰彇绫讳腑姣忎竴涓彉閲忕殑瀹氫箟
    jfieldID j_ss			 = input_env->GetFieldID(objectClass, "ss", "D");
    jfieldID j_blank_buffer  = input_env->GetFieldID(objectClass, "blankBuffer", "D");
    jfieldID j_recog_stat    = input_env->GetFieldID(objectClass, "recogStat", "I");

    //鏂板缓涓�涓璞＄敤浜庤繑鍥炲埌Java涓�
    jobject status = input_env->AllocObject(objectClass);

    //缁欐瘡涓�涓疄渚嬬殑鍙橀噺

    input_env -> SetDoubleField(status, j_ss          ,g_java->sys_status.ss );
    input_env -> SetDoubleField(status, j_blank_buffer,g_java->sys_status.blank_buffer);
    input_env -> SetIntField(   status, j_recog_stat  ,g_java->sys_status.recog_state );

    __END
    return status;
}
//***************************************************************************
//
//							鑷姩娉ㄥ唽鍑芥暟鍒楄〃
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
//鍑芥暟鍚嶇О锛歳egisterNativeMethods
//鍑芥暟鍔熻兘锛氭敞鍐屽崟涓湰鍦版柟娉曞埌铏氭嫙鏈轰腑
//杈撳叆鍙傛暟锛氭柟娉曟暟缁�
//杈撳嚭鍙傛暟锛�
//澶�	         娉� 锛�
//			clazz = env->FindClass(env, className); C涓棿浣跨敤鐨勬柟娉�
//			clazz = env->FindClass(className);//C++浣跨敤鐨勬柟娉�
//			if (env->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
//			if (env->RegisterNatives( clazz, gMethods, numMethods) < 0) {
//**************************************************************************

static int register_native_methods(JNIEnv* env, const char* className,
                                   JNINativeMethod* g_methods,
                                   int num_methods)
{

    jclass clazz;

    clazz = env->FindClass(className);//C++浣跨敤鐨勬柟娉�
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, g_methods, num_methods) < 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}
//***************************************************************************
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛氭牴鎹湰鍦板嚱鏁板垪琛ㄦ寚姝�,娉ㄥ唽鍑芥暟鍒癡M涓�
//杈撳叆鍙傛暟锛欽VM铏氭嫙鏈�
//杈撳嚭鍙傛暟锛�
//澶�	         娉� 锛�
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
//鍑芥暟鍚嶇О锛�
//鍑芥暟鍔熻兘锛�
//杈撳叆鍙傛暟锛�
//杈撳嚭鍙傛暟锛�
//澶�	         娉� 锛�
//			!鑷姩娉ㄥ唽鏈湴鍑芥暟
//			!鐢宠java瀵硅薄鐨勪娇鐢ㄧ殑绌洪棿,涓�涓粨鏋勪綋
// 			if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK)
//			if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
//**************************************************************************

JINT JNI_OnLoad(JavaVM *vm, void *reserved)
 {

    //鐢ㄤ簬淇濆瓨鍏ㄥ眬鐨凧AVA鏁版嵁缁撴瀯浣�
     g_java= new JAVA;
    //鍒ゅ畾绯荤粺鍚姩start鍑芥暟姝ゆ椂
     g_java->first_start = 0;

     //灏忔妧宸�,鐢ㄤ簬鍒ゅ畾鐢ㄦ埛鏄惁杈撳叆閰嶇疆鍙傛暟,
     //鐢ㄦ埛寮�杈熺殑缂撳啿鍖轰笉鍙兘鏄�0X01澶у皬鐨�
     //绯荤粺榛樿鐨勫垵濮嬪寲涔熶笉鑳芥槸OX01锛岃涔圤XOO.瑕佷箞0XFF

     g_java->sys_cfg.buffer_len = 0;
     g_java->sys_status.ss = 1.0;
     g_java->sys_status.blank_buffer = 1.0;
     g_java->sys_status.recog_state  = 0;
    JNIEnv* env = NULL;

     // 鑾峰彇JNI鐗堟湰
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK){
        INFOR("ERROR IN JNI_VERSION_1_4");
        return JNI_FALSE;

    }
     //娉ㄥ唽鏈湴鍑芥暟
    if (!register_natives(env)){
        INFOR("ERROR IN regedit");
        return JNI_FALSE;
    }

    return JNI_VERSION_1_4;

 }
 //***************************************************************************
 //鍑芥暟鍚嶇О锛欽NI_OnUnload
 //鍑芥暟鍔熻兘锛氳櫄鎷熸満娓呯悊鍑芥暟
 //杈撳叆鍙傛暟锛�
 //杈撳嚭鍙傛暟锛�
 //澶�	 娉� 锛�
 //
 //			娓呯悊鐢宠鐨勫叏灞�寮曠敤绌洪棿
 //**************************************************************************
JVOID JNI_OnUnload(JavaVM* vm, void* reserved){

     delete g_java;
 }

#ifdef __cplusplus
}
#endif
