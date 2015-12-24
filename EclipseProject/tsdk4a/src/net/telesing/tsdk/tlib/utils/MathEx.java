package net.telesing.tsdk.tlib.utils;

import java.math.BigDecimal;
import java.text.DecimalFormat;

/**
 * Created by FengJZ on 2014/8/14.
 */
public class MathEx {

    public static int divideCeil(int a, int b) {
        return (int) Math.ceil(a / (double) b);
    }

    public static int divideRHU(int a, int b) {
        BigDecimal target = new BigDecimal(Double.toString(a / (double) b));
        BigDecimal one = new BigDecimal("1");
        return (int) target.divide(one, 0, BigDecimal.ROUND_HALF_UP).doubleValue();
    }

    public static double roundHalfUp(double value, int scale) {
        BigDecimal target = new BigDecimal(Double.toString(value));
        BigDecimal one = new BigDecimal("1");
        return target.divide(one, scale, BigDecimal.ROUND_HALF_UP).doubleValue();
    }

    public static String formatHalfUp(double value, int scale) {
        StringBuilder sb = new StringBuilder("0");
        if (scale > 0) {
            sb.append(".");
            for (int i = 0; i < scale; i++) {
                sb.append("0");
            }
        }
        DecimalFormat df = new DecimalFormat(sb.toString());
        return df.format(value);
    }
}