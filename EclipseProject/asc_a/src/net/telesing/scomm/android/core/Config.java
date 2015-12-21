package net.telesing.scomm.android.core;

/**
 * Created by FengJZ on 2015/5/29.
 */
public class Config {

    public static final String QUERY_MSG_BY_SN_URL = "http://test.tele-sing.com/msg/qMsgBySNs.do";
    public static final int CACHE_CLEAR_MS = 1000 * 60 * 5 / 1000;//5 min
    public static final int DEM_BUFFER_SIZE = 44100 * 10;
    public static final int DEM_N = 2048;
    public static final double DEM_ST = 0.15;
    public static final int DEM_GAP = 512;
//    public static final double[] DEM_SFEQ = {16, 12, 8, 4};
//    public static final double[] DEM_EFEQ = {20, 16, 12, 8};
//    public static final int DEM_FEQ_SIZE = DEM_SFEQ.length < DEM_EFEQ.length ? DEM_SFEQ.length : DEM_EFEQ.length;

}
