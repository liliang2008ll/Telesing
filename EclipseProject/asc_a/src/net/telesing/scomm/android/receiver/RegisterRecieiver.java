package net.telesing.scomm.android.receiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

/**
 * Created by Administrator on 2015/11/3.
 */
public class RegisterRecieiver extends BroadcastReceiver {
    public static final String ACTION = "net.telesing.scomm.android.receiver.BroadcastReceiver";
    public static final String STRING_EXTRA = "CER_CODE";
    @Override
    public void onReceive(Context context, Intent intent) {

        doWhat(intent.getPackage(), intent.getStringExtra(STRING_EXTRA));
    }
    protected void doWhat(String packageName, String asc) {
        Log.e("dowhat",packageName+","+asc);
    }
}
