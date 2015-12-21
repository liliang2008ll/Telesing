package net.telesing.tsdk.tlib;

import java.io.Serializable;

public class Recog_status implements Serializable {
	
	private static final long serialVersionUID = 1L;
	public double ss;//相似度
	public int blank_buffer;//缓冲区空白区域大小
	public int recog_stat ;//通信算法状态
	public double getSs() {
		return ss;
	}
	public void setSs(double ss) {
		this.ss = ss;
	}
	public int getBlank_buffer() {
		return blank_buffer;
	}
	public void setBlank_buffer(int blank_buffer) {
		this.blank_buffer = blank_buffer;
	}
	public int getRecog_stat() {
		return recog_stat;
	}
	public void setRecog_stat(int recog_stat) {
		this.recog_stat = recog_stat;
	}
	
	

}
