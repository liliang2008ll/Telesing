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
public class CerInfor implements Serializable {

	/**
	 *
	 */
	private static final long serialVersionUID = 8732543735745997438L;

	// 有效字段
	public String section; // 码段
	public String expiration; // 失效日期
	public String authority; // 证书权限

	// 附加字段
	public String ID; // 证书ID
	public String orgID; // 证书编号
	public String genrTime; // 时间
	public String genrType; // 生成方式 --人工或则程序生成?
	public String genrIp; // 生成ID
	public String sectSrc; // 证书来源

	// 调试信息
	public String voidString; // 占位符号

	public int cerLen; // 字节长度
	// 随机附加数据
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
