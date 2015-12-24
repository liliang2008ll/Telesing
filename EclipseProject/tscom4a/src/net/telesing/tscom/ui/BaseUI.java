package net.telesing.tscom.ui;

import android.content.Context;
import android.content.SharedPreferences;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.KeyEvent;
import android.widget.Button;
import net.telesing.tscom.common.utils.YCCookie;
import net.telesing.tscom.db.DBHelper;
import net.telesing.tscom.receiver.SNReceiver;
import net.telesing.tsdk.tlib.db.Cookie;

/**
 * Created by Administrator on 2015/11/20.
 */
public class BaseUI extends FragmentActivity {
    public YCCookie ycCookie;
    public SharedPreferences sharedPreferences;
    public Button backBtn,saveBtn;
    public DBHelper dbHelper;
    public SQLiteDatabase sdb;
    public SNReceiver snReceiver;

    public Cookie cookie;
    public SharedPreferences c_sharedPreferences;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        sharedPreferences = getSharedPreferences(YCCookie.COOKIENAME, Context.MODE_PRIVATE);
        ycCookie = new YCCookie(sharedPreferences);
        c_sharedPreferences = getSharedPreferences(Cookie.COOKIENAME, Context.MODE_PRIVATE);
        cookie = new Cookie(c_sharedPreferences);
        dbHelper = new DBHelper(BaseUI.this, DBHelper.DB_NAME, null, DBHelper.DB_VERSION);
        sdb = dbHelper.getWritableDatabase();
    }

    public void goBack(){
        BaseUI.this.finish();
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            goBack();
            return false;
        } else {
            return super.onKeyDown(keyCode, event);
        }
    }



}
