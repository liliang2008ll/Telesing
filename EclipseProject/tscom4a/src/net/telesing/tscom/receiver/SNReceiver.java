package net.telesing.tscom.receiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import com.google.gson.Gson;
import net.telesing.tsdk.tlib.bean.SNBean;

/**
 * 接收到声码
 */
public class SNReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        SNBean snBean = new Gson().fromJson(intent.getStringExtra("svec_bean"),SNBean.class);
        if(snBean != null){
            snBean.setOrgName(intent.getStringExtra("org_name"));
        }
        doWhat(snBean);
    }
    protected  void doWhat(SNBean snBean){}
}
