package net.telesing.acs.bean;

import java.io.Serializable;


public class SNBean implements Serializable {
    private long id;
    private String sn;

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public String getSn() {
        return sn;
    }

    public void setSn(String sn) {
        this.sn = sn;
    }
}