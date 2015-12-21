package net.telesing.acs.db;


import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;
import net.telesing.acs.bean.BeanManage;
import net.telesing.acs.bean.SNBean;
import net.telesing.acs.bean.SectionBean;
import net.telesing.acs.common.contants.ConstantValues;
import net.telesing.scomm.android.SvecResBean;

import java.util.ArrayList;
import java.util.List;

public class DBUtil {

    public static void wavData(SvecResBean svecResBean, SQLiteDatabase sdb, DBHelper dbHelper) {
        Cursor cursor = dbHelper.execQueryParam(ConstantValues.GET_SVECRESBEAN_BY_DATE_SQL, new String[]{svecResBean.getDates()}, sdb);
        SvecResBean svecBean = BeanManage.getSvecBean(cursor);
        cursor.close();
        if (svecBean == null) {

            operSvec(svecResBean, sdb, dbHelper, 0);

        } else {
            operSvec(svecResBean, sdb, dbHelper, 1);
        }
    }

    public static String getMsgs(String sn,SQLiteDatabase sdb, DBHelper dbHelper){
        Cursor cursor = dbHelper.execQueryParam(ConstantValues.GET_SNMSG_BY_DATE_SQL, new String[]{sn}, sdb);
        String msgs = null;
        if(cursor != null && cursor.getCount() > 0){
            if(cursor.moveToFirst()){
                msgs = cursor.getString(cursor.getColumnIndex("msg"));
            }
        }
        return msgs;
    }

    public static void wavMsgData(String sn,String msgs, SQLiteDatabase sdb, DBHelper dbHelper) {
        Cursor cursor = dbHelper.execQueryParam(ConstantValues.GET_SNMSG_BY_DATE_SQL, new String[]{sn}, sdb);
        if (cursor == null || cursor.getCount() == 0) {
            operMsg(sn, msgs, sdb, dbHelper, 0);

        } else {
            operMsg(sn, msgs, sdb, dbHelper, 1);
        }
        cursor.close();
    }

    public static void operMsg(String sn,String msg, SQLiteDatabase sdb, DBHelper dbHelper, int type) {
        ContentValues ctv = new ContentValues();
        ctv.put("sn", sn);
        ctv.put("msg", msg);
        if (type == 0) {
            dbHelper.insertSQL("sn_msg", null, ctv, sdb);
        } else {
            dbHelper.updateSQL("sn_msg", ctv, "sn = ?", new String[]{sn}, sdb);
        }
    }


    public static void operSvec(SvecResBean svecResBean, SQLiteDatabase sdb, DBHelper dbHelper, int type) {
        ContentValues ctv = new ContentValues();
        ctv.put("dates", svecResBean.getDates());
        ctv.put("file_path", svecResBean.getFilePath());
        ctv.put("resTag", svecResBean.getResTag());
        ctv.put("total_time", svecResBean.getTotalTime());
        ctv.put("wav_path", svecResBean.getWavPath());
        ctv.put("result", svecResBean.getResult());
        if (type == 0) {
            dbHelper.insertSQL("svec", null, ctv, sdb);
        } else {
            dbHelper.updateSQL("svec", ctv, "dates = ?", new String[]{svecResBean.getDates()}, sdb);
        }
    }

    public static List<SvecResBean> findAllSvecs(String sql, SQLiteDatabase sdb, DBHelper dbHelper) {
        Cursor cursor = dbHelper.execQuery(sql, sdb);
        List<SvecResBean> svecList = BeanManage.findSvecs(cursor);
        cursor.close();
        return svecList;
    }

    public static void wavSnData(SNBean snBean, SQLiteDatabase sdb, DBHelper dbHelper) {
        Cursor cursor = dbHelper.execQueryParam(ConstantValues.GET_SN, new String[]{snBean.getSn()}, sdb);
        SNBean sn = BeanManage.getOneSNBean(cursor);
        cursor.close();
        if (sn == null) {//????
            operSn(snBean, sdb, dbHelper);

        }
    }

    public static boolean checkSn(String sn, SQLiteDatabase sdb, DBHelper dbHelper){
        Cursor cursor = dbHelper.execQueryParam(ConstantValues.GET_SN, new String[]{sn}, sdb);
        SNBean snBean = BeanManage.getOneSNBean(cursor);
        cursor.close();
        if(snBean != null)
            return true;
        return false;
    }

    public static void operSn(SNBean snBean, SQLiteDatabase sdb, DBHelper dbHelper) {
        ContentValues ctv = new ContentValues();
        ctv.put("sn", snBean.getSn());
        dbHelper.insertSQL("sn_list", null, ctv, sdb);

    }

    public static List<SNBean> findAllSN( SQLiteDatabase sdb, DBHelper dbHelper) {
        Cursor cursor = dbHelper.execQuery(ConstantValues.FIND_ALL_SN, sdb);
        List<SNBean> svecList = BeanManage.findAllSn(cursor);
        cursor.close();
        return svecList;
    }

    public static int deleteAllSN(DBHelper dbHelper,SQLiteDatabase sdb){
        return dbHelper.deleteSQL("sn_list",null,null,sdb);
    }



    public static void operReceiver(String packageName,String rsa, SQLiteDatabase sdb, DBHelper dbHelper) {
        //???? rsa ???
        Log.e("===>",packageName+","+rsa);
        operSvec(packageName, rsa, 50, 1111111,"临时机构",sdb, dbHelper);
    }
    //section VARCHAR(50),validate VARCHAR(200),sid VARCHAR(50),state INTEGER,upt_date INTEGER )
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
        String temp = section.substring(0, 6);
        Log.e("temp",temp);
        String sql = "select * from sca_section where section = '"+temp+"'";
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
