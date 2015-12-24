package net.telesing.tsdk.tlib.service;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.database.sqlite.SQLiteDatabase;
import android.os.IBinder;
import android.util.Log;
import net.telesing.tsdk.tlib.core.Demodulator;
import net.telesing.tsdk.tlib.db.Cookie;
import net.telesing.tsdk.tlib.db.DBHelper;

/**
 * Created by Administrator on 2015/12/2.
 */
public class AcommsService extends Service {

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



    @Override
    public void onDestroy() {
        super.onDestroy();
        Demodulator.close();
    }


}
