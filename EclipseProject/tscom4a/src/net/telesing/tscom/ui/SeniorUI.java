package net.telesing.tscom.ui;

import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.*;
import net.telesing.tscom.R;
import net.telesing.tscom.adapter.MsgAdapter;
import net.telesing.tscom.bean.SN;

import net.telesing.tscom.common.contants.ConstantHandler;
import net.telesing.tscom.common.utils.CommUtils;
import net.telesing.tscom.common.utils.DialogUtil;
import net.telesing.tscom.common.utils.DisplayUtil;
import net.telesing.tscom.db.DBUtil;
import net.telesing.tscom.manager.FloatWindowManager;
import net.telesing.tscom.receiver.SNReceiver;
import net.telesing.tscom.view.draw.Line;
import net.telesing.tscom.view.draw.LineGraph;
import net.telesing.tscom.view.draw.LinePoint;
import net.telesing.tsdk.tlib.bean.SNBean;
import net.telesing.tsdk.tlib.core.AcommsInterface;
import net.telesing.tsdk.tlib.db.Cookie;

import java.util.*;

/**
 * Created by Administrator on 2015/11/23.
 */
public class SeniorUI extends BaseUI {

    private Button startBtn, resetBtn;
    private TextView resTxt;
    private ListView showList;
    private RelativeLayout settingLayout;
    private LineGraph lineGraph;

