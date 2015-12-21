package net.telesing.acs.receiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import com.google.gson.Gson;
import net.telesing.scomm.android.SvecResBean;

/**
 * 接收到声码
 */
public class SNReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        SvecResBean svecResBean = new Gson().fromJson(intent.getStringExtra("svec_bean"),SvecResBean.class);
        svecResBean.setOrgName(intent.getStringExtra("org_name"));
        doWhat(svecResBean);
    }
    protected  void doWhat(SvecResBean svecResBean){}
}
