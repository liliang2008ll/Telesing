package net.telesing.acs;

import android.app.*;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.NotificationCompat;
import android.util.Log;
import android.widget.RemoteViews;
import net.telesing.acs.common.utils.YCCookie;
import net.telesing.acs.manager.FloatWindowManager;
import net.telesing.acs.manager.ServiceManager;
import net.telesing.acs.ui.MainUI;


public class SvecApplication extends Application
{
    public static final String TAG = SvecApplication.class.getSimpleName();
    public static SvecApplication _instance;
    private Activity curActivity;
    private YCCookie ycCookie;
    private SharedPreferences sharedPreferences;
    private boolean isNotification = false;
    private NotificationManager mNotificationManager ;
    private RemoteViews contentView;
    private Notification notification;
    private boolean isStartService = false;
    @Override
    public void onCreate()
    {
        super.onCreate();
        _instance = this;
        sharedPreferences = getApplicationContext().getSharedPreferences(YCCookie.COOKIENAME, Context.MODE_PRIVATE);
        ycCookie = new YCCookie(sharedPreferences);
//        CatchHandler.getInstance().init(getApplicationContext());
    }
    @Override
    public void onTerminate()
    {
        super.onTerminate();
        _instance = null;
    }

    public static SvecApplication getInstance()
    {
        return _instance;
    }

    public void setCurrentActivity(Activity curActivity){
        this.curActivity = curActivity;
    }

    public boolean isNotification() {
        return isNotification;
    }

    public void showNotification(Context context){
        isNotification = true;
        mNotificationManager = (NotificationManager)context.getSystemService(context.NOTIFICATION_SERVICE);
        notification = new Notification();
        notification.icon = R.drawable.ic_launcher;
        notification.tickerText = "阙音";
        notification.when = 0;

        /***
         * 在这里我们用自定的view来显示Notification
         */

        contentView = new RemoteViews(context.getPackageName(), R.layout.notification_layout);
        contentView.setTextViewText(R.id.text11, "检测开关");
        contentView.setTextViewText(R.id.text22, "开机启动");
        contentView.setTextViewText(R.id.text33, "悬浮球开关");
        contentView.setTextViewText(R.id.text44, "仅桌面显示");

        setImage(null,0);

        notification.flags = Notification.FLAG_ONGOING_EVENT; // 设置常驻，不能滑动取消
        notification.priority = Notification.PRIORITY_MAX;
        //默认跳转的主界面
        Intent intent = new Intent(context, MainUI.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, intent,PendingIntent.FLAG_CANCEL_CURRENT);

        //自定义跳转
        contentView.setOnClickPendingIntent(R.id.ll_11, PendingIntent.getBroadcast(context, 11, new Intent().setAction(YCCookie.CHECK_OPEN), PendingIntent.FLAG_UPDATE_CURRENT));
        contentView.setOnClickPendingIntent(R.id.ll_22, PendingIntent.getBroadcast(context, 11, new Intent().setAction(YCCookie.OPEN_RUN), PendingIntent.FLAG_UPDATE_CURRENT));
        contentView.setOnClickPendingIntent(R.id.ll_33, PendingIntent.getBroadcast(context, 11, new Intent().setAction(YCCookie.WIN_OPEN), PendingIntent.FLAG_UPDATE_CURRENT));
        contentView.setOnClickPendingIntent(R.id.ll_44, PendingIntent.getBroadcast(context, 11, new Intent().setAction(YCCookie.DESTTOP_OPEN), PendingIntent.FLAG_UPDATE_CURRENT));
        notification.contentView = contentView;
        notification.contentIntent = pendingIntent;
        mNotificationManager.notify(100, notification);
    }

    public void setImage(String text,int type){
        if(contentView != null){
            contentView.setImageViewResource(R.id.img1,isStartService? R.drawable.sense_switch_open: R.drawable.sense_switch_close);
            contentView.setImageViewResource(R.id.img2,ycCookie.getBoolean(YCCookie.OPEN_RUN)? R.drawable.start_up_open: R.drawable.start_up_close);
            contentView.setImageViewResource(R.id.img3,ycCookie.getBoolean(YCCookie.WIN_OPEN)? R.drawable.ball_open: R.drawable.ball_close);
            contentView.setImageViewResource(R.id.img4,ycCookie.getBoolean(YCCookie.DESTTOP_OPEN)? R.drawable.only_wind_open: R.drawable.only_wind_close);
        }
        if(text != null && contentView != null){
            Log.e("=======>",text);
            contentView.setTextViewText(R.id.text55, text);
        }
        if(type == 1){
            mNotificationManager.notify(100,notification);
        }

    }

    private Handler mHandler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what){
                case  11100000:
                    doWhat(msg.obj.toString());
                    break;
            }
        }
    };

    private void doWhat(String action){
        Log.e("action",action);
        sharedPreferences = getSharedPreferences(YCCookie.COOKIENAME, Context.MODE_PRIVATE);
        ycCookie = new YCCookie(sharedPreferences);
        if(!"chang_view".equals(action)){
            boolean isChecked = ycCookie.getBoolean(action);
            Log.e("action",String.valueOf(isChecked));

            if(ycCookie.CHECK_OPEN.equals(action)){
                setStartService(isStartService?false:true);
            }else {
                ycCookie.putBoolean(action, ycCookie.getBoolean(action)?false:true);
            }
            if(action.equals(YCCookie.CHECK_OPEN)){
                if(isChecked){
                    FloatWindowManager.setSvecImgTag("CLOSE");
                    ServiceManager.closeVoice(getApplicationContext());
                }else{
                    FloatWindowManager.setSvecImgTag("OPEN");
                    ServiceManager.startVoice(getApplicationContext());

                }
            }
            if(action.equals(YCCookie.WIN_OPEN)){
                if(isChecked){
                    FloatWindowManager.removeSmallWindow(getApplicationContext());
                    ServiceManager.stopFloatService(getApplicationContext());
                }else {
                    ServiceManager.startFloatService(getApplicationContext());
                }
            }
        }
        setImage(null,1);
        if(ycCookie.CHECK_OPEN.equals(action)){
            ServiceManager.sendSeniorReceiver(getApplicationContext(),15);
        }else {
            ServiceManager.sendSeniorReceiver(getApplicationContext(),5);
        }

    }

    public Handler getmHandler() {
        return mHandler;
    }

    public boolean isStartService() {
        return isStartService;
    }

    public void setStartService(boolean startService) {
        isStartService = startService;
    }
}