    private List<String> sucList = new ArrayList<String>();
    private MsgAdapter msgAdapter;
    private boolean isStart = false;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.senior_layout);
        initWidget();
        registerSnReceiver();
    }

    private void initWidget() {
        backBtn = (Button) findViewById(R.id.back_btn);
        saveBtn = (Button) findViewById(R.id.save_btn);
        startBtn = (Button) findViewById(R.id.start_btn);
        resetBtn = (Button) findViewById(R.id.reset_btn);
        resTxt = (TextView) findViewById(R.id.res_title);
        showList = (ListView) findViewById(R.id.msg_show_list);
        settingLayout = (RelativeLayout) findViewById(R.id.setting_layout);

        lineGraph = (LineGraph)findViewById(R.id.line_chart);

        showList.setAdapter(getMsgAdapter(null));
        backBtn.setOnClickListener(new ClickListener());
        saveBtn.setOnClickListener(new ClickListener());
        startBtn.setOnClickListener(new ClickListener());
        resetBtn.setOnClickListener(new ClickListener());
        settingLayout.setOnClickListener(new ClickListener());
        lineGraph.setOnClickListener(new ClickListener());
        setBtnDrawable(isStart);

    }





    private class ClickListener implements View.OnClickListener {
        @Override
        public void onClick(View v) {
            Intent intent = null;
            switch (v.getId()) {
                case R.id.line_chart:
                    if(isStart){
                        intent = new Intent(SeniorUI.this,SignalChartUI.class);
                        startActivity(intent);
                    }
                    break;
                case R.id.back_btn:
                    SeniorUI.this.finish();
                    break;
                case R.id.save_btn:
                    intent = new Intent(SeniorUI.this, SettingUI.class);
                    startActivity(intent);
                    break;
                case R.id.start_btn:
                    if (!isStart) {
                        List<SN> sns = DBUtil.findAllSN(sdb, dbHelper);
                        if (sns != null && sns.size() > 0) {
                            isStart = true;
//                            startTxt.setText(R.string.pause_str);
//                            startBtn.setBackgroundResource(R.drawable.msg_pause);
                            setBtnDrawable(isStart);

                        } else {
                            DialogUtil.showOperDialog(getApplicationContext(), handler, "您还没有加载声码列表\n请点击空白箭头区域->重载->保存加载声码列表", 9, ConstantHandler.NO_SETTING_SNS_LIST);
                        }

                    } else {
//                        startTxt.setText(R.string.start_str);
//                        startBtn.setBackgroundResource(R.drawable.msg_start);
                        isStart = false;
                        setBtnDrawable(isStart);
                    }
                    operTimer(isStart);
                    break;
                case R.id.reset_btn:
                    //清空数据库中的所有数据
                    if (!isStart) {
                        sucList = new ArrayList<String>();
                        showList.setAdapter(getMsgAdapter(null));
                        resTxt.setText(null);
                    } else {
                        DialogUtil.showToast(getApplicationContext(), "请先停止，再复位。", Toast.LENGTH_SHORT);
                    }

                    break;
                case R.id.setting_layout:
                    intent = new Intent(SeniorUI.this, EditSnUI.class);
                    startActivity(intent);
                    break;
            }
        }
    }

    private void setBtnDrawable(boolean isStart){
        int drawId = R.drawable.start_msg;

        int txtId = R.string.start_str;
        if(isStart){
            drawId = R.drawable.pause_msg;
            txtId = R.string.pause_str;
        }else{
            drawId = R.drawable.start_msg;
            txtId = R.string.start_str;
        }
        Drawable drawable =  getResources().getDrawable(drawId);
        drawable.setBounds(0, 0, DisplayUtil.getPx(SeniorUI.this,36), DisplayUtil.getPx(SeniorUI.this,36));
        startBtn.setCompoundDrawables(null, drawable, null, null);
        startBtn.setText(txtId);
        setResetBtnDrawable();
    }

    private void setResetBtnDrawable(){

        Drawable drawable =  getResources().getDrawable(R.drawable.reset_msg);
        drawable.setBounds(0, 0, DisplayUtil.getPx(SeniorUI.this,36), DisplayUtil.getPx(SeniorUI.this,36));
        resetBtn.setCompoundDrawables(null, drawable, null, null);
        resetBtn.setText(R.string.reset_str);
    }

    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
                case ConstantHandler.RESOLUTION_END:
                    msgAdapter.addInfo(checkSn((SNBean) msg.obj, 1));
                    String text = "正确率：" + (int)((sucList.size()*1.0 / msgAdapter.getInfoList().size()) * 100) + "%（" + (msgAdapter.getInfoList().size() - sucList.size()) + "/" + sucList.size() + "/" + msgAdapter.getInfoList().size() + "）\n平均耗时" + getSnAvTime() + "秒";
                    resTxt.setText(text);
                    FloatWindowManager.updateUsedPercent(2);
                    break;
                case ConstantHandler.SHOW_LINE_CHART:
                    showLineChart();
                    break;
            }
        }
    };

    private SNBean checkSn(SNBean svecResBean, int type) {
        //判断当前的声码是否在数据库中存在
        if (svecResBean != null && svecResBean.getSn() != null) {
            svecResBean.setRes("N");
            if (DBUtil.checkSn(svecResBean.getSn(), sdb, dbHelper)) {
                if (svecResBean.isGood()) {//成功
                    svecResBean.setColor(R.color.s_success);
                    if (svecResBean.isHave()) {
                        svecResBean.setRes("R");
                    }
                    svecResBean.setResult("成功");
                    sucList.add(svecResBean.getSn());

                } else {//弱对
                    svecResBean.setColor(R.color.s_fail);
                    svecResBean.setResult("弱对");
                }
            } else {
                if (svecResBean.isGood()) {//强错
                    svecResBean.setColor(R.color.f_success);
                    svecResBean.setResult("强错");
                } else {//弱错
                    svecResBean.setColor(R.color.f_fail);
                    svecResBean.setResult("弱错");
                }
            }
        }

        //
        return svecResBean;
    }


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
                if(isStart){
                    handler.postDelayed(this, 1000);
                    showLineChart();
                }

            } catch (Exception e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
                System.out.println("exception...");
            }
        }
    };


    private void showLineChart(){
        if(isStart){
            Line l = new Line();
            LinePoint p = new LinePoint();
            for (int i = 0; i < 300; i++) {
                p = new LinePoint();
                p.setX(i);
                p.setY(getFloat());
                l.addPoint(p);
            }
            l.setColor(getResources().getColor(R.color.white));
            l.setShowingPoints(false);
            lineGraph.removeAllLines();
            lineGraph.addLine(l);
            lineGraph.setRangeY(0, (float)((cookie.getStDouble(Cookie.DEM_ST)*10*2.5/1.5)));
            lineGraph.setLineToFill(1);
        }else{
            lineGraph.removeAllLines();
        }


    }

    public float getFloat(){
        float res = (float)(AcommsInterface.getSignal()*10);
        return res;
    }

    private MsgAdapter getMsgAdapter(final List<SNBean> infoList) {
        msgAdapter = new MsgAdapter(getApplicationContext(), infoList);
        msgAdapter.notifyDataSetChanged();
        return msgAdapter;
    }


    private String getSnAvTime() {
        if (msgAdapter != null) {
            int count = 0;
            if (msgAdapter.getInfoList() != null && msgAdapter.getInfoList().size() > 0) {
                long time = 0;
                for (int i = 0; i < msgAdapter.getInfoList().size(); i++) {
                    SNBean sb = msgAdapter.getInfoList().get(i);
                    if ("成功".equals(sb.getResult())) {
                        time += sb.getTimes();
                        count++;
                    }
                }
                if (time != 0 && count != 0) {
                    return CommUtils.getDiff(time / count);
                }
                return "0";
            }
            return "0";
        }
        return "0";
    }


    public void registerSnReceiver() {
        if (snReceiver == null) {
            snReceiver = new SNReceiver() {
                @Override
                protected void doWhat(SNBean svecResBean) {

                    super.doWhat(svecResBean);
                    if (isStart) {
                        Message msg = new Message();
                        msg.what = ConstantHandler.RESOLUTION_END;
                        msg.obj = svecResBean;
                        handler.sendMessage(msg);
                    }
                }
            };
            IntentFilter snFilter = new IntentFilter(AcommsInterface.HAVE_SN_INTENT);
            registerReceiver(snReceiver, snFilter);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (snReceiver != null)
            unregisterReceiver(snReceiver);
    }

}
