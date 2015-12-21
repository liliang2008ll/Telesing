package net.telesing.scomm.android;


import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;


public class Cookie {

    public static final String COOKIENAME = "SCALIB_qyin_Cookie";

    public static String QUERY_MSG_BY_SN_URL = "query_msg_by_sn_url";
    public static String BY_QUERY_MSG_BY_SN_URL = "by_query_msg_by_sn_url";
    public static String CACHE_CLEAR_MS = "cache_clear_ms";
    public static String DEM_N = "dem_n";
    public static String DEM_ST = "dem_st";
    public static String DEM_GAP = "dem_gap";

    public static String DEM_SFEQ = "dem_sfeq";
    public static String DEM_EFEQ = "dem_efeq";
    private SharedPreferences sp;
    public Cookie(SharedPreferences sp) {
        this.sp = sp;
    }
    public boolean putString(String key, String value) {
        // 添加数据
        Editor editor = sp.edit();
        editor.putString(key, value);
        return editor.commit();
    }

    public boolean putBoolean(String key, boolean value) {
        // 添加数据
        Editor editor = sp.edit();
        editor.putBoolean(key, value);
        return editor.commit();
    }

    public boolean putInteger(String key, int value) {
        // 添加数据
        Editor editor = sp.edit();
        editor.putInt(key, value);
        return editor.commit();
    }

    public String getString(String key) {
        String value = sp.getString(key, "");
        return value;
    }

    public String getBfString(String key) {
        String value = sp.getString(key, "http://test.tele-sing.com");
        return value;
    }

    public String getZhuFString(String key) {
        String value = sp.getString(key, "http://test.tele-sing.com");
        return value;
    }

    public String getByUrlString(String key) {
        String value = sp.getString(key, "http://test.tele-sing.com");
        return value+"/msg/qMsgBySNs.do";
    }

    public String getUrlString(String key) {
        String value = sp.getString(key, "http://test.tele-sing.com");
        return value+"/msg/qMsgBySNs.do";
    }

    public boolean getBoolean(String key) {
        boolean value = sp.getBoolean(key, true);
        return value;
    }

    public Integer getInteger(String key) {
        Integer value = sp.getInt(key, -1);
        return value;
    }
    public Integer getNumInteger(String key) {
        Integer value = sp.getInt(key, 2048);
        return value;
    }
    public Integer getGapInteger(String key) {
        Integer value = sp.getInt(key, 512);
        return value;
    }

    public Long getCacheTime(String key){
        String value = sp.getString(key, "1");
        return (long)Double.parseDouble(value)*1000*60;
    }

    public String getTime(String key){
        String value = sp.getString(key, "1");
       return value;
    }



    public Double getStDouble(String key){
        String value = sp.getString(key, "0.15");
        return Double.parseDouble(value);
    }

    public double[] getfeq(String key,double defaultEq){
        String val = sp.getString(key,String.valueOf(defaultEq));
        return new double[]{Double.parseDouble(val)};
    }


    public SharedPreferences getSp() {
        return sp;
    }

    public void setSp(SharedPreferences sp) {
        this.sp = sp;
    }
}
