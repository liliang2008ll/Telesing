package net.telesing.acs.service;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.sqlite.SQLiteDatabase;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import net.telesing.acs.SvecApplication;
import net.telesing.acs.aidl.IAcsService;
import net.telesing.acs.db.DBHelper;
import net.telesing.acs.db.DBUtil;
import net.telesing.acs.manager.ServiceManager;
import net.telesing.scomm.android.Cookie;
import net.telesing.scomm.android.DemInterface;
import net.telesing.scomm.android.core.Demodulator;

import java.util.Random;


public class AscServiceImpl extends Service {

    public Cookie ycCookie;
    public SharedPreferences sharedPreferences;
    public DBHelper dbHelper;
    public SQLiteDatabase sdb;

    @Override
    public void onCreate() {
        super.onCreate();
        sharedPreferences = getApplicationContext().getSharedPreferences(Cookie.COOKIENAME, Context.MODE_PRIVATE);
        ycCookie = new Cookie(sharedPreferences);
        dbHelper = new DBHelper(getApplicationContext(), DBHelper.DB_NAME, null, DBHelper.DB_VERSION);
        sdb = dbHelper.getWritableDatabase();
        Log.e("onCreate","oncreate");
    }

    @Override
    public IBinder onBind(Intent intent) {

        return new IAscServiceImpl();
    }

    private void showNotification(){
        //打开通知栏
        if(!SvecApplication.getInstance().isNotification()){
            SvecApplication.getInstance().showNotification(getApplicationContext());
        }else{
            SvecApplication.getInstance().setImage(null,1);
        }
        //打开悬浮窗
        ServiceManager.startFloatService(getApplicationContext());
    }

    public class IAscServiceImpl extends IAcsService.Stub{
        @Override
        public boolean startService() throws RemoteException {
            return ServiceManager.startVoice(getApplicationContext());
        }

        @Override
        public void register(String packageName, String cert_code) throws RemoteException {
            DBUtil.operReceiver(packageName,cert_code,sdb,dbHelper);
        }

        @Override
        public int getSignal() throws RemoteException {
            return (int)(Demodulator.getSignal()*100);
        }

        @Override
        public int start(String packageName, String cert_code) throws RemoteException {
            //启动检测开关
            startService();
            register(packageName, cert_code); //注册
            showNotification();
            return Demodulator.state();

        }

        @Override
        public boolean isRegister(String packageName, String cert_code) throws RemoteException {
            return DBUtil.isHad(packageName,cert_code,sdb,dbHelper);
        }

        @Override
        public boolean isRunning() throws RemoteException {
            if(Demodulator.state() == DemInterface.RUNNING){
                return true;
            }
            return  false;
        }

        @Override
        public void stopService() throws RemoteException {
            ServiceManager.closeVoice(getApplicationContext());
            showNotification();
        }
    }


}
