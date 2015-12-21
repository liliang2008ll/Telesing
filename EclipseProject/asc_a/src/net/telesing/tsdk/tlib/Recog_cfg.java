package net.telesing.tsdk.tlib;

import java.io.Serializable;

public class Recog_cfg implements Serializable {
	private static final long serialVersionUID = -5324620371099616623L;
	public int start_feq;//启始频率
	public int end_feq;//截止频率
	public int gap   ;//跳转间隙
	public double minss ;//相似度
	public int getStart_feq() {
		return start_feq;
	}
	public void setStart_feq(int start_feq) {
		this.start_feq = start_feq;
	}
	public int getEnd_feq() {
		return end_feq;
	}
	public void setEnd_feq(int end_feq) {
		this.end_feq = end_feq;
	}
	public int getGap() {
		return gap;
	}
	public void setGap(int gap) {
		this.gap = gap;
	}
	public double getMinss() {
		return minss;
	}
	public void setMinss(double minss) {
		this.minss = minss;
	}
	
	
	

}
