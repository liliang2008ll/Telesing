package net.telesing.scomm.android;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.database.sqlite.SQLiteDatabase;
import android.os.IBinder;
import android.util.Log;
import net.telesing.acs.db.DBHelper;
import net.telesing.acs.db.DBUtil;
import net.telesing.scomm.android.core.Demodulator;
import net.telesing.scomm.android.receiver.RegisterRecieiver;

/**
 * Created by FengJZ on 2015/4/21.
 */
public class DemService extends Service {

    public static final String OP_STR = "DemService.Start.OP_STR";
    public static final int OP_DEF = 0;
    public static final int OP_OPEN = 1;
    public static final int OP_RUN = 2;
    public Cookie ycCookie;
    public SharedPreferences sharedPreferences;
    public DBHelper dbHelper;
    public SQLiteDatabase sdb;
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        sharedPreferences = getApplicationContext().getSharedPreferences(Cookie.COOKIENAME, Context.MODE_PRIVATE);
        ycCookie = new Cookie(sharedPreferences);
        dbHelper = new DBHelper(getApplicationContext(), DBHelper.DB_NAME, null, DBHelper.DB_VERSION);
        sdb = dbHelper.getWritableDatabase();
        registerSnReceiver();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        super.onStartCommand(intent, flags, startId);
        //注册广播
        if (intent != null) {

            Demodulator.setContext(getApplicationContext());
            Demodulator.setCookie(ycCookie);
            Demodulator.setSQLiteDatabase(sdb);
            Demodulator.setDBHelper(dbHelper);

            switch (intent.getIntExtra(OP_STR, OP_DEF)) {
                case OP_OPEN:
                    Demodulator.open();
                    break;
                case OP_RUN:
                    Demodulator.run();
                    break;
            }
        }
        return START_STICKY;
    }

    private RegisterRecieiver registerRecieiver;

    private void registerSnReceiver() {
        if (registerRecieiver == null) {
            Log.e("registerRecieiver:","here le ");
            registerRecieiver = new RegisterRecieiver() {

                @Override
                protected void doWhat(String packageName, String asc) {
                    super.doWhat(packageName,asc);
                    Log.e("registerRecieiver:",packageName+","+asc);
                    DBUtil.operReceiver(packageName, asc, sdb, dbHelper);

                }
            };
            IntentFilter snFilter = new IntentFilter(RegisterRecieiver.ACTION);
            registerReceiver(registerRecieiver, snFilter);
        }
    }

    private void unregisterReceiver()
    {

        if(registerRecieiver != null)
            unregisterReceiver(registerRecieiver);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Demodulator.close();
        unregisterReceiver();
    }

}
