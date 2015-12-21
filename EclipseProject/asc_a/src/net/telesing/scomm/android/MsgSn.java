package net.telesing.scomm.android;

import java.io.Serializable;

/**
 * Created by Administrator on 2015/6/11.
 */
public class MsgSn implements Serializable {
    private String sn;
    private Long beginTime;
    private Long endTime;
    private String msg;
    private Long time;

    public Long getTime() {
        return this.endTime - beginTime;
    }

    public void setTime(Long time) {
        this.time = time;
    }

    public String getSn() {
        return sn;
    }

    public void setSn(String sn) {
        this.sn = sn;
    }

    public Long getBeginTime() {
        return beginTime;
    }

    public void setBeginTime(Long beginTime) {
        this.beginTime = beginTime;
    }

    public Long getEndTime() {
        return endTime;
    }

    public void setEndTime(Long endTime) {
        this.endTime = endTime;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }


}
