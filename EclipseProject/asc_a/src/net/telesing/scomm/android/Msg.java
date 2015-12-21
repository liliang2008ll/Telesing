package net.telesing.scomm.android;

/**
 * Created by FengJZ on 2015/5/26.
 */
public class Msg {

    private String sn;

    private Long m_id;

    private Long c_id;

    private Long m_remind_interval;


    public String getSn() {
        return sn;
    }

    public void setSn(String sn) {
        this.sn = sn;
    }

    public Long getM_id() {
        return m_id;
    }

    public void setM_id(Long m_id) {
        this.m_id = m_id;
    }

    public Long getC_id() {
        return c_id;
    }

    public void setC_id(Long c_id) {
        this.c_id = c_id;
    }

    public Long getM_remind_interval() {
        return m_remind_interval;
    }

    public void setM_remind_interval(Long m_remind_interval) {
        this.m_remind_interval = m_remind_interval;
    }

}
