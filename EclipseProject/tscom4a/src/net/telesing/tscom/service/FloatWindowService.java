package net.telesing.tscom.service;

import android.app.ActivityManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.os.Handler;
import android.os.IBinder;
import net.telesing.tscom.SvecApplication;
import net.telesing.tscom.bean.SN;
import net.telesing.tscom.common.utils.YCCookie;
import net.telesing.tscom.manager.FloatWindowManager;
import net.telesing.tscom.manager.ServiceManager;
import net.telesing.tscom.receiver.OlympicsReceiver;
import net.telesing.tscom.receiver.SNReceiver;
import net.telesing.tsdk.tlib.bean.SNBean;
import net.telesing.tsdk.tlib.core.AcommsInterface;

import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;


public class FloatWindowService extends Service {

    /**
     * 用于在线程中创建或移除悬浮窗。
     */
    private Handler handler = new Handler();
    public YCCookie ycCookie;
    public SharedPreferences sharedPreferences;
    /**
     * 定时器，定时进行检测当前应该创建还是移除悬浮窗。
     */
    private Timer timer;

    private int total = 0;

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        sharedPreferences = getApplicationContext().getSharedPreferences(YCCookie.COOKIENAME, Context.MODE_PRIVATE);
        ycCookie = new YCCookie(sharedPreferences);
        registerSnReceiver();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        // 开启定时器，每隔0.5秒刷新一次
        if(ycCookie.getBoolean(YCCookie.OPEN_RUN) || intent.getIntExtra("win_open",0) == 2){
            if(ycCookie.getBoolean(YCCookie.WIN_OPEN)){
                if (timer == null) {
                    timer = new Timer();
                    timer.scheduleAtFixedRate(new RefreshTask(), 0, 500);
                }
            }
            if(SvecApplication.getInstance().isStartService()){
                FloatWindowManager.setSvecImgTag("OPEN");
                ServiceManager.startVoice(getApplicationContext());
            }else {
                FloatWindowManager.setSvecImgTag("CLOSE");
            }
            SvecApplication.getInstance().showNotification(getApplicationContext());
        }
        flags = START_STICKY;
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        // Service被终止的同时也停止定时器继续运行
//        cancelNotification();
        timer.cancel();
        timer = null;
        stopForeground(true);
        sendReceiver();
    }

    private void sendReceiver(){
        if(snReceiver != null)
            unregisterReceiver(snReceiver);
        Intent intent = new Intent(OlympicsReceiver.ACTION_1);
        sendBroadcast(intent);
    }

    class RefreshTask extends TimerTask {

        @Override
        public void run() {
            if (isHome() && !FloatWindowManager.isWindowShowing()) {// 当前界面是桌面，且没有悬浮窗显示，则创建悬浮窗。
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        FloatWindowManager.createSmallWindow(getApplicationContext(),SvecApplication.getInstance().isStartService());
                    }
                });
            }else if (!isHome() && FloatWindowManager.isWindowShowing()) { // 当前界面不是桌面，且有悬浮窗显示，则移除悬浮窗。
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        if(ycCookie.getBoolean(YCCookie.DESTTOP_OPEN)){
                            FloatWindowManager.removeSmallWindow(getApplicationContext());
                        }

                    }
                });
            }
            if ( FloatWindowManager.isWindowShowing()) { // 当前界面是桌面，且有悬浮窗显示，则更新内存数据。
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        if(total > 0){
                            total--;
                        }else {
                            total = 0;
                            FloatWindowManager.updateUsedPercent(1);
                        }

                    }
                });
            }
        }
    }

    /**
     * 判断当前界面是否是桌面
     */
    private boolean isHome() {
        ActivityManager mActivityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        List<ActivityManager.RunningTaskInfo> rti = mActivityManager.getRunningTasks(1);
        return getHomes().contains(rti.get(0).topActivity.getPackageName());
    }

    /**
     * 获得属于桌面的应用的应用包名称
     *
     * @return 返回包含所有包名的字符串列表
     */
    private List<String> getHomes() {
        List<String> names = new ArrayList<String>();
        PackageManager packageManager = this.getPackageManager();
        Intent intent = new Intent(Intent.ACTION_MAIN);
        intent.addCategory(Intent.CATEGORY_HOME);
        List<ResolveInfo> resolveInfo = packageManager.queryIntentActivities(intent,
                PackageManager.MATCH_DEFAULT_ONLY);
        for (ResolveInfo ri : resolveInfo) {
            names.add(ri.activityInfo.packageName);
        }
        return names;
    }

    private SNReceiver snReceiver;

    public void registerSnReceiver()
    {
        if(snReceiver == null)
        {
            snReceiver = new SNReceiver()
            {
                @Override
                protected void doWhat(SNBean snBean){

                    super.doWhat(snBean);
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            total = 10;
                            FloatWindowManager.updateUsedPercent(2);
                        }
                    });
                }
            };
            IntentFilter snFilter = new IntentFilter(AcommsInterface.HAVE_SN_INTENT);
            registerReceiver(snReceiver, snFilter);
        }
    }


}
