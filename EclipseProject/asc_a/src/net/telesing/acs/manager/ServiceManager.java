package net.telesing.acs.manager;

import android.content.Context;
import android.content.Intent;
import android.util.Log;
import net.telesing.acs.SvecApplication;
import net.telesing.acs.receiver.SeniorReceiver;
import net.telesing.acs.service.FloatWindowService;
import net.telesing.scomm.android.DemInterface;
import net.telesing.scomm.android.receiver.RegisterRecieiver;

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

    public static void sendRegisterReceiver(Context context,String packageName,String section){
        Intent intent = new Intent(RegisterRecieiver.ACTION);
        intent.setPackage(packageName);
        intent.putExtra(RegisterRecieiver.STRING_EXTRA, section);
        context.sendBroadcast(intent);
        Log.e("==sender=>",context.toString()+"."+packageName+","+section);
    }

    public static boolean startVoice(Context context){
        SvecApplication.getInstance().setStartService(true);
        boolean isSuc = DemInterface.run(context);
        sendSeniorReceiver(context, 3);
        return isSuc;
    }
    public static void closeVoice(Context context){
        SvecApplication.getInstance().setStartService(false);
        DemInterface.close(context);
        sendSeniorReceiver(context,4);
    }

}
