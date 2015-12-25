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

public class CerInfor implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 8732543735745997438L;

	// ��Ч�ֶ�
	public String section; // ���
	public String expiration; // ʧЧ����
	public String authority; // ֤��Ȩ��

	// �����ֶ�
	public String ID; // ֤��ID
	public String orgID; // ֤����
	public String genrTime; // ʱ��
	public String genrType; // ���ɷ�ʽ --�˹������������?
	public String genrIp; // ����ID
	public String sectSrc; // ֤����Դ

	// ������Ϣ
	public String voidString; // ռλ����

	public int cerLen; // �ֽڳ���
	// �����������
	private String appendData;

	public String getSection() {
		return section;
	}

	public void setSection(String section) {
		this.section = section;
	}

	public String getExpiration() {
		return expiration;
	}

	public void setExpiration(String expiration) {
		this.expiration = expiration;
	}

	public String getAuthority() {
		return authority;
	}

	public void setAuthority(String authority) {
		this.authority = authority;
	}

	public String getAppendData() {
		return appendData;
	}

	public void setAppendData(String appendData) {
		this.appendData = appendData;
	}

	public String getID() {
		return ID;
	}

	public void setID(String iD) {
		ID = iD;
	}

	public String getOrgID() {
		return orgID;
	}

	public void setOrgID(String orgID) {
		this.orgID = orgID;
	}

	public String getSectSrc() {
		return sectSrc;
	}

	public void setSectSrc(String sectSrc) {
		this.sectSrc = sectSrc;
	}

	public String getGenrTime() {
		return genrTime;
	}

	public void setGenrTime(String genrTime) {
		this.genrTime = genrTime;
	}

	public String getGenrType() {
		return genrType;
	}

	public void setGenrType(String genrType) {
		this.genrType = genrType;
	}

	public String getGenrIp() {
		return genrIp;
	}

	public void setGenrIp(String genrIp) {
		this.genrIp = genrIp;
	}

	public int getCerLen() {
		return cerLen;
	}

	public void setCerLen(int cerLen) {
		this.cerLen = cerLen;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}

}