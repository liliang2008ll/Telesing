package net.telesing.tscom.view;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import net.telesing.tscom.R;
import net.telesing.tscom.SvecApplication;
import net.telesing.tscom.manager.ServiceManager;
import net.telesing.tsdk.tlib.core.AcommsInterface;

import java.lang.reflect.Field;

/**
 * Created by Administrator on 2015/9/21.
 */
public class FloatWindowView extends RelativeLayout {

    public static int viewWidth;


    public static int viewHeight;


    private static int statusBarHeight;


    private WindowManager windowManager;


    private WindowManager.LayoutParams mParams;


    private float xInScreen;


    private float yInScreen;


    private float xDownInScreen;


    private float yDownInScreen;

    private float xInView;


    private float yInView;
    public ImageView svecImg;


    public FloatWindowView(Context context) {
        super(context);
        windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        LayoutInflater.from(context).inflate(R.layout.floating_window, this);
        View view = findViewById(R.id.float_window_layout);
        viewWidth = view.getLayoutParams().width;
        viewHeight = view.getLayoutParams().height;
        initWidget(view);
    }

    public void initWidget(View view){
        svecImg = (ImageView)view.findViewById(R.id.wins_logo);

    }


    @Override
    public boolean onTouchEvent(MotionEvent event) {
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                xInView = event.getX();
                yInView = event.getY();
                xDownInScreen = event.getRawX();
                yDownInScreen = event.getRawY() - getStatusBarHeight();
                xInScreen = event.getRawX();
                yInScreen = event.getRawY() - getStatusBarHeight();
                break;
            case MotionEvent.ACTION_MOVE:
                xInScreen = event.getRawX();
                yInScreen = event.getRawY() - getStatusBarHeight();
                updateViewPosition();
                break;
            case MotionEvent.ACTION_UP:
                if (Math.abs(xDownInScreen - xInScreen) <=10 && Math.abs(yDownInScreen - yInScreen) <= 10) {
                    openBigWindow();
                }
                break;
            default:
                break;
        }
        return true;
    }


    public void setParams(WindowManager.LayoutParams params) {
        mParams = params;
    }


    private void updateViewPosition() {
        mParams.x = (int) (xInScreen - xInView);
        mParams.y = (int) (yInScreen - yInView);
        windowManager.updateViewLayout(this, mParams);
    }


    private void openBigWindow() {
            if(SvecApplication.getInstance().isStartService()){
                SvecApplication.getInstance().setStartService(false);
                ServiceManager.closeVoice(getContext());
            }else {
                SvecApplication.getInstance().setStartService(true);
                ServiceManager.startVoice(getContext());
            }
            SvecApplication.getInstance().setImage(null,1);
            ServiceManager.sendSeniorReceiver(getContext(),15);

    }

    public void setSvecImgTag(String tag){
        if(svecImg != null){
            svecImg.setTag(tag);
            if(SvecApplication.getInstance().isStartService()){
                svecImg.setBackgroundResource(R.drawable.check_open_icon_0);
            }else {
                svecImg.setBackgroundResource(R.drawable.check_close_icon);
            }
        }
    }

    public void setSignalImageTag(int type){
        if(svecImg != null){
            if(type == 1){
                if(SvecApplication.getInstance().isStartService()){
                    int singnal = (int)(AcommsInterface.getSignal()*100);
                    if(singnal > 0 && singnal <= 30){
                        svecImg.setBackgroundResource(R.drawable.check_open_icon_1);
                    }else if(singnal > 30 && singnal <=60){
                        svecImg.setBackgroundResource(R.drawable.check_open_icon_2);
                    }else if(singnal > 60 && singnal <=100){
                        svecImg.setBackgroundResource(R.drawable.check_open_icon_3);
                    }else {
                        svecImg.setBackgroundResource(R.drawable.check_open_icon_0);
                    }
                }else{
                    svecImg.setBackgroundResource(R.drawable.check_close_icon);
                }
            }else {
                if(SvecApplication.getInstance().isStartService()){
                    svecImg.setBackgroundResource(R.drawable.check_open_icon_m);
                }else{
                    svecImg.setBackgroundResource(R.drawable.check_close_icon);
                }

            }

        }
    }


    private int getStatusBarHeight() {
        if (statusBarHeight == 0) {
            try {
                Class<?> c = Class.forName("com.android.internal.R$dimen");
                Object o = c.newInstance();
                Field field = c.getField("status_bar_height");
                int x = (Integer) field.get(o);
                statusBarHeight = getResources().getDimensionPixelSize(x);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return statusBarHeight;
    }

    public ImageView getSvecImg() {
        return svecImg;
    }

    public void setSvecImg(ImageView svecImg) {
        this.svecImg = svecImg;
    }
}
