package net.telesing.scomm.android;

import android.content.Context;
import net.telesing.scomm.android.core.DemImplement;

import java.util.List;

/**
 * Created by FengJZ on 2015/4/20.
 */
public class DemInterface {

    public static final String HAVE_SN_INTENT = "android.intent.action.net.telesing.scomm.HAVE_SN_INTENT";
    public static final String HAVE_SN_STRING = "android.intent.action.net.telesing.scomm.HAVE_SN_STRING";
    public static final String HAVE_MSGS_INTENT = "android.intent.action.net.telesing.scomm.HAVE_MSGS_INTENT";
    public static final String HAVE_MSGS_STRING = "android.intent.action.net.telesing.scomm.HAVE_MSGS_STRING";
    public static final String FAULT_WEB_INTENT = "android.intent.action.net.telesing.scomm.FAULT_WEB_INTENT";
    public static final String FAULT_MIC_INTENT = "android.intent.action.net.telesing.scomm.FAULT_MIC_INTENT";

    public static final int CLOSED = 11;//已关闭
    public static final int INITIALLING = 12;//初始化中
    public static final int RELEASING = 21;//释放中
    public static final int OPENED = 22;//已打开
    public static final int STARTING = 23;//启动中
    public static final int STOPPING = 32;//停止中
    public static final int RUNNING = 33;//运行中

    public static final int INITIALLING_FAILE = 13;//初始化失败



    public static int state() {
        return DemImplement.state();
    }

    public static boolean close(Context context) {
        return DemImplement.close(context);
    }

    public static boolean open(Context context) {
        return DemImplement.open(context);
    }

    public static boolean run(Context context) {
        return DemImplement.run(context);
    }

    public static List<Msg> parseMsgs(String msgsJSON) {
        return DemImplement.parseMsgs(msgsJSON);
    }

}
