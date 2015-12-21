package net.telesing.acs.receiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import net.telesing.scomm.android.DemInterface;
import net.telesing.scomm.android.MsgSn;
import net.telesing.scomm.android.SvecResBean;

/**
 *  接收到声码绑定的消息
 */
public class MsgReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        doWhat((MsgSn) intent.getSerializableExtra(DemInterface.HAVE_MSGS_STRING));
        doWhatSvec((SvecResBean)intent.getSerializableExtra("svec_bean"));
    }

    protected  void doWhat(MsgSn msgSn){}

    protected void doWhatSvec(SvecResBean svecResBean){}
}
