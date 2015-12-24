package net.telesing.tsdk.tlib.utils;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;

/**
 * Created by Administrator on 2015/12/16.
 */
public class Utils {
    /**
     * 获取日期相差天数
     *
     * @param
     * @return 日期类型时间
     * @throws ParseException
     */
    public static int getDiffDay(String beginDate, String endDate) {
        SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd");
        int checkday = 0;
        try {
            checkday = (int)(formatter.parse(endDate).getTime() - formatter.parse(beginDate).getTime()) / (1000 * 24 * 60 * 60);
        } catch (ParseException e) {
            e.printStackTrace();
            checkday = 0;
        }
        return checkday;
    }


    /**
     * 获取系统时间
     *
     * @param type: 1 获取日期  2 获取详细时间
     * @return 字符串类型时间
     * @throws ParseException
     */
    public static String getStrSystemTime(String type) {
        String Pattern = "yyyy-MM-dd";
        if (type.equals("1")) {
            Pattern = "yyyy-MM-dd";
        }
        if (type.equals("2")) {
            Pattern = "yyyy-MM-dd HH:mm:ss";
        }
        SimpleDateFormat formatter = new SimpleDateFormat(Pattern);
        Calendar calendar = Calendar.getInstance();
        formatter.format(calendar.getTime());
        String strdate = null;
        strdate = formatter.format(calendar.getTime());
        return strdate;
    }
}
