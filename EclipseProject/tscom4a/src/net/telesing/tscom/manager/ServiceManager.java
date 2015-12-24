package net.telesing.tscom.manager;

import android.content.Context;
import android.content.Intent;
import android.util.Log;
import net.telesing.tscom.SvecApplication;
import net.telesing.tscom.receiver.SeniorReceiver;
import net.telesing.tscom.service.FloatWindowService;
import net.telesing.tsdk.tlib.core.AcommsInterface;

/**
 * Created by Administrator on 2015/9/24.
 */
public class ServiceManager {

    /**
     * 开启悬浮窗服务
     * @param context
     */
    public static void startFloatService(Context context){
        Intent intent = new Intent(context, FloatWindowService.class);
        intent.putExtra("win_open", 2);
        context.startService(intent);

    }

    public static void sendSeniorReceiver(Context context,int type){
        Intent intent = new Intent(SeniorReceiver.SENIORRECEIVER_ACTION);
        intent.putExtra("senior_type", type);
        context.sendBroadcast(intent);
    }

    /**
     * 关闭悬浮窗服务
     * @param context
     */
    public static void stopFloatService(Context context){
        Intent intent = new Intent(context, FloatWindowService.class);
        context.stopService(intent);

    }


    public static int startVoice(Context context){
        SvecApplication.getInstance().setStartService(true);
        int isSuc = AcommsInterface.run(context);
        sendSeniorReceiver(context, 3);
        return isSuc;
    }
    public static void closeVoice(Context context){
        SvecApplication.getInstance().setStartService(false);
        AcommsInterface.close(context);
        sendSeniorReceiver(context,4);
    }

}
