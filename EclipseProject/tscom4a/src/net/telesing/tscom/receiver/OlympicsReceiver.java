package net.telesing.tscom.receiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import net.telesing.tscom.service.FloatWindowService;

public class OlympicsReceiver extends BroadcastReceiver
{

    public static final String ACTION = "android.intent.action.BOOT_COMPLETED";
    public static final String ACTION_1 = "net.telesing.acs.receiver.OlympicsReceiver";

    public void onReceive(Context context, Intent intent)
    {    
        if (intent.getAction().equals(ACTION) || intent.getAction().equals(ACTION_1)){
            context.startService(new Intent(context, FloatWindowService.class));
        }    
    }    
 
}   
