package net.telesing.acs.bean;


import android.database.Cursor;
import net.telesing.scomm.android.SvecResBean;

import java.util.ArrayList;
import java.util.List;

public class BeanManage {
    public static final SvecResBean getSvecBean(Cursor cursor) {
        SvecResBean svecResBean = null;
        if (cursor != null) {
            if (cursor.moveToNext()) {
                svecResBean = new SvecResBean();
                svecResBean.setDates(cursor.getString(cursor.getColumnIndex("dates")));
                svecResBean.setFilePath(cursor.getString(cursor.getColumnIndex("file_path")));
                svecResBean.setId(cursor.getInt(cursor.getColumnIndex("id")));
                svecResBean.setResTag(cursor.getInt(cursor.getColumnIndex("res")));
                svecResBean.setTotalTime(cursor.getString(cursor.getColumnIndex("total_time")));
                svecResBean.setWavPath(cursor.getString(cursor.getColumnIndex("wav_path")));
                svecResBean.setResult(cursor.getString(cursor.getColumnIndex("result")));

            }
        }
        return svecResBean;
    }

    public static final List<SvecResBean> findSvecs(Cursor cursor) {

        List<SvecResBean> list = new ArrayList<SvecResBean>();
        if (cursor != null) {
            while (cursor.moveToNext()) {
                SvecResBean svec = getSvecBeanNoMove(cursor);
                if (svec != null)
                    list.add(svec);
            }
        }
        return list;
    }

    public static final List<SNBean> findAllSn(Cursor cursor) {

        List<SNBean> list = new ArrayList<SNBean>();
        if (cursor != null) {
            while (cursor.moveToNext()) {
                SNBean svec = getSNBean(cursor);
                if (svec != null)
                    list.add(svec);
            }
        }
        return list;
    }

    public static final SNBean getSNBean(Cursor cursor) {
        SNBean snBean = null;
        if (cursor != null) {
            snBean = new SNBean();
            snBean.setId(cursor.getLong(cursor.getColumnIndex("id")));
            snBean.setSn(cursor.getString(cursor.getColumnIndex("sn")));
        }
        return snBean;
    }

    public static final SNBean getOneSNBean(Cursor cursor) {
        SNBean snBean = null;
        if (cursor != null) {
            if(cursor.moveToNext()){

                snBean = new SNBean();
                snBean.setId(cursor.getLong(cursor.getColumnIndex("id")));
                snBean.setSn(cursor.getString(cursor.getColumnIndex("sn")));
            }
        }
        return snBean;
    }

    public static final SvecResBean getSvecBeanNoMove(Cursor cursor) {

        SvecResBean svecResBean = null;
        if (cursor != null) {

            svecResBean = new SvecResBean();
            svecResBean.setDates(cursor.getString(cursor.getColumnIndex("dates")));
            svecResBean.setFilePath(cursor.getString(cursor.getColumnIndex("file_path")));
            svecResBean.setId(cursor.getInt(cursor.getColumnIndex("id")));
            svecResBean.setResTag(cursor.getInt(cursor.getColumnIndex("resTag")));
            svecResBean.setTotalTime(cursor.getString(cursor.getColumnIndex("total_time")));
            svecResBean.setWavPath(cursor.getString(cursor.getColumnIndex("wav_path")));
            svecResBean.setResult(cursor.getString(cursor.getColumnIndex("result")));
        }
        return svecResBean;
    }

    public static final List<Integer> allBecons() {
        List<Integer> becons = new ArrayList<Integer>();
        becons.add(39583384);
        becons.add(12427821);
        becons.add(78608620);
        becons.add(33958164);
        becons.add(89704060);
        becons.add(59851413);
        becons.add(37302884);
        becons.add(49965496);
        becons.add(43775358);
        becons.add(34548392);


        becons.add(42554477);
        becons.add(47482541);
        becons.add(26353176);
        becons.add(37119171);
        becons.add(18958400);
        becons.add(42059959);
        becons.add(54469479);
        becons.add(30244048);
        becons.add(19805351);
        becons.add(38614020);


        return becons;
    }


}
