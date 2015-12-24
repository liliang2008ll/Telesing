package net.telesing.tscom.db;


import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import net.telesing.tscom.bean.BeanManage;
import net.telesing.tscom.bean.SN;
import net.telesing.tscom.common.contants.ConstantValues;
import java.util.List;

public class DBUtil {






    public static void wavSnData(SN snBean, SQLiteDatabase sdb, DBHelper dbHelper) {
        Cursor cursor = dbHelper.execQueryParam(ConstantValues.GET_SN, new String[]{snBean.getSn()}, sdb);
        SN sn = BeanManage.getOneSNBean(cursor);
        cursor.close();
        if (sn == null) {
            operSn(snBean, sdb, dbHelper);

        }
    }

    public static boolean checkSn(String sn, SQLiteDatabase sdb, DBHelper dbHelper){
        Cursor cursor = dbHelper.execQueryParam(ConstantValues.GET_SN, new String[]{sn}, sdb);
        SN snBean = BeanManage.getOneSNBean(cursor);
        cursor.close();
        if(snBean != null)
            return true;
        return false;
    }

    public static void operSn(SN snBean, SQLiteDatabase sdb, DBHelper dbHelper) {
        ContentValues ctv = new ContentValues();
        ctv.put("sn", snBean.getSn());
        dbHelper.insertSQL("sn_list", null, ctv, sdb);

    }

    public static List<SN> findAllSN( SQLiteDatabase sdb, DBHelper dbHelper) {
        Cursor cursor = dbHelper.execQuery(ConstantValues.FIND_ALL_SN, sdb);
        List<SN> svecList = BeanManage.findAllSn(cursor);
        cursor.close();
        return svecList;
    }

    public static int deleteAllSN(DBHelper dbHelper,SQLiteDatabase sdb){
        return dbHelper.deleteSQL("sn_list",null,null,sdb);
    }





}
