/*******************************************************************
 *  Copyright(c) 2015-2017 Nanjing telesing
 *  All rights reserved.
 *
 *  �ļ�����:	Recog_status.java
 *  ��Ҫ����:	Recog_status �����㷨������״̬����
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

public class RecogStatus implements Serializable {
	
	private static final long serialVersionUID = 1L;
	
	public double 		ss;				//���ƶ�
	public double 	    blankBuffer;	//�������հ������С
	public int			recogStat ;	//ͨ���㷨״̬,���л���ֹͣ
	
	
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
