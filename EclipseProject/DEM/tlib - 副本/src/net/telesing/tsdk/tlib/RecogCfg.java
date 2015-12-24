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
	
	public int[] startFeq;	//��ʼƵ��
	public int[] endFeq;	//��ֹƵ��
	public int 		gap   ;		//��ת��϶
	public double 	st ;		//���ƶ�
	public int 		fftPoints; //FFT����
	public int 		fftChains;	//FFT�ŵ���
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
