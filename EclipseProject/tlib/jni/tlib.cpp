#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <assert.h>
#include "tlib.h"
#include <time.h>
#include ".\core\dem.h"
#include ".\core\core.h"
#ifdef __cplusplus
extern "C" {
#endif


//***************************************************************************
//!包名称
#define JNIREG_CLASS "net/telesing/tsdk/tlib/Acomms"

//**************************************************************************

#define __START     __android_log_print(ANDROID_LOG_INFO, "NATIVE:","START:line %d, in function %s",__LINE__,__FUNCTION__);
#define __END 		__android_log_print(ANDROID_LOG_INFO, "NATIVE:","END  :line %d, in function %s",__LINE__,__FUNCTION__);
//**************************************************************************

//简洁函数名称
//返回值			 JNI指定值
#define JINT		JNIEXPORT jint JNICALL
#define	JVOID		JNIEXPORT void JNICALL
#define	JOBJECT	 	JNIEXPORT jobject JNICALL
#define	JSTRING  	JNIEXPORT jstring JNICALL
#define JSHORTARRAY JNIEXPORT jshortArray JNICALL


//指针数组,用来存放接收到的解密后的字符
 char store_cer_section_length[]={4,10,1,10,10,19,1,15,64};

 char store_cer_string[][65] = {  "4444",      //码段
								  "YYYY-MM-DD",//失效日期    YYYY-MM-DD
								  "1",          //权限       ’1‘生成权限  ’2‘ 识别权限 ’3‘通用 =  生成 + 识别
								  "1234567890", //编号       例如”232134234“ 正整数
								  "1234567890", //机构编号    例如”1123123123“ 正整数
								  "2015-12-09 11:11:11",//生成时间 YYYY-MM-DD hh:mm:ss
								  "1",					//生成方式 '1' 网站获取 ‘2’ 接口获取(API)
								  "192.168.100.100",    //生成IP
								  "1111111122222222333333334444444455555555666666667777777788888888",//码段来源    例如 ”智声E联“
};
//回调函数
typedef struct {
	const char* packagename;
	const char* name;
	const char* signature;
} JNICallBackMethod;

//证书签名用结构体
typedef struct {
	const char* name;
	const char* signature;
} CER_INFOR;

//全局的java选项
JAVA *g_java ;

//全局算法
CORE core;

static JNICallBackMethod g_cb_methods[] = {
		{"net/telesing/tsdk/tlib/Acomms","recogResult", "(Ljava/lang/String;Ljava/lang/String;I)V"},
};

int check_sections_state(char *section){
	char cer_store[4];
		int  authority = 0;

		//证书信息保存到本地
		  for(int i = 0 ; i < 4 ; i++){
			  cer_store[i] = *(section + i);
		  }

		//证书缓冲区找断码绑定的权限
		for(int i= 0 ; i< core.cer_max_number; i ++){

			//通用权限
			if(0 == strncmp(core.cer_generate_recognise[i].section,cer_store,4)){

				//证书还未到期
				if(core.generate_recognise_days_left[i] >0){
					return 3;
				}
			}
			//识别权限
			if(0 == strncmp(core.cer_recognise[i].section,cer_store,4)){

				//证书还未到期
				if(core.recognise_days_left[i] > 0){
					return 2;
				}
			}
			//生成权限
			if(0 == strncmp(core.cer_generate[i].section,cer_store,4)){
				//证书还未到期
				if(core.generate_days_left[i] > 0){
					return 1;
				}
			}
		}


	    return 0;
}
//***************************************************************************
//函数名称：产生长度为length的随机字符串
//函数功能：
//输入参数：
//输出参数： 无
//备	        注 ：
//
//**************************************************************************
static char* gen_random_string(int length)
{
	int flag, i;
	char* string;
	srand((unsigned) time(NULL ));
	if ((string = (char*) malloc(length)) == NULL )
	{

		return NULL ;
	}

	for (i = 0; i < length - 1; i++)
	{
		flag = rand() % 3;
		switch (flag)
		{
			case 0:
				string[i] = 'A' + rand() % 26;
				break;
			case 1:
				string[i] = 'a' + rand() % 26;
				break;
			case 2:
				string[i] = '0' + rand() % 10;
				break;
			default:
				string[i] = 'x';
				break;
		}
	}
	string[length - 1] = '\0';
	return string;
}
//***************************************************************************
//函数名称：callback_java
//函数功能：同步回掉函数
//输入参数： char *section, 	证书段
//			char *data		解析到的数据
//			int times		解析花费时间单位ms
//输出参数： 无
//备	        注 ：只能在线程里面调用,不能在非线程
//
//
//**************************************************************************
void callback_java( char *section, char *data,int times){

	JNIEnv* input_env;

	g_java->jvm->AttachCurrentThread(&input_env, NULL);//获取线程环境

	//调用返回值为void的方法
	input_env->CallVoidMethod(  g_java->jobj, g_java->cb_recog_result,
								input_env->NewStringUTF(section),	//传入证书信息
								input_env->NewStringUTF(data),	  	//传入数据信息
								times);							  	//传入时间信息

	g_java->jvm->DetachCurrentThread();

}
//***************************************************************************
//函数名称：GetClassMethodID
//函数功能：根据函数名和参数列表找到回调函数ID
//输入参数： JNIEnv* env:						线程环境
//			const char* class_pathName,     类名称
//			const char* function_Name,		函数名称
//			const char* arg_format			函数参数签名
//
//输出参数：用于保存的方法的ID，返回值保存下来的一定是全局变量
//备	         注 ：
//			由于NDK的BUG，该函数的返回值回一直整个程序的生命周期
//			 relese
//			去掉错误提示信息
//**************************************************************************
static jmethodID get_class_method_id( JNIEnv* env,
								  	  const char* class_pathName,
									  const char* function_Name,
									  const char* arg_format
									)
{
	jclass clazz = env->FindClass(class_pathName); //classPathName完整的包名加类名

	jmethodID j_callback = env->GetMethodID(clazz, function_Name, arg_format); //获取java层方法id

	return j_callback;
}

static CER_INFOR g_cer_infor[] = {
		{"section"   ,"Ljava/lang/String;"},
		{"expiration","Ljava/lang/String;"},
		{"authority" ,"Ljava/lang/String;"},
		{"ID"        ,"Ljava/lang/String;"},
		{"orgID"     ,"Ljava/lang/String;"},
		{"genrTime"  ,"Ljava/lang/String;"},
		{"genrType"  ,"Ljava/lang/String;"},
		{"genrIp"    ,"Ljava/lang/String;"},
		{"sectSrc"   ,"Ljava/lang/String;"}
};
//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：保存全局变量,处理回掉函数
//
//**************************************************************************
static void save_java_option(JNIEnv *input_env, jobject obj){

	//获取JVM虚拟机并且保存在全局变量中
	input_env->GetJavaVM(&g_java->jvm);
	//@获取对象
	g_java->jobj = input_env->NewGlobalRef(obj);

	jclass javaClass = input_env->GetObjectClass(obj);
	//@对象转为全局引用
	g_java->jclass = (jclass)input_env->NewGlobalRef(javaClass);

	jmethodID *cb_method_ids = new jmethodID[sizeof(g_cb_methods) / sizeof(g_cb_methods[0])];

	//找到系统中所有的回掉方法
	for( int i = 0 ; i < sizeof(g_cb_methods) / sizeof(g_cb_methods[0]);i ++){

		*(cb_method_ids++)  = get_class_method_id(  input_env,
													g_cb_methods[i].packagename,
													g_cb_methods[i].name,
													g_cb_methods[i].signature);	}
	//保存回掉函数方法ID为全局变量
	g_java->cb_recog_result = *(--cb_method_ids);

	delete []cb_method_ids;

}
//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：载入用户配置参数
//**************************************************************************
static int load_sdk_cfg(JNIEnv *input_env, jobject obj,jobject cfg){

	//获取Java中的实例类
	jclass objectClass = input_env->FindClass("net/telesing/tsdk/tlib/RecogCfg");

	//获取类中每一个变量的定义
	jfieldID  jstart_feq   = input_env->GetFieldID(objectClass,"startFeq", "[I");
	jfieldID  jend_feq     = input_env->GetFieldID(objectClass,"endFeq"  , "[I");
	jfieldID  jbuffer_len = input_env->GetFieldID(objectClass, "bufferLen", "I");
	jfieldID  jgap        = input_env->GetFieldID(objectClass, "gap"      , "I");
	jfieldID  jfft_points = input_env->GetFieldID(objectClass, "fftPoints", "I");
	jfieldID  jfft_chains = input_env->GetFieldID(objectClass, "fftChains", "I");

	jfieldID  jst         = input_env->GetFieldID(objectClass, "st"        ,"D");

	//获取Java中数组属性arrays的对象
	 jintArray  istart_feq = (jintArray)( input_env )->GetObjectField( cfg, jstart_feq );
	 jintArray  iend_feq   = (jintArray)( input_env )->GetObjectField( cfg, jend_feq   );

	 //数组里面一定要有数据
	 if( istart_feq == NULL|| iend_feq == NULL){
		 return 1;
	 }
	 //获取arrays对象的指针
	 jint* int_start_feq = input_env->GetIntArrayElements(istart_feq,0);
	 jint* int_end_feq   = input_env->GetIntArrayElements(iend_feq  ,0);

	 //赋值过来
	 for(int i = 0 ; i < 4; i ++){

		 g_java->sys_cfg.start_feq[i] = *(int_start_feq + i);
		 g_java->sys_cfg.end_feq[i]   = *(int_end_feq   + i);
	 }


	//int类型变量
	g_java->sys_cfg.gap			= input_env->GetIntField(cfg,jgap);
	g_java->sys_cfg.fft_points  = input_env->GetIntField(cfg,jfft_points);
	g_java->sys_cfg.fft_chains  = input_env->GetIntField(cfg,jfft_chains);
	g_java->sys_cfg.buffer_len  = input_env->GetIntField(cfg,jbuffer_len);

	//浮点类型的变量
	g_java->sys_cfg.st  		= input_env->GetDoubleField(cfg,jst);

	__android_log_print(ANDROID_LOG_INFO, "SET:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[0]);
	__android_log_print(ANDROID_LOG_INFO, "SET:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[1]);
	__android_log_print(ANDROID_LOG_INFO, "SET:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[2]);
	__android_log_print(ANDROID_LOG_INFO, "SET:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[3]);

	__android_log_print(ANDROID_LOG_INFO, "SET:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[0]);
	__android_log_print(ANDROID_LOG_INFO, "SET:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[1]);
	__android_log_print(ANDROID_LOG_INFO, "SET:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[2]);
	__android_log_print(ANDROID_LOG_INFO, "SET:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[3]);

	__android_log_print(ANDROID_LOG_INFO, "SET:","g_java->sys_cfg.gap        = %d", g_java->sys_cfg.gap);
	__android_log_print(ANDROID_LOG_INFO, "SET:","g_java->sys_cfg.fft_points = %d", g_java->sys_cfg.fft_points);
	__android_log_print(ANDROID_LOG_INFO, "SET:","g_java->sys_cfg.fft_chains = %d", g_java->sys_cfg.fft_chains);
	__android_log_print(ANDROID_LOG_INFO, "SET:","g_java->sys_cfg.buffer_len = %d", g_java->sys_cfg.buffer_len);

	__android_log_print(ANDROID_LOG_INFO, "SET:","g_java->sys_cfg.st         = %f", g_java->sys_cfg.st);
	return 0;
}
//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：载入系统默认配置参数
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

//	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[0]);
//	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[0]);
//	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[1]);
//	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[1]);
//	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[2]);
//	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[2]);
//	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.start_feq  = %d", g_java->sys_cfg.start_feq[3]);
//	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.end_feq    = %d", g_java->sys_cfg.end_feq[3]);
//	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.gap        = %d", g_java->sys_cfg.gap       );
//	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.fft_points = %d", g_java->sys_cfg.fft_points);
//	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.fft_chains = %d", g_java->sys_cfg.fft_chains);
//	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.buffer_len = %d", g_java->sys_cfg.buffer_len);
//	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","g_java->sys_cfg.st         = %f", g_java->sys_cfg.st        );

}
//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：保存系统配置参数
//              	public int[] start_feq;	//启始频率
//					public int[] end_feq;	//截止频率
//					public int 		gap   ;		//跳转间隙
//					public double 	st ;		//相似度
//					public int 		fft_points; //FFT点数
//					public int 		fft_chains;	//FFT信道数
//					public int 		fft_gap;	//同步失败后跳转间隙
//**************************************************************************
static int save_sys_cfg(JNIEnv *input_env, jobject obj,jobject cfg){

	//判定载入默认参数还是系统配置参数,根据初始化时候用于缓冲区大小
	if(0X01 ==g_java->sys_cfg.buffer_len){

		INFOR("LOAD DEFAULT ARGS------>");

		return load_default_cfg();
	}else{
	//	return load_default_cfg();
		return load_sdk_cfg(input_env, obj,cfg);
	}
}
//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：没有填写算法,不管你TM输入的是啥,我都TM的返回一个规定的CER
//
//**************************************************************************
JOBJECT native_decrypt_cer(JNIEnv *env, jobject obj, jstring str){
__START
	jfieldID jfield_cer;
	jstring  jstringID;

	const char  magic_data[]="QUEYIN";
	char cer_magic[7];
	char cer_data[200];//用来存放收到的证书
	const char *c_str; //java传过来的
	char *p_ce_rdata=cer_data;
	char  cer_store_data[65];
	char  p_cer[2048];
	char *cer_char;
	int  offset;

	memset(cer_data,0,255*sizeof(char));
	memset(cer_magic,0,6);
	memset(cer_store_data,0,65);

	 char *p = NULL;
	//获取JAVA中的实例类
	 jclass objectClass = env->FindClass("net/telesing/tsdk/tlib/CerInfor");

	 //申请到用于返回的cer
	 jobject cer		= env->AllocObject(objectClass);

	//获取字符串,等待解密
	c_str = env->GetStringUTFChars(str,NULL);

	//复制字符串进来,准备处理
	memcpy(p_cer,c_str,2048);

	//把送进来的字符串送到解密算法中去解密
	p_ce_rdata = core.cer_decrypt(p_cer);

	//解密后放在本地
	for(int i = 0 ; i < 200; i++){

		cer_data[i] = *(p_ce_rdata+i);
	}
	__android_log_print(ANDROID_LOG_INFO, "SDK:","cer_data= %s", cer_data );

	//找到魔术字
	strncpy(cer_magic,&cer_data[20],6);

	//准备校验
	cer_magic[6]='\0';

	//校验失败
	if(0!= strcmp(cer_magic,magic_data)){
		__android_log_print(ANDROID_LOG_INFO, "SDK:","CER ERROR!!!!!!!!!");
		__android_log_print(ANDROID_LOG_INFO, "SDK:","cer_magic= %s", cer_magic );
		return cer;
	}

	offset = 0;
	int i = 0;
	//替换本地证书,准备返回,
	for( i = 0 ; i < (sizeof(store_cer_section_length)) ; i++){

		//复制到对应的位置去
		memcpy(store_cer_string[i],&cer_data[65 + offset],store_cer_section_length[i]);

		//末尾加个结束字符串
		store_cer_string[i][store_cer_section_length[i]] = '\0';

		//提示消息
		__android_log_print(ANDROID_LOG_INFO, "SDK:","store_cer_string[%d] %s----%d",i, store_cer_string[i],sizeof(store_cer_string[i]) );

		//偏移
		offset += store_cer_section_length[i];
	}
	//最后面的源字段一定能够要加上结束字符	,其他字段全部都是固定的,至少目前不会变化
	  *store_cer_string[i-1 + store_cer_section_length[i-1]*(i-1)] = '\0';
	__android_log_print(ANDROID_LOG_INFO, "SDK:","cer_magic= %s", cer_magic );

	 //改变下申请到的数据准备返回给JAVA用
	 for(int i = 0;i < sizeof(g_cer_infor)/sizeof(g_cer_infor[0]);i ++){

		//获取标签并存储
		 jfield_cer = env->GetFieldID(objectClass, g_cer_infor[i].name,g_cer_infor[i].signature);

		 if(jfield_cer == NULL){
				__android_log_print(ANDROID_LOG_INFO, "NATIVE:","AAAAAAAAAAAi= %d", i   );
			 return 0;
		 }
		 //新建一个变量用于转换字符串
		 jstring jstrTmp = env->NewStringUTF(*(store_cer_string+i));

		 //修改证书
		 env->SetObjectField(cer, jfield_cer,jstrTmp);

		 //释放刚才新建的
		 env->DeleteLocalRef(jstrTmp);

	 }
	 env->ReleaseStringUTFChars(str, c_str);
 __END
	 return cer;

}
//***************************************************************************
//函数名称： 1024RSA分组加密
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：CerInfor 中的每一个字段一定要保证全部填写完整,
//		   GetStringUTFChars如果没有正确的字符串传入回内存越界错误
//**************************************************************************
JSTRING native_encrypt_cer(JNIEnv *env, jobject obj, jobject cer){

	__START

	jfieldID jfield_cer;
	jstring  jstringID;

	 CER cer1temp;

	//指针数组存放接收到准备加密的证书
	 char cer_src_data[255];

	 //加密后密文存放点
	 char cer_enc_data[2048]={0};

	 char *p_cer;

	 //用来判定证书是否本人生成
	 char  magic_data[]="QUEYIN";
	 char  random_string[66];
	 char *p_random_string = random_string;
	 char *cer_char;

	 //清理工作区
	 memset(cer_src_data,0,255*sizeof(char));
	 memset(random_string,0,66*sizeof(char));

	 //区分证书的编号,随机加入一定的字符串66长度
	 p_random_string= gen_random_string(66);

	 for(int i= 0;i<66;i++){
		 random_string[i]=*(p_random_string+i);
	 }
	 //加入魔术字节
	 for(int i = 20 ; i <26; i ++){
		 random_string[i] = magic_data[i-20]; //这地方做好了一堆的附加数据
	 }
	 //链接到数据到上面去
	 strcat(cer_src_data,random_string);

	//获取Java中的RecogCfg实例类
	jclass objectClass  = env->FindClass("net/telesing/tsdk/tlib/CerInfor");

	//获取RecogCfg类中每一个变量的定义
	for(int i = 0;i < sizeof(g_cer_infor)/sizeof(g_cer_infor[0]);i ++){

		//获取标签并存储
		 jfield_cer = env->GetFieldID(objectClass, g_cer_infor[i].name,g_cer_infor[i].signature);

		//转换成jstring
		 jstringID  = (jstring)env->GetObjectField(cer, jfield_cer);

		if( jstringID == NULL){
			return 	env->NewStringUTF("XXXXXXXXERROR CER IS NOT VAILD---->XXXXXXX");
		}

		//转换成char *存放在本地指针
		  cer_char  = (char*)env->GetStringUTFChars(jstringID,NULL);

		  //跟新解密后的字符串的长度
		  store_cer_section_length[i] = strlen(cer_char);

		  //跟新后的字符串的长度
		  core.store_cer_section_length[i] = strlen(cer_char);

			__android_log_print(ANDROID_LOG_INFO, "NATIVE:","store_cer_section_length[%d] = %d",i, store_cer_section_length[i]   );

		//链接成字符串,用来后面算法内核中的加解密
		  strcat(cer_src_data,cer_char);
	}

	//6666666666666666666666666666666
	//包含伪随机字符串和用于验证的签名
	//共计70+64+66=200个
	//66666666666666666666666666666
	__android_log_print(ANDROID_LOG_INFO, "NATIVE:","INPUT_CER= %s", cer_src_data   );

    //@加密开始
	p_cer = core.cer_encrypt(cer_src_data);

	//准备输出到JAVA
	for(int i = 0 ;i <2048;i++){
		cer_enc_data[i] = *(p_cer+i);
	}

	__END
	//JAVA收到的加密信息
	 return (env->NewStringUTF(cer_enc_data));
 }
//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：绑定一个证书,直接返回成功绑定
//
//**************************************************************************
JINT native_binder (JNIEnv * env, jobject obj, jstring str){
	__START

	//获取证书的内容先
	const char  magic_data[]="QUEYIN";
	char cer_magic[7];
	char cer_data[200];//用来存放收到的证书
	const char *c_str; //java传过来的
	char *p_ce_rdata=cer_data;
	char  cer_store_data[65];
	char  p_cer[2048];
	char *cer_char;
	int  offset;

	memset(cer_data,0,255*sizeof(char));
	memset(cer_magic,0,6);
	memset(cer_store_data,0,65);

	//获取字符串,等待解密
	c_str = env->GetStringUTFChars(str,NULL);

	//复制字符串进来,准备处理
	memcpy(p_cer,c_str,2048);

	//把送进来的字符串送到解密算法中去解密
	p_ce_rdata = core.cer_decrypt(p_cer);

	//解密后放在本地
	for(int i = 0 ; i < 200; i++){

		cer_data[i] = *(p_ce_rdata+i);
	}

	//找到魔术字
	strncpy(cer_magic,&cer_data[20],6);

	//准备校验
	cer_magic[6]='\0';

	//校验失败
	if(0!= strcmp(cer_magic,magic_data)){
		__android_log_print(ANDROID_LOG_INFO, "BIND:","CER ERROR!!!!!!!!!");
		__android_log_print(ANDROID_LOG_INFO, "BIND:","cer_magic= %s", cer_magic );
		return - 1;
	}
	__END
	//送到内核绑定
	return core.cer_bind(cer_data);;

}
//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：检查参数是否绑定,直接返回 0 == OK
//
//**************************************************************************
JINT native_unbinder (JNIEnv * env, jobject obj, jstring str ){
	__START

	//获取证书的内容先
	const char  magic_data[]="QUEYIN";
	char cer_magic[7];
	char cer_data[200];//用来存放收到的证书
	const char *c_str; //java传过来的
	char *p_ce_rdata=cer_data;
	char  cer_store_data[65];
	char  p_cer[2048];
	char *cer_char;
	int  offset;

	memset(cer_data,0,255*sizeof(char));
	memset(cer_magic,0,6);
	memset(cer_store_data,0,65);

	//获取字符串,等待解密
	c_str = env->GetStringUTFChars(str,NULL);

	//复制字符串进来,准备处理
	memcpy(p_cer,c_str,2048);

	//把送进来的字符串送到解密算法中去解密
	p_ce_rdata = core.cer_decrypt(p_cer);

	//解密后放在本地
	for(int i = 0 ; i < 200; i++){

		cer_data[i] = *(p_ce_rdata+i);
	}

	//找到魔术字
	strncpy(cer_magic,&cer_data[20],6);

	//准备校验
	cer_magic[6]='\0';

	//校验失败
	if(0!= strcmp(cer_magic,magic_data)){
		__android_log_print(ANDROID_LOG_INFO, "UBIND:","CER ERROR!!!!!!!!!");
		__android_log_print(ANDROID_LOG_INFO, "UBIND:","cer_magic= %s", cer_magic );
		return - 1;
	}
	__END

	//送到内核解绑定
	return core.cer_unbind(cer_data);
	__END
}
//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：检查证书是否绑定,直接返回
//
//**************************************************************************
JINT native_is_bind_cer (JNIEnv * env, jobject obj,jstring str ){
	__START

	//获取证书的内容先
	const char  magic_data[]="QUEYIN";
	char cer_magic[7];
	char cer_data[200];//用来存放收到的证书
	const char *c_str; //java传过来的
	char *p_ce_rdata=cer_data;
	char  cer_store_data[65];
	char  p_cer[2048];
	char *cer_char;
	int  offset;

	memset(cer_data,0,255*sizeof(char));
	memset(cer_magic,0,6);
	memset(cer_store_data,0,65);

	//获取字符串,等待解密
	c_str = env->GetStringUTFChars(str,NULL);

	//复制字符串进来,准备处理
	memcpy(p_cer,c_str,2048);

	//把送进来的字符串送到解密算法中去解密
	p_ce_rdata = core.cer_decrypt(p_cer);

	//解密后放在本地
	for(int i = 0 ; i < 200; i++){

		cer_data[i] = *(p_ce_rdata+i);
	}

	//找到魔术字
	strncpy(cer_magic,&cer_data[20],6);

	//准备校验
	cer_magic[6]='\0';

	//校验失败
	if(0!= strcmp(cer_magic,magic_data)){
		__android_log_print(ANDROID_LOG_INFO, "UBIND:","CER ERROR!!!!!!!!!");
		__android_log_print(ANDROID_LOG_INFO, "UBIND:","cer_magic= %s", cer_magic );
		return - 1;
	}
	__END

	//送到内核检查是不是绑定好了
	return core.cer_isbind(cer_data);
}
//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：检查证书权限,直接返回0表
//
//**************************************************************************
JINT native_checkSectionsBindState (JNIEnv *env, jobject obj, jstring str){
	__START

	//获取证书的内容先
	const char  magic_data[]="QUEYIN";
	char cer_magic[7];
	char cer_data[200];//用来存放收到的证书
	const char *c_str; //java传过来的
	char *p_ce_rdata=cer_data;
	char  cer_store_data[65];
	char  p_cer[2048];
	char *cer_char;
	int  offset;

	memset(cer_data,0,255*sizeof(char));
	memset(cer_magic,0,6);
	memset(cer_store_data,0,65);

	//获取字符串,等待验证
	c_str = env->GetStringUTFChars(str,NULL);

	//复制字符串进来,准备处理
	memcpy(p_cer,c_str,2048);

	//把送进来的字符串送到验证算法中去验证

	return core.checkSectionsBindState(p_cer);
}
//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：目前不需要实现
//
//**************************************************************************
JSHORTARRAY native_genr_wave (JNIEnv *env, jobject obj, jstring section, jstring data){
	__START

	__END
}

//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：
//
//**************************************************************************
JINT native_start_recog(JNIEnv *env, jobject obj, jobject cfg){
	__START
///////////////////////////////////////////////////////////////////////////
#if 1
	int error = 0;
	int n_v=2048;
	double st_v=0.15;						//! 相似度
	int gap_v=512;
	int feq_size=4;				//!  信道数
	//! 信道
	double sFeq[]={16, 12, 8, 4};//{5.5125};
	double eFeq[]={20, 16, 12, 8};//{11.025};
#endif

/////////////////////////////////////////////////////////////////////////
	//判定启动状态
	switch(g_java->first_start)
	{
		//开机运行,执行初始化动作
		case 0:
				save_java_option(env,obj);//保存java数据到全局结构体中
				//error =save_sys_cfg(env,obj,cfg);//保存或则装载数据,准备启动

				__android_log_print(ANDROID_LOG_INFO, "START:","!!FIRST READ TO G0G0G0");
				__android_log_print(ANDROID_LOG_ERROR, "START:","!!FIRST READ TO G0G0G0");
				//准备运行
				g_java->first_start = 2;

				//把数据送到内核开始处理
				core.recog_start(g_java->sys_cfg);
				break;
		//被setcfg函数调用过,需要装载setcfg函数带过来的参数值
		case 1:

			//保存全局虚拟机数据
			save_java_option(env,obj);//保存java数据到全局结构体中

			//用户输入的参数有效,但是还是要尝试本次输入的参数
			if(0==load_sdk_cfg(env,obj,cfg)){

				//本次携带参数OK,启动
				core.recog_start(g_java->sys_cfg);
				g_java->first_start = 2;
				__android_log_print(ANDROID_LOG_INFO, "CFGCFGCG:","SETSETSETGOGOGOGOG!!!");
				break;
			}

			//准备运行
			g_java->first_start = 2;

			//开始处理数据
			__android_log_print(ANDROID_LOG_INFO, "START:","SETSETSET!!!");


		case 2:
			//把数据送到内核开始处理
			core.recog_start(g_java->sys_cfg);
			__android_log_print(ANDROID_LOG_INFO, "START:","!!GOGOGGOGOGO");

		default:
			error = 1;
		break;
	}

	__END
	return error;
}


//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：
//
//**************************************************************************
JINT native_stop_recog(JNIEnv *env, jobject obj){
	__START
	core.recog_stop();
	 __END
}
//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：
//
//**************************************************************************
JINT native_wrire_recog (JNIEnv *env, jobject obj, jshortArray waves){

		jshort *carr;
		carr = env->GetShortArrayElements( waves, NULL);
		jint lengh = env->GetArrayLength(waves);
		__android_log_print(ANDROID_LOG_INFO, "NDK_READ_WRITE:","!!%d",lengh );

		int re=core.recog_write(carr,lengh);

		env->ReleaseShortArrayElements(waves,carr,0);
		return re ;

}
//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：
//
//**************************************************************************
JINT native_set_recog_config(JNIEnv *env, jobject obj, jobject cfg){
	__START
	int error = 0;
	error = load_sdk_cfg(env, obj,cfg);
	if(error==0 ){

		//通知Start函数,配置参数有效
		g_java->first_start = 1;
	}else{
		g_java->first_start = 0;
	}



	return error;//保存或则装载数据,准备启动
	__END
}
//***************************************************************************
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	        注 ：
//
//**************************************************************************
JOBJECT native_get_recog_config(JNIEnv *input_env, jobject obj){
	__START

	//获取Java中的实例类
	jclass objectClass = input_env->FindClass("net/telesing/tsdk/tlib/RecogCfg");

	//新建一个变量
	jobject cfg = input_env->AllocObject(objectClass);
	//获取类中每一个变量的定义
	jfieldID  jstart_feq   = input_env->GetFieldID(objectClass,"startFeq", "[I");
	jfieldID  jend_feq     = input_env->GetFieldID(objectClass,"endFeq"  , "[I");
	jfieldID  jbuffer_len  = input_env->GetFieldID(objectClass, "bufferLen", "I");
	jfieldID  jgap         = input_env->GetFieldID(objectClass, "gap"      , "I");
	jfieldID  jfft_points  = input_env->GetFieldID(objectClass, "fftPoints", "I");
	jfieldID  jfft_chains  = input_env->GetFieldID(objectClass, "fftChains", "I");

	jfieldID  jst          = input_env->GetFieldID(objectClass, "st"        ,"D");



	//获取Java中数组属性arrays的对象
	 jintArray  istart_feq = (jintArray)( input_env )->GetObjectField( cfg, jstart_feq );
	 jintArray  iend_feq   = (jintArray)( input_env )->GetObjectField( cfg, jend_feq   );


	 //获取arrays对象的指针 int 类型
	 jint* int_start_feq = (jint *)input_env->GetIntArrayElements(istart_feq,0);
	 jint* int_end_feq   = (jint *)input_env->GetIntArrayElements(iend_feq  ,0);

	 //赋值过来
	 for(int i = 0 ; i < 4; i ++){

	//	 input_env->SetIntArrayElements()
	//	 g_java->sys_cfg.start_feq[i] = *(int_start_feq + i);
	//	 g_java->sys_cfg.end_feq[i]   = *(int_end_feq   + i);
	//	 env->SetObjectArrayElement(result, i, intArray);
	 }

	// input_env->SetIntArrayRegion(g_java->sys_cfg.start_feq, 0, 4, (const jint*)int_start_feq);                 // (2)
	//int类型变量
	input_env->SetIntField(cfg,jgap,g_java->sys_cfg.gap	);
	input_env->SetIntField(cfg,jfft_points,g_java->sys_cfg.fft_points);
	input_env->SetIntField(cfg,jfft_chains,g_java->sys_cfg.fft_chains );
	input_env->SetIntField(cfg,jbuffer_len,g_java->sys_cfg.buffer_len);

	//浮点类型的变量
	input_env->SetDoubleField(cfg,jst,g_java->sys_cfg.st );

	return cfg;
	__END
}
//***************************************************************************
//函数名称：native_getRecogStatus
//函数功能：测试用绑定本地函数
//输入参数：
//输出参数：
//备	         注 ： 0.调试的时候作为系统的测试函数
//			1.去掉系统自动生成的函数名前缀
//			2.测试保存全局的环境变量和方法的ID
//			3.测试全局回调函数
//			4.测试全局变量数据结构体
//			5.测试JAVA结构体参数输入,
//			6.测试C结构体作为返回值
//
//**************************************************************************
JOBJECT native_get_recog_status(JNIEnv *input_env, jobject obj)
{
	__START

	//获取JAVA中的实例类
	 jclass objectClass = (input_env)->FindClass("net/telesing/tsdk/tlib/RecogStatus");

	//获取类中每一个变量的定义
	jfieldID j_ss			 = input_env->GetFieldID(objectClass, "ss", "D");
	jfieldID j_blank_buffer  = input_env->GetFieldID(objectClass, "blankBuffer", "D");
	jfieldID j_recog_stat    = input_env->GetFieldID(objectClass, "recogStat", "I");

	//新建一个对象用于返回到Java中
	jobject status = input_env->AllocObject(objectClass);

	//给每一个实例的变量
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
//							自动注册函数列表
//
//***************************************************************************

static JNINativeMethod g_methods[] = {
	{ "encryptCer"       , "(Lnet/telesing/tsdk/tlib/CerInfor;)Ljava/lang/String;",(void*)native_encrypt_cer},
	{ "decryptCer"       , "(Ljava/lang/String;)Lnet/telesing/tsdk/tlib/CerInfor;",(void*)native_decrypt_cer},
	{ "bindCer"          , "(Ljava/lang/String;)I"  			                  ,(void*)native_binder},
	{ "unbindCer"        , "(Ljava/lang/String;)I"  			                  ,(void*)native_unbinder},
	{ "isBindCer"        , "(Ljava/lang/String;)I"                                ,(void*)native_is_bind_cer},
	{ "checkSectionsBindState","(Ljava/lang/String;)I"                            ,(void*)native_checkSectionsBindState},
//	{ "genrWave"         , "(Ljava/lang/String;Ljava/lang/String;)[S"              ,(void*)native_genr_wave},

	{ "startRecog"       , "(Lnet/telesing/tsdk/tlib/RecogCfg;)I"                  ,(void*)native_start_recog},
	{ "stopRecog"        , "()I"                                                   ,(void*)native_stop_recog},
	{ "writeRecog"       , "([S)I"                                                ,(void*)native_wrire_recog},
	{ "setRecogConfig"   , "(Lnet/telesing/tsdk/tlib/RecogCfg;)I"                  ,(void*)native_set_recog_config},
	{ "getRecogConfig"   , "()Lnet/telesing/tsdk/tlib/RecogCfg;"                   ,(void*)native_get_recog_config},
	{ "getRecogStatus"   , "()Lnet/telesing/tsdk/tlib/RecogStatus;"                ,(void*)native_get_recog_status },
};
//***************************************************************************
//函数名称：registerNativeMethods
//函数功能：注册单个本地方法到虚拟机中
//输入参数：方法数组
//输出参数：
//备	         注 ：
//			clazz = env->FindClass(env, className); C中间使用的方法
//			clazz = env->FindClass(className);//C++使用的方法
//			if (env->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
//			if (env->RegisterNatives( clazz, gMethods, numMethods) < 0) {
//**************************************************************************

static int register_native_methods(JNIEnv* env, const char* className,
								   JNINativeMethod* g_methods,
								   int num_methods)
{

	jclass clazz;

	clazz = env->FindClass(className);//C++使用的方法
	if (clazz == NULL) {
		return JNI_FALSE;
	}
	if (env->RegisterNatives(clazz, g_methods, num_methods) < 0) {
		return JNI_FALSE;
	}

	return JNI_TRUE;
}
//***************************************************************************
//函数名称：
//函数功能：根据本地函数列表指正,注册函数到VM中
//输入参数：JVM虚拟机
//输出参数：
//备	         注 ：
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
//函数名称：
//函数功能：
//输入参数：
//输出参数：
//备	         注 ：
//			!自动注册本地函数
//			!申请java对象的使用的空间,一个结构体
// 			if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK)
//			if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
//**************************************************************************

JINT JNI_OnLoad(JavaVM *vm, void *reserved)
 {

	//用于保存全局的JAVA数据结构体
	 g_java= new JAVA;
	//判定系统启动start函数此时
	 g_java->first_start = 0;

	 //小技巧,用于判定用户是否输入配置参数,
	 //用户开辟的缓冲区不可能是0X01大小的
	 //系统默认的初始化也不能是OX01，要么OXOO.要么0XFF

	 g_java->sys_cfg.buffer_len = 0;
	 g_java->sys_status.ss = 1.0;
	 g_java->sys_status.blank_buffer = 1.0;
	 g_java->sys_status.recog_state  = 0;
	JNIEnv* env = NULL;

	 // 获取JNI版本
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK){
		INFOR("ERROR IN JNI_VERSION_1_4");
		return JNI_FALSE;

	}
	 //注册本地函数
	if (!register_natives(env)){
		INFOR("ERROR IN regedit");
		return JNI_FALSE;
	}

	return JNI_VERSION_1_4;

 }
 //***************************************************************************
 //函数名称：JNI_OnUnload
 //函数功能：虚拟机清理函数
 //输入参数：
 //输出参数：
 //备	 注 ：
 //
 //			清理申请的全局引用空间
 //**************************************************************************
JVOID JNI_OnUnload(JavaVM* vm, void* reserved){

	 delete g_java;
 }

#ifdef __cplusplus
}
#endif
