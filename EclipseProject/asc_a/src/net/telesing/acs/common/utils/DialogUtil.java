package net.telesing.acs.common.utils;

import android.content.Context;
import android.os.Handler;
import android.view.WindowManager;
import android.widget.Toast;
import net.telesing.acs.view.LoginDialog;


public class DialogUtil {
    public static void showToast(Context context, String text, int duration) {
        if (text.length() > 0) {
            Toast.makeText(context, text, duration).show();
        }
    }

    public static void showToast(Context context, int resId, int duration) {
        Toast.makeText(context, resId, duration).show();
    }



    public static void showOperDialog(Context context,Handler handler,int title,int type,int handlerWhat){
        LoginDialog alertDialog = new LoginDialog(context,handler,title,type,handlerWhat);
        alertDialog.getWindow().setType(WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
        alertDialog.show();
    }
    public static void showOperDialog(Context context,Handler handler,String title,int type,int handlerWhat){
        LoginDialog alertDialog = new LoginDialog(context,handler,title,type,handlerWhat);
        alertDialog.getWindow().setType(WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
        alertDialog.show();
    }

}
