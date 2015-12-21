package net.telesing.acs.common.utils;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Build;
import android.util.DisplayMetrics;
import net.telesing.acs.R;

import java.lang.reflect.Method;


public final class DisplayUtil
{
    private DisplayUtil() { }

    public static String getCurrentAppVersionName(Context context) throws Exception {
        //获取PackageManager的实例
        PackageManager packageManager = context.getPackageManager();
        //getPackageName()是你当前类的包名，0代表是获取版本信息
        PackageInfo packageInfo = packageManager.getPackageInfo(context.getPackageName(), 0);
        String versionName = packageInfo.versionName;
        return context.getString(R.string.version) + versionName;
    }

    public static float getDensity(Context context){
        return context.getResources().getDisplayMetrics().density;
    }

    public static int getPx(Context context,int dp){
        return (int)(dp*getDensity(context));
    }


    //<editor-fold desc="长度转换">
    public static int px2dip(Context context, float pxValue)
    {
        final float scale = context.getResources().getDisplayMetrics().density;
        return (int) (pxValue / scale + 0.5f);
    }

    public static int dip2px(Context context, float dipValue)
    {
        final float scale = context.getResources().getDisplayMetrics().density;
        return (int) (dipValue * scale + 0.5f);
    }

    public static int px2sp(Context context, float pxValue)
    {
        final float fontScale = context.getResources().getDisplayMetrics().scaledDensity;
        return (int) (pxValue / fontScale + 0.5f);
    }

    public static int sp2px(Context context, float spValue)
    {
        final float fontScale = context.getResources().getDisplayMetrics().scaledDensity;
        return (int) (spValue * fontScale + 0.5f);
    }
    //</editor-fold>
    public static int getDisplayScreenResolution(Activity context)
    {
        int ver = Build.VERSION.SDK_INT;

        DisplayMetrics dm = new DisplayMetrics();
        android.view.Display display = context.getWindowManager().getDefaultDisplay();
        display.getMetrics(dm);

        int screen_w  = dm.widthPixels;
        int screen_h = dm.heightPixels;
        if (ver < 13)
        {
            screen_h = dm.heightPixels;
        }
        else if (ver == 13)
        {
            try {
                Method mt = display.getClass().getMethod("getRealHeight");
                screen_h = (Integer) mt.invoke(display);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        else if (ver > 13)
        {
            try {
                Method mt = display.getClass().getMethod("getRawHeight");
                screen_h = (Integer) mt.invoke(display);

            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return screen_h;
    }
}
