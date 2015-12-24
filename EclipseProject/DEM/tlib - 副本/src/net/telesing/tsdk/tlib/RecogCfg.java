/*******************************************************************
 *  Copyright(c) 2015-2017 Nanjing telesing
 *  All rights reserved.
 *
 *  文件名称:	Recog_cfg.java
 *  简要描述:	Recog_cfg 核心算法库配置参数
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

import android.R.integer;

public class RecogCfg implements Serializable {
	
	private static final long serialVersionUID = -5324620371099616623L;
	
	public int[] startFeq;	//启始频率
	public int[] endFeq;	//截止频率
	public int 		gap   ;		//跳转间隙
	public double 	st ;		//相似度
	public int 		fftPoints; //FFT点数
	public int 		fftChains;	//FFT信道数
	public int      bufferLen;
	public int[] getStartFeq() {
		return startFeq;
	}
	public void setStartFeq(int[] startFeq) {
		this.startFeq = startFeq;
	}
	public int[] getEndFeq() {
		return endFeq;
	}
	public void setEndFeq(int[] endFeq) {
		this.endFeq = endFeq;
	}
	public int getGap() {
		return gap;
	}
	public void setGap(int gap) {
		this.gap = gap;
	}
	public double getSt() {
		return st;
	}
	public void setSt(double st) {
		this.st = st;
	}
	public int getFftPoints() {
		return fftPoints;
	}
	public void setFftPoints(int fftPoints) {
		this.fftPoints = fftPoints;
	}
	public int getFftChains() {
		return fftChains;
	}
	public void setFftChains(int fftChains) {
		this.fftChains = fftChains;
	}
	public int getBufferLen() {
		return bufferLen;
	}
	public void setBufferLen(int bufferLen) {
		this.bufferLen = bufferLen;
	}
	


	
	
	
	

}
