package net.telesing.tsdk.tlib.bean;

/**
 * Created by Administrator on 2015/12/7.
 */
public class SNBean {
    private String section;
    private String data;
    private int times;
    private long rec_time;
    private String res;
    private boolean isHave;

    private String orgName;
    private int color;
    private String sn;

    private String result;
    private boolean isGood;

    public boolean isGood() {
        return isGood;
    }

    public void setGood(boolean good) {
        isGood = good;
    }

    public String getResult() {
        return result;
    }

    public void setResult(String result) {
        this.result = result;
    }

    public String getSn() {
        return this.getSection()+this.getData();
    }

    public void setSn(String sn) {
        this.sn = sn;
    }

    public String getOrgName() {
        return orgName;
    }

    public void setOrgName(String orgName) {
        this.orgName = orgName;
    }

    public int getColor() {
        return color;
    }

    public void setColor(int color) {
        this.color = color;
    }

    public SNBean() {
    }

    public SNBean(String section, String data, int times, long rec_time) {
        this.section = section;
        this.data = data;
        if(data == null || data.length() ==0){
            this.data = "";
            setGood(false);
        }else {
            setGood(true);
        }

        this.times = times;
        this.rec_time = rec_time;
    }

    public String getSection() {
        return section;
    }

    public void setSection(String section) {
        this.section = section;
    }

    public String getData() {
        return data;
    }

    public void setData(String data) {
        this.data = data;
    }

    public int getTimes() {
        return times;
    }

    public void setTimes(int times) {
        this.times = times;
    }

    public long getRec_time() {
        return rec_time;
    }

    public void setRec_time(long rec_time) {
        this.rec_time = rec_time;
    }

    public String getRes() {
        return res;
    }

    public void setRes(String res) {
        this.res = res;
    }

    public boolean isHave() {
        return isHave;
    }

    public void setHave(boolean have) {
        isHave = have;
    }
}
