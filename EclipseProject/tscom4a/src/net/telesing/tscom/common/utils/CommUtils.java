package net.telesing.tscom.common.utils;

import android.util.Log;
import net.telesing.tscom.bean.SN;
import org.apache.http.util.EncodingUtils;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by Administrator on 2015/10/9.
 */
public class CommUtils {
    /**
     * 获取字符串中数字
     *
     * @param text
     * @return
     */
    public static long getNumber(String text) {
        String regEx = "[^0-9]";
        Pattern p = Pattern.compile(regEx);
        Matcher m = p.matcher(text);
        System.out.println(m.replaceAll("").trim());
        return Long.parseLong(m.replaceAll("").trim());
    }

    public static boolean isNull(String text) {
        if (text != null && text.trim().length() > 0)
            return true;
        return false;
    }

    public static String readText(String fileName) {
        Log.e("fileName", fileName);
        String res = "";
        try {
            FileInputStream fin = new FileInputStream(fileName);
            int length = fin.available();
            byte[] buffer = new byte[length];
            fin.read(buffer);
            res = EncodingUtils.getString(buffer, "UTF-8");
            fin.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            res = "文件不存在";
        } catch (IOException e) {
            res = "文件读取异常";
        }
        return res;
    }

    public static List<SN> findSns(String text){
        List<SN> list = new ArrayList<SN>();
        String res = getNumberString(text);
        String nums [] = res.split(",");
        for (String str : nums) {
            if(str.trim().length() > 0){
                SN sn = new SN();
                sn.setSn(str);
                list.add(sn);
            }
        }
        return list;
    }
    public static String getNumberString (String text) {
        String regEx = "[^0-9]";
        Pattern p = Pattern.compile(regEx);
        Matcher m = p.matcher(text);
        return m.replaceAll(",").trim();
    }

    public static String getDate() {
        Calendar c = Calendar.getInstance();
        int year = c.get(Calendar.YEAR);
        int month = c.get(Calendar.MONTH) + 1;
        int day = c.get(Calendar.DAY_OF_MONTH);
        int hour = c.get(Calendar.HOUR_OF_DAY);
        int mins = c.get(Calendar.MINUTE);
        int second = c.get(Calendar.SECOND);
        int milSecond = c.get(Calendar.MILLISECOND);
        StringBuffer sbBuffer = new StringBuffer();
        sbBuffer.append(year + "" + (month >= 10 ? (month + "") : ("0" + month)) + "" + (day >= 10 ? (day + "") : ("0" + day)) + "_" + (hour >= 10 ? (hour + "") : ("0" + hour)) + "" + (mins >= 10 ? (mins + "") : ("0" + mins)) + "" + (second >= 10 ? (second + "") : ("0" + second) + "." + milSecond));
        return sbBuffer.toString();
    }
    public static String getCurrentTimeStr()
    {
        SimpleDateFormat format = new SimpleDateFormat("yyyyMMddhhmmss");
        String result = format.format(new Date());
        return result;
    }
    public static String long2String(long longDate) {
        String sDateTime = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.S");
        Date dt = new Date(longDate);
        sDateTime = sdf.format(dt);
        return sDateTime;
    }

    public static String longTString(long longDate) {
        String sDateTime = null;
        SimpleDateFormat sdf = new SimpleDateFormat("MM-dd HH:mm:ss.S");
        Date dt = new Date(longDate);
        sDateTime = sdf.format(dt);
        return sDateTime;
    }

    public static String long2ShortString(long longDate) {
        String sDateTime = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd_HHmmss");
        Date dt = new Date(longDate);
        sDateTime = sdf.format(dt);
        return sDateTime;
    }

    public static String longShortString(long longDate) {
        String sDateTime = null;
        SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
        Date dt = new Date(longDate);
        sDateTime = sdf.format(dt);
        return sDateTime;
    }

    public static String longToTime(long longDate) {
        String sDateTime = null;
        SimpleDateFormat sdf = new SimpleDateFormat("HH:mm");
        Date dt = new Date(longDate);
        sDateTime = sdf.format(dt);
        return sDateTime;
    }

    public static String getDiffSeconds(long endTime, long startTime) {
        long l = endTime - startTime;
        return getDiff(l);
    }

    public static String getDiff(long time){
        long day = time / (24 * 60 * 60 * 1000);
        long hour = (time / (60 * 60 * 1000) - day * 24);
        long min = ((time / (60 * 1000)) - day * 24 * 60 - hour * 60);
        long s = (time / 1000 - day * 24 * 60 * 60 - hour * 60 * 60 - min * 60);
        long ms = (time - day * 24 * 60 * 60 * 1000 - hour * 60 * 60 * 1000 - min * 60 * 1000 - s * 1000);
        return String.valueOf((double) s + ((double) ms) / 1000);
    }


}
