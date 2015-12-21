package net.telesing.acs.receiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Message;
import net.telesing.acs.SvecApplication;

/**
 * Created by Administrator on 2015/11/9.
 */
public class NotificationReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        Message msg = new Message();
        msg.what = 11100000;
        msg.obj = intent.getAction();
       SvecApplication.getInstance().getmHandler().sendMessage(msg);
    }




}
