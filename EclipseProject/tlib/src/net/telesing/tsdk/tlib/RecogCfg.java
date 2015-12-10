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
	
	public double[] start_feq;	//启始频率
	public double[] end_feq;	//截止频率
	public int 		gap   ;		//跳转间隙
	public double 	st ;		//相似度
	public int 		fft_points; //FFT点数
	public int 		fft_chains;	//FFT信道数
	public int 		fft_gap;	//同步失败后跳转间隙
	
//set,get方法
	public double[] getStart_feq() {
		return start_feq;
	}
	public static long getSerialversionuid() {
		return serialVersionUID;
	}
	public void setStart_feq(double[] start_feq) {
		this.start_feq = start_feq;
	}
	public double[] getEnd_feq() {
		return end_feq;
	}
	public void setEnd_feq(double[] end_feq) {
		this.end_feq = end_feq;
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
	public int getFft_points() {
		return fft_points;
	}
	public void setFft_points(int fft_points) {
		this.fft_points = fft_points;
	}
	public int getFft_chains() {
		return fft_chains;
	}
	public void setFft_chains(int fft_chains) {
		this.fft_chains = fft_chains;
	}
	public int getFft_gap() {
		return fft_gap;
	}
	public void setFft_gap(int fft_gap) {
		this.fft_gap = fft_gap;
	}
	
	
	
	

}
