/*******************************************************************
 *  Copyright(c) 2015-2017 Nanjing telesing
 *  All rights reserved.
 *
 *  文件名称:	Acomms.java
 *  简要描述:	acomms 核心算法Java调用
 *
 *  当前版本:	1.1
 *  作   	    者:	Lee.liang
 *  日         期:
 *  说         明:
 *
 *  取代版本:	1.0
 *  作          者:
 *  日         期:
 *  说         明:
 ******************************************************************/
package net.telesing.tsdk.tlib;

import org.w3c.dom.Text;

import android.R.integer;
import android.R.string;
import android.util.Log;

import net.telesing.tsdk.tlib.RecogStatus;
import net.telesing.tsdk.tlib.RecogCfg;
import net.telesing.tsdk.tlib.CerInfor;


//!声波通信算法接口
public class Acomms {
	

	
	//!根据输入的证书结构体加密
	public native String encryptCer(CerInfor cer,int cerLen); //ok
	
	///!根据输入的证书结构体解密
	public native CerInfor decryptCer(String cer,int cerLen);
	
	//!绑定证书
	 public native int bindCer(CerInfor cer,int cerLen);
	 
	//!消除证书
	 public native int unbindCer(CerInfor cer,int cerLen);
	 
	//!检查证书
	 public native int isBindCer(CerInfor cer,int cerLen);
	 
	//!检查证书内容
	 public native int getSectionsStatus(CerInfor section);
	 
	//!生成包含特定内容的波形文件
	 public native short[] genrWave(String section,String data);
	 
	//初始化解析
	 public native int initRecog();
	 
	//!开始解析
	 public native int startRecog(RecogCfg cfg);
	 
	//!暂停解析
	 public native int pauseRecog();
	 
	//!重启解析
	 public native int restartRecog();
	 
	//!停止解析
	 public native int stopRecog();
	 
	//!写入带解析的音频文件
	 public native int writeRecog(short[] waves,int waveLen);
	 
	//!设置解析参数
	 public native int setRecogConfig(RecogCfg cfg);
	 
	//!获取解析参数
	 public native RecogCfg getRecogConfig();
	 
	 //!获取解析参数配置信息
	 //!本地静态函数
	 public native  RecogStatus getRecogStatus();  
	 
	 
//!NDK回调函数
	 //参数更新
	 public void recogResult(String section,String data,int times)
	 {
		 Log.e("[APP]:","回调recogResult() success");
		 Log.e("[APP]:","section  = "+section);
		 Log.e("[APP]:","data     = "+data);
		 Log.e("[APP]:","times    = "+times);		 
	 }

	 
	 //!加载tlib库函数
	 static {
		     try{
		         Log.e("[APP]:","加载解析库函数成功success");
		         System.loadLibrary("tlib");
		     }catch (Exception e){
		         Log.e("system.loadlibrary:",e.getMessage());
		     }
	
	 }
}
