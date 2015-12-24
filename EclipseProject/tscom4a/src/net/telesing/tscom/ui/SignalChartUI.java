package net.telesing.tscom.ui;

import android.os.Bundle;
import android.os.Handler;
import net.telesing.tscom.R;
import net.telesing.tscom.view.draw.Line;
import net.telesing.tscom.view.draw.LineGraph;
import net.telesing.tscom.view.draw.LinePoint;
import net.telesing.tscom.view.draw.LineYGraph;
import net.telesing.tsdk.tlib.core.AcommsInterface;
import net.telesing.tsdk.tlib.db.Cookie;

import java.util.Random;

/**
 * Created by Administrator on 2015/12/17.
 */
public class SignalChartUI extends BaseUI {
    private LineYGraph lineGraph;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.signal_layout);
        lineGraph = (LineYGraph)findViewById(R.id.line_chart);
    }
    @Override
    protected void onResume() {
        /**
         * 设置为横屏
         */
//        if(getRequestedOrientation()!=ActivityInfo.SCREEN_ORIENTATION_PORTRAIT){
//            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
//        }
        super.onResume();
        operTimer(true);
//        showLineChart();
    }


    private Handler handler = new Handler();

    private void operTimer(boolean isShow){
        if(isShow){
            handler.postDelayed(runnable,1000);
        }else{
            showLineChart();
        }


    }

    Runnable runnable = new Runnable() {

        @Override
        public void run() {
            // handler自带方法实现定时器
            try {
                handler.postDelayed(this, 1000);
                showLineChart();

            } catch (Exception e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
                System.out.println("exception...");
            }
        }
    };


    private void showLineChart(){
        Line l = new Line();
        LinePoint p = new LinePoint();
        for (int i = 0; i <600; i++) {
            p = new LinePoint();
//            p.setX(getFloat());
            p.setY((float)(AcommsInterface.getSignal()*100));
            p.setY(i);
            l.addPoint(p);
        }
        l.setColor(getResources().getColor(R.color.white));
        l.setShowingPoints(false);
        lineGraph.removeAllLines();
        lineGraph.addLine(l);
        lineGraph.setRangeY(0, (float)((cookie.getStDouble(Cookie.DEM_ST)*10*2.5/1.5)));
        lineGraph.setLineToFill(1);


    }

    public float getFloat(){
        float res = new Random().nextFloat();
        if(res < 0.2f && res >0.13f){
            return res*100;
        }else {
            return 15f;
        }
    }
}
