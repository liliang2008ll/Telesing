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


import android.util.Log;
import net.telesing.tsdk.tlib.bean.SNBean;
import net.telesing.tsdk.tlib.core.Demodulator;


//!声波通信算法接口
public class Acomms {


	// !根据输入的证书结构体加密
	public native String encryptCer(CerInfor cer); // ok

	/**
	 * 解密证书
	 *
	 * @param encCer
	 *            密文证书
	 * @return
	 */
	public native CerInfor decryptCer(String encCer);

	/**
	 * 绑定证书
	 *
	 * @param encCer
	 *            证书内容
	 * @return 0，成功； 非0，错误码
	 */
	public native int bindCer(String encCer);

	/**
	 * 解绑证书
	 *
	 * @param encCer
	 *            密文证书
	 * @return 0，成功； 非0，错误码
	 */
	public native int unbindCer(String encCer);

	/**
	 * 验证证书是否绑定
	 *
	 * @param encCer
	 *            密文证书
	 * @return 0，未绑定； 1，已绑定
	 */
	public native int isBindCer(String encCer);

	/**
	 * 获得号段证书的绑定状态
	 *
	 * @param section
	 *            号段
	 * @return 0，没有绑定； 1，绑定生成； 2，绑定识别； 3，全部绑定；
	 */
	public native int checkSectionsBindState(String section);

	/**
	 * 生成Wave 文件
	 *
	 * @param section
	 *            号段名称
	 * @param data
	 *            阙音码内容
	 * @return wave 流
	 */
	public native short[] genrWave(String section, String data);

	/**
	 * 开始解调
	 *
	 * @param cfg
	 *            RecogCfg 参数
	 * @return 0，成功； 非0，错误码
	 */
	public native int startRecog(RecogCfg cfg);

	/**
	 * 暂停解调
	 *
	 * @return 0，成功； 非0，错误码
	 */
	public native int pauseRecog();

	/**
	 * 恢复解调
	 *
	 * @return
	 */
	public native int resumeRecog();

	/**
	 * 停止解调
	 *
	 * @return 0，成功； 非0，错误码
	 */
	public native int stopRecog();

	/**
	 * 向缓冲区写入数据
	 *
	 * @param waves
	 *            音频数据
	 * @return 0，成功； 非0，错误码
	 */
	public native int writeRecog(short[] waves);

	/**
	 * 设置参数
	 *
	 * @param cfg
	 * @return 0，成功； 非0，错误码
	 */
	public native int setRecogConfig(RecogCfg cfg);

	/**
	 * 获取设置参数
	 *
	 * @return Recog_cfg
	 */
	public native RecogCfg getRecogConfig();

	/**
	 * 获取解调状态
	 *
	 * @return Recog_status
	 */
	public native RecogStatus getRecogStatus();

	/**
	 * 回调函数
	 *
	 * @param section
	 *            码段
	 * @param data
	 *            阙音码内容
	 * @param duration
	 *            解析时间
	 */
	public void recogResult(String section, String data, int duration) {

		SNBean snBean = new SNBean(data.substring(0,4),data.substring(4),duration,System.currentTimeMillis());
		Demodulator.success(snBean);

		Log.e("[APP]:", "证书类型  = " + section);
		Log.e("[APP]:", "消息编号  = " + data);
		Log.e("[APP]:", "花费时间  = " + duration);
//
//		// updata
//		Log.e("[APP]:", "读取声码通信算法状态");
//		RecogStatus status = getRecogStatus();
//		Log.e("[APP]:", "缓 冲    区 = " + status.blankBuffer);
//		Log.e("[APP]:", "相  似   度 = " + status.ss);
//		Log.e("[APP]:", "线程状态 = " + status.recogStat);
	}

	// !加载tlib库函数
	static {
		try {
			Log.e("[APP]:", "加载解析库函数成功success");
			System.loadLibrary("tlib");
		} catch (Exception e) {
			Log.e("system.loadlibrary:", e.getMessage());
		}

	}
}
