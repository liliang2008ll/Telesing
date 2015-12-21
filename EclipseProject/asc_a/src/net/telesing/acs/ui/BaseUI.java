package net.telesing.acs.ui;

import android.content.Context;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.os.Message;
import android.support.v4.app.FragmentActivity;
import android.widget.Button;
import net.telesing.acs.common.contants.ConstantHandler;
import net.telesing.acs.common.utils.YCCookie;
import net.telesing.acs.db.DBHelper;
import net.telesing.acs.receiver.SNReceiver;
import net.telesing.scomm.android.DemInterface;
import net.telesing.scomm.android.SvecResBean;

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
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        sharedPreferences = getSharedPreferences(YCCookie.COOKIENAME, Context.MODE_PRIVATE);
        ycCookie = new YCCookie(sharedPreferences);
        dbHelper = new DBHelper(BaseUI.this, DBHelper.DB_NAME, null, DBHelper.DB_VERSION);
        sdb = dbHelper.getWritableDatabase();
    }





}
