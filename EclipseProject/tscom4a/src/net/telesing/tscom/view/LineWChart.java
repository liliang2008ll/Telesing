package net.telesing.tscom.view;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.widget.ImageView;

/**
 * Created by Administrator on 2015/12/14.
 */
public class LineWChart extends ImageView {

    public LineWChart(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public LineWChart(Context context){
        this(context, null);
    }

    /**
     * 获得我自定义的样式属性
     *
     * @param context
     * @param attrs
     * @param defStyle
     */
    public LineWChart(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

        @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
    }


}
