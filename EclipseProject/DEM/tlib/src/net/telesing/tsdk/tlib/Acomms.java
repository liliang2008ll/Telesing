/*******************************************************************
 *  Copyright(c) 2015-2017 Nanjing telesing
 *  All rights reserved.
 *
 *  �ļ�����:	Acomms.java
 *  ��Ҫ����:	acomms �����㷨Java����
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

import org.w3c.dom.Text;

import android.R.integer;
import android.R.string;
import android.util.Log;

import net.telesing.tsdk.tlib.RecogStatus;
import net.telesing.tsdk.tlib.RecogCfg;
import net.telesing.tsdk.tlib.CerInfor;

//!����ͨ���㷨�ӿ�
public class Acomms {

	// !���������֤��ṹ�����
	public native String encryptCer(CerInfor cer); // ok

	/**
	 * ����֤��
	 * 
	 * @param encCer
	 *            ����֤��
	 * @param cerLen
	 *            ����֤�鳤��
	 * @return
	 */
	public native CerInfor decryptCer(String encCer);

	/**
	 * ��֤��
	 * 
	 * @param encCer
	 *            ֤������
	 * @param cerLen
	 *            ֤�鳤��
	 * @return 0���ɹ��� ��0��������
	 */
	public native int bindCer(String encCer);

	/**
	 * ���֤��
	 * 
	 * @param encCer
	 *            ����֤��
	 * @param cerLen
	 *            ����֤�鳤��
	 * @return 0���ɹ��� ��0��������
	 */
	public native int unbindCer(String encCer);

	/**
	 * ��֤֤���Ƿ��
	 * 
	 * @param encCer
	 *            ����֤��
	 * @param cerLen
	 *            ����֤�鳤��
	 * @return 0��δ�󶨣� 1���Ѱ�
	 */
	public native int isBindCer(String encCer);

	/**
	 * ��úŶ�֤��İ�״̬
	 * 
	 * @param section
	 *            �Ŷ�
	 * @return 0��û�а󶨣� 1�������ɣ� 2����ʶ�� 3��ȫ���󶨣�
	 */
	public native int checkSectionsBindState(String section);

	/**
	 * ����Wave �ļ�
	 * 
	 * @param section
	 *            �Ŷ�����
	 * @param data
	 *            ����������
	 * @return wave ��
	 */
	public native short[] genrWave(String section, String data);

	/**
	 * ��ʼ���
	 * 
	 * @param cfg
	 *            RecogCfg ����
	 * @return 0���ɹ��� ��0��������
	 */
	public native int startRecog(RecogCfg cfg);

	/**
	 * ��ͣ���
	 * 
	 * @return 0���ɹ��� ��0��������
	 */
	public native int pauseRecog();

	/**
	 * �ָ����
	 * 
	 * @return
	 */
	public native int resumeRecog();

	/**
	 * ֹͣ���
	 * 
	 * @return 0���ɹ��� ��0��������
	 */
	public native int stopRecog();

	/**
	 * �򻺳���д������
	 * 
	 * @param waves
	 *            ��Ƶ����
	 * @param waveLen
	 *            ���ݳ���
	 * @return 0���ɹ��� ��0��������
	 */
	public native int writeRecog(short[] waves);

	/**
	 * ���ò���
	 * 
	 * @param cfg
	 * @return 0���ɹ��� ��0��������
	 */
	public native int setRecogConfig(RecogCfg cfg);

	/**
	 * ��ȡ���ò���
	 * 
	 * @return Recog_cfg
	 */
	public native RecogCfg getRecogConfig();

	/**
	 * ��ȡ���״̬
	 * 
	 * @return Recog_status
	 */
	public native RecogStatus getRecogStatus();

	/**
	 * �ص�����
	 * 
	 * @param section
	 *            ���
	 * @param data
	 *            ����������
	 * @param times
	 *            ����ʱ��
	 */
	public void recogResult(String section, String data, int duration) {

		Log.e("[APP]:", "֤������  = " + section);
		Log.e("[APP]:", "��Ϣ���  = " + data);
		Log.e("[APP]:", "����ʱ��  = " + duration);

		// updata
		Log.e("[APP]:", "��ȡ����ͨ���㷨״̬");
		RecogStatus status = getRecogStatus();
		Log.e("[APP]:", "�� ��    �� = " + status.blankBuffer);
		Log.e("[APP]:", "��  ��   �� = " + status.ss);
		Log.e("[APP]:", "�߳�״̬ = " + status.recogStat);
	}

	// !����tlib�⺯��
	static {
		try {
			Log.e("[APP]:", "���ؽ����⺯���ɹ�success");
			System.loadLibrary("tlib");
		} catch (Exception e) {
			Log.e("system.loadlibrary:", e.getMessage());
		}

	}
}