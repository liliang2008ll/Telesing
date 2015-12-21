package net.telesing.acs.receiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

/**
 *  接收到声码绑定的消息
 */
public class SeniorReceiver extends BroadcastReceiver {

    public static  final String SENIORRECEIVER_ACTION = "net.telesing.acs.receiver.SeniorReceiver";

    @Override
    public void onReceive(Context context, Intent intent) {
        doWhat(intent.getIntExtra("senior_type",9));
    }
    protected  void doWhat(int type){}
}
