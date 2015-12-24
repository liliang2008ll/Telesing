package net.telesing.tscom.bean;


import android.database.Cursor;
import java.util.ArrayList;
import java.util.List;

public class BeanManage {


    public static final List<SN> findAllSn(Cursor cursor) {

        List<SN> list = new ArrayList<SN>();
        if (cursor != null) {
            while (cursor.moveToNext()) {
                SN sn = getSNBean(cursor);
                if (sn != null)
                    list.add(sn);
            }
        }
        return list;
    }

    public static final SN getSNBean(Cursor cursor) {
        SN snBean = null;
        if (cursor != null) {
            snBean = new SN();
            snBean.setId(cursor.getLong(cursor.getColumnIndex("id")));
            snBean.setSn(cursor.getString(cursor.getColumnIndex("sn")));
        }
        return snBean;
    }

    public static final SN getOneSNBean(Cursor cursor) {
        SN snBean = null;
        if (cursor != null) {
            if(cursor.moveToNext()){

                snBean = new SN();
                snBean.setId(cursor.getLong(cursor.getColumnIndex("id")));
                snBean.setSn(cursor.getString(cursor.getColumnIndex("sn")));
            }
        }
        return snBean;
    }



}
