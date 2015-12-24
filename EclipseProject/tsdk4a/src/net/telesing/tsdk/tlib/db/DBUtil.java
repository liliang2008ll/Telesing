package net.telesing.tsdk.tlib.db;


import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;
import net.telesing.tsdk.tlib.bean.SectionBean;

import java.util.ArrayList;
import java.util.List;

public class DBUtil {

    public static void operReceiver(String packageName,String rsa, SQLiteDatabase sdb, DBHelper dbHelper) {
        Log.e("===>",packageName+","+rsa);
        String rsas[] = rsa.split("-");
        operSvec(packageName, rsas[0], 50, 1111111,rsas[1],sdb, dbHelper);
    }
    public static void operSvec(String packageName,String section,int validate,int sid, String orgName,SQLiteDatabase sdb, DBHelper dbHelper) {
        if(!isHad(packageName,section,sdb,dbHelper)){
            ContentValues ctv = new ContentValues();
            ctv.put("packageName", packageName);
            ctv.put("section", section);
            ctv.put("validate", validate);
            ctv.put("sid", sid);
            ctv.put("state",1);
            ctv.put("upt_date", System.currentTimeMillis());
            ctv.put("org_name",orgName);
            dbHelper.insertSQL("sca_section", null, ctv, sdb);
        }

    }

    public static boolean isHad(String packageName,String rsa, SQLiteDatabase sdb, DBHelper dbHelper){
        Cursor cursor = dbHelper.execQuery("select * from sca_section where section = '" + rsa + "' and packageName = '" + packageName + "'", sdb);
        if(cursor != null && cursor.getCount() > 0){
            Log.e("isHad",String.valueOf(cursor.getCount()));
            cursor.close();
            return true;
        }
        cursor.close();
        return false;
    }


    public static List<SectionBean> findPackages(String section, SQLiteDatabase sdb, DBHelper dbHelper){
        String sql = "select * from sca_section where section = '"+section+"'";
        Cursor cursor = dbHelper.execQuery(sql, sdb);
        List<SectionBean> packages = null;
        if(cursor != null ){
            packages = new ArrayList<SectionBean>();
            SectionBean sectionBean = null;
            while (cursor.moveToNext()){
                int validate = cursor.getInt(cursor.getColumnIndex("validate"));
                long upttime = cursor.getLong(cursor.getColumnIndex("upt_date"));
                if(validdate(validate,upttime)){
                    sectionBean = new SectionBean();
                    sectionBean.setPackageName(cursor.getString(cursor.getColumnIndex("packageName")));
                    sectionBean.setOrgName(cursor.getString(cursor.getColumnIndex("org_name")));
                    packages.add(sectionBean);
                }
            }
            cursor.close();
        }
        return packages;
    }

    public static boolean  validdate(int vdate,long uptTime){
        long date = (System.currentTimeMillis()-uptTime)/(24*60*60*1000);
        if(Integer.parseInt(String.valueOf(date)) < vdate)
            return true;
        return false;
    }

}
