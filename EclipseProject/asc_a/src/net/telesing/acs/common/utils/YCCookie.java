package net.telesing.acs.common.utils;


import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;


public class YCCookie {

    public static final String COOKIENAME = "Svec_qyin_Cookie";


    public static final String IS_FIRST = "is_first";
    public static final String SYNCSKIP = "sync_skip";
    public static final String PARSETIMES = "parse_times";
    public static final String PARSESKIP = "parse_skip";
    public static final String FASTMODE = "fast_mode";
    public static final String SIGNLTHRESHOLD = "signal_threshold";
    public static final String YBNUM = "yb_num";
    public static final String SFEQ = "s_feq";
    public static final String EFEQ = "e_feq";


    public static final String OPEN_RUN = "open_run";//开机启动
    public static final String CHECK_OPEN = "check_open";//检测开关
    public static final String WIN_OPEN = "win_open";//悬浮窗
    public static final String DESTTOP_OPEN = "desttop_open";//仅桌面

    private SharedPreferences sp;

    public YCCookie(SharedPreferences sp) {
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

    public boolean getBoolean(String key) {
        boolean value = sp.getBoolean(key, true);
        return value;
    }

    public Integer getInteger(String key) {
        Integer value = sp.getInt(key, -1);
        return value;
    }


    public SharedPreferences getSp() {
        return sp;
    }

    public void setSp(SharedPreferences sp) {
        this.sp = sp;
    }
}
