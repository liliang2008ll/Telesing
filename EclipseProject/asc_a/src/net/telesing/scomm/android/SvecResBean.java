package net.telesing.scomm.android;

import java.io.Serializable;


public class SvecResBean implements Serializable {

    private int id;
    private String dates;
    private int resTag;
    private String filePath;
    private String wavPath;
    private String totalTime;
    private String result;

    private long startTime;
    private long endTime;
    private String beaconSn;
    private boolean good;

    private Double syncRate;
    private double dataRates[];
    private boolean dataBites_one[];
    private boolean dataBites_two[];

    private short regSignal[];
    private String msgs;

    private long time ;

    private int bg_res;

    private String msgRes;

    private MsgSn msgSn;

    private boolean have;

    private String orgName;

    public String getOrgName() {
        return orgName;
    }

    public void setOrgName(String orgName) {
        this.orgName = orgName;
    }

    public boolean isHave() {
        return have;
    }

    public void setHave(boolean have) {
        this.have = have;
    }

    public String getMsgRes() {
        return msgRes;
    }

    public void setMsgRes(String msgRes) {
        this.msgRes = msgRes;
    }

    public MsgSn getMsgSn() {
        return msgSn;
    }

    public int getBg_res() {
        return bg_res;
    }

    public void setBg_res(int bg_res) {
        this.bg_res = bg_res;
    }

    public void setMsgSn(MsgSn msgSn) {
        this.msgSn = msgSn;
    }

    public long getTime() {
        return this.endTime - this.startTime;
    }

    public void setTime(long time) {
        this.time = time;
    }

    public short[] getRegSignal() {
        return regSignal;
    }

    public void setRegSignal(short[] regSignal) {
        this.regSignal = regSignal;
    }

    public Double getSyncRate() {
        return syncRate;
    }

    public void setSyncRate(Double syncRate) {
        this.syncRate = syncRate;
    }

    public double[] getDataRates() {
        return dataRates;
    }

    public void setDataRates(double[] dataRates) {
        this.dataRates = dataRates;
    }

    public boolean[] getDataBites_one() {
        return dataBites_one;
    }

    public void setDataBites_one(boolean[] dataBites_one) {
        this.dataBites_one = dataBites_one;
    }

    public boolean[] getDataBites_two() {
        return dataBites_two;
    }

    public void setDataBites_two(boolean[] dataBites_two) {
        this.dataBites_two = dataBites_two;
    }

    public boolean isGood() {
        return good;
    }

    public void setGood(boolean good) {
        this.good = good;
    }

    public String getBeaconSn() {
        return beaconSn;
    }

    public void setBeaconSn(String beaconSn) {
        this.beaconSn = beaconSn;
    }

    public long getStartTime() {
        return startTime;
    }

    public void setStartTime(long startTime) {
        this.startTime = startTime;
    }

    public long getEndTime() {
        return endTime;
    }

    public void setEndTime(long endTime) {
        this.endTime = endTime;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getDates() {
        return dates;
    }

    public void setDates(String dates) {
        this.dates = dates;
    }

    public int getResTag() {
        return resTag;
    }

    public void setResTag(int resTag) {
        this.resTag = resTag;
    }

    public String getResult() {
        return result;
    }

    public void setResult(String result) {
        this.result = result;
    }

    public String getFilePath() {
        return filePath;
    }

    public void setFilePath(String filePath) {
        this.filePath = filePath;
    }

    public String getWavPath() {
        return wavPath;
    }

    public void setWavPath(String wavPath) {
        this.wavPath = wavPath;
    }

    public String getTotalTime() {
        return totalTime;
    }

    public void setTotalTime(String totalTime) {
        this.totalTime = totalTime;
    }

    public String getMsgs() {
        return msgs;
    }

    public void setMsgs(String msgs) {
        this.msgs = msgs;
    }
}
