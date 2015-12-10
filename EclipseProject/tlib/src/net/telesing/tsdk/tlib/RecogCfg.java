/*******************************************************************
 *  Copyright(c) 2015-2017 Nanjing telesing
 *  All rights reserved.
 *
 *  �ļ�����:	Recog_cfg.java
 *  ��Ҫ����:	Recog_cfg �����㷨�����ò���
 *
 *  ��ǰ�汾:	1.1
 *  ��   	    ��:	Lee.liang
 *  ��         ��:
 *  ˵         ��:
 *
 *  ȡ���汾:	1.0
 *  ��          ��:
 *  ��         ��:
 *  ˵         ��:
 ******************************************************************/
package net.telesing.tsdk.tlib;

import java.io.Serializable;

import android.R.integer;

public class RecogCfg implements Serializable {
	
	private static final long serialVersionUID = -5324620371099616623L;
	
	public double[] start_feq;	//��ʼƵ��
	public double[] end_feq;	//��ֹƵ��
	public int 		gap   ;		//��ת��϶
	public double 	st ;		//���ƶ�
	public int 		fft_points; //FFT����
	public int 		fft_chains;	//FFT�ŵ���
	public int 		fft_gap;	//ͬ��ʧ�ܺ���ת��϶
	
//set,get����
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
