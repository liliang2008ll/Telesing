package net.telesing.tsdk.tlib.core;

import android.content.Context;
import net.telesing.tsdk.tlib.RecogCfg;
import net.telesing.tsdk.tlib.RecogStatus;
import net.telesing.tsdk.tlib.core.impl.AcommsImpl;

/**
 * Created by Administrator on 2015/12/2.
 */
public class AcommsInterface {


    public static final String HAVE_SN_INTENT = "android.intent.action.net.telesing.tsdk.HAVE_SN_INTENT";
    public static final String HAVE_SN_STRING = "android.intent.action.net.telesing.tsdk.HAVE_SN_STRING";
    public static final String HAVE_MSGS_INTENT = "android.intent.action.net.telesing.tsdk.HAVE_MSGS_INTENT";
    public static final String HAVE_MSGS_STRING = "android.intent.action.net.telesing.tsdk.HAVE_MSGS_STRING";

    public static final int REGISTERED=43;//已注册
    public static final int REGISTERING = 42;//注册中
    public static final int UNREGISTER = 41;//未注册
    public static final int CLOSED = 11;//已关闭
    public static final int INITIALLING = 12;//初始化中
    public static final int RELEASING = 21;//释放中
    public static final int OPENED = 22;//已打开
    public static final int STARTING = 23;//启动中
    public static final int STOPPING = 32;//停止中
    public static final int RUNNING = 33;//运行中
    public static final int PAUSEING = 34;//暂停中


    public static final int INITIALLING_FAILE = 13;//初始化失败


    public static int run(Context context){
        return AcommsImpl.run(context);
    }
    public static int state() {
        return AcommsImpl.state();
    }
    public static int close(Context context) {
        return AcommsImpl.close(context);
    }
    public static int open(Context context) {
        return AcommsImpl.open(context);
    }

    public static int bindCer(String packageName,String cer){
        return AcommsImpl.bindCer(packageName,cer);
    }
    public static int unbindCer(String cer){
        return AcommsImpl.unbindCer(cer);
    }
    public static int isbindCer(String cer){
        return AcommsImpl.isbindCer(cer);
    }

    public static int checkSectionsBindState(String section){
        return AcommsImpl.checkSectionsBindState(section);
    }

    public static short[] genrWave(String section,String data){
        return AcommsImpl.genrWave(section,data);
    }

    public static int genrWave(String section,String data,String wavePath){
        return AcommsImpl.genrWave(section,data,wavePath);
    }


    public static int setRecogConfig(RecogCfg cfg){
        return AcommsImpl.setRecogConfig(cfg);
    }
    public static RecogCfg getRecogConfig(){
        return AcommsImpl.getRecogConfig();
    }

    public static RecogStatus getRecogStatus(){
        return AcommsImpl.getRecogStatus();
    }
    public static int play(String filePath){
        return AcommsImpl.play(filePath);
    }
    public static int play(String section,String data){
        return AcommsImpl.play(section, data);
    }
    public static double getSignal(){
        return AcommsImpl.getSignal();
    }
    public static String getSection(String enccer){
        return AcommsImpl.getSection(enccer);
    }


    public static String getCert(){
        return AcommsImpl.getCert();
    }
}
