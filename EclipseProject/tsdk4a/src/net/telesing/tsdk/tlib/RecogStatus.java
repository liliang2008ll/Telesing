/*******************************************************************
 *  Copyright(c) 2015-2017 Nanjing telesing
 *  All rights reserved.
 *
 *  文件名称:	Recog_status.java
 *  简要描述:	Recog_status 核心算法库运行状态参数
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

import java.io.Serializable;

public class RecogStatus implements Serializable {

	private static final long serialVersionUID = 1L;

	public double ss; // 相似度
	public double blankBuffer; // 缓冲区空白区域大小
	public int recogStat; // 通信算法状态,运行还是停止

	public double getSs() {
		return ss;
	}

	public void setSs(double ss) {
		this.ss = ss;
	}

	public double getBlank_buffer() {
		return blankBuffer;
	}

	public void setBlank_buffer(int blank_buffer) {
		this.blankBuffer = blank_buffer;
	}

	public int getRecog_stat() {
		return recogStat;
	}

	public void setRecog_stat(int recog_stat) {
		this.recogStat = recog_stat;
	}




}
