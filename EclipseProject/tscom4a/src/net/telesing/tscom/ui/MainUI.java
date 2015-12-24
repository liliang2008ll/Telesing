package net.telesing.tscom.ui;

import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.*;
import net.telesing.tscom.R;
import net.telesing.tscom.SvecApplication;
import net.telesing.tscom.common.contants.ConstantHandler;
import net.telesing.tscom.common.utils.CommUtils;
import net.telesing.tscom.common.utils.YCCookie;
import net.telesing.tscom.manager.FloatWindowManager;
import net.telesing.tscom.manager.ServiceManager;
import net.telesing.tscom.receiver.SNReceiver;
import net.telesing.tscom.receiver.SeniorReceiver;
import net.telesing.tsdk.tlib.bean.SNBean;
import net.telesing.tsdk.tlib.core.AcommsInterface;
import net.telesing.tsdk.tlib.core.impl.AcommsImpl;


public class MainUI extends BaseUI {

    private Button aboutBtn;
    private RelativeLayout signalLayout;//信号强度背景
    private LinearLayout stateLayout;//麦克风异常，收到新消息  显示5秒后消失
    private ImageView stateImg;//麦克风异常，收到新消息 图标
    private TextView wareTxt;//异常内容
    private TextView signalTxt;//信号强度数值
    private TextView stateTxt;//当前开启状态
    private TextView msgTxt;//收到消息提示内容

    private Button switchBtn,startUpBtn,ballBtn,onlyWinsBtn;

    private int currentSignal = 0;
    private SNBean proSvecBean;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_ui_layout);
        initWidget();
        registerSnReceiver();
        if(!SvecApplication.getInstance().isNotification()){
            SvecApplication.getInstance().showNotification(getApplicationContext());
        }
    }

    private void initWidget(){
        aboutBtn = (Button)findViewById(R.id.about_btn);

        signalLayout = (RelativeLayout)findViewById(R.id.signal_layout);
        stateLayout = (LinearLayout)findViewById(R.id.state_layout);
        stateImg = (ImageView)findViewById(R.id.state_img);
        wareTxt = (TextView)findViewById(R.id.ware_txt);
        signalTxt = (TextView)findViewById(R.id.signal_txt);
        stateTxt = (TextView)findViewById(R.id.state_txt);
        msgTxt = (TextView)findViewById(R.id.msg_txt);

        switchBtn = (Button)findViewById(R.id.switch_btn);
        startUpBtn = (Button)findViewById(R.id.startUp_btn);
        ballBtn = (Button)findViewById(R.id.ball_btn);
        onlyWinsBtn = (Button)findViewById(R.id.onlyWins_btn);

        aboutBtn.setOnClickListener(new ClickListener());
        switchBtn.setOnClickListener(new ClickListener());
        startUpBtn.setOnClickListener(new ClickListener());
        ballBtn.setOnClickListener(new ClickListener());
        onlyWinsBtn.setOnClickListener(new ClickListener());
        stateTxt.setOnClickListener(new ClickListener());

        initDatabase(0);
        if(ycCookie.getBoolean(YCCookie.WIN_OPEN)){
            ServiceManager.startFloatService(getApplicationContext());
        }
    }

    private void initDatabase(int type){
        startUpBtn.setBackgroundResource(ycCookie.getBoolean(YCCookie.OPEN_RUN)?R.drawable.start_up_open:R.drawable.start_up_close);
        ballBtn.setBackgroundResource(ycCookie.getBoolean(YCCookie.WIN_OPEN)?R.drawable.ball_open:R.drawable.ball_close);
        onlyWinsBtn.setBackgroundResource(ycCookie.getBoolean(YCCookie.DESTTOP_OPEN)?R.drawable.only_wind_open:R.drawable.only_wind_close);
        if(SvecApplication.getInstance().isStartService()){
            switchBtn.setBackgroundResource(R.drawable.sense_switch_open);
            stateLayout.setVisibility(View.GONE);
            signalTxt.setVisibility(View.VISIBLE);
            msgTxt.setText(null);
            msgTxt.setVisibility(View.VISIBLE);
            stateTxt.setText(R.string.started);
            stateTxt.setBackgroundResource(R.drawable.edit_input_normal);
            stateTxt.setTextColor(getResources().getColor(R.color.white));
            changeSignal();

        }else {
            switchBtn.setBackgroundResource(R.drawable.sense_switch_close);
            stateLayout.setVisibility(View.GONE);
            signalTxt.setVisibility(View.GONE);
            msgTxt.setVisibility(View.GONE);
            stateTxt.setText(R.string.no_start);
            stateTxt.setBackgroundResource(R.drawable.edit_input_focused);
            stateTxt.setTextColor(getResources().getColor(R.color.gray));
            signalLayout.setBackgroundResource(R.drawable.signal_0);
        }
        if(type == 1){
            SvecApplication.getInstance().setImage(null,1);
        }


    }

    private void changeSignal(int curSignal){
        int res = R.drawable.signal_0;
        if(curSignal > 0 && curSignal < 20){
            res = R.drawable.signal_1;
        }else if(curSignal >= 20 && curSignal < 40){
            res = R.drawable.signal_2;
        }else if(curSignal >= 40 && curSignal < 60){
            res = R.drawable.signal_3;
        }else if(curSignal >= 60 && curSignal < 80){
            res = R.drawable.signal_4;
        }else if(curSignal >= 80 && curSignal < 100){
            res = R.drawable.signal_5;
        }
        signalLayout.setBackgroundResource(res);
        signalTxt.setText(String.valueOf(curSignal));
    }

    private void operService(){
        if("close".equals(switchBtn.getTag().toString())){
            switchBtn.setTag("open");
            SvecApplication.getInstance().setStartService(true);
        }else if("open".equals(switchBtn.getTag().toString())){
            switchBtn.setTag("close");
            SvecApplication.getInstance().setStartService(false);
            msgTxt.setVisibility(View.GONE);
            stateLayout.setVisibility(View.GONE);
        }
        if(SvecApplication.getInstance().isStartService()){
            ServiceManager.startFloatService(getApplicationContext());
            ServiceManager.startVoice(getApplicationContext());
        }else{
            ServiceManager.closeVoice(getApplicationContext());
        }
        initDatabase(1);
    }

    private void setYcc(String name,boolean value){
        ycCookie.putBoolean(name,value);
    }

    private class ClickListener implements View.OnClickListener{
        @Override
        public void onClick(View v) {
            switch (v.getId()){
                case R.id.about_btn:
                    Intent intent = new Intent(MainUI.this,AboutUI.class);
                    startActivity(intent);
                    break;
                case R.id.switch_btn:
                    operService();
                    break;
                case R.id.startUp_btn:
                    setYcc(YCCookie.OPEN_RUN,ycCookie.getBoolean(YCCookie.OPEN_RUN)?false:true);
                    initDatabase(1);
                    break;
                case R.id.ball_btn:
                    setYcc(YCCookie.WIN_OPEN,ycCookie.getBoolean(YCCookie.WIN_OPEN)?false:true);
                    if(ycCookie.getBoolean(YCCookie.WIN_OPEN)){
                        ServiceManager.startFloatService(getApplicationContext());
                    }else {
                        FloatWindowManager.removeSmallWindow(getApplicationContext());
                        ServiceManager.stopFloatService(getApplicationContext());
                    }
                    initDatabase(1);
                    break;
                case R.id.onlyWins_btn:
                    setYcc(YCCookie.DESTTOP_OPEN,ycCookie.getBoolean(YCCookie.DESTTOP_OPEN)?false:true);
                    if(ycCookie.getBoolean(YCCookie.DESTTOP_OPEN)){
                        FloatWindowManager.createSmallWindow(getApplicationContext(),SvecApplication.getInstance().isStartService());
                    }else {
                        FloatWindowManager.removeSmallWindow(getApplicationContext());
                    }
                    initDatabase(1);
                    break;
                case R.id.state_txt:
                    if("detail".equals(stateTxt.getTag().toString())){
                        Intent dIntent = new Intent(MainUI.this,DetailUI.class);
                        startActivity(dIntent);
                    }
                    break;
            }
        }
    }

    private Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            String txt = "";
            switch (msg.what){
                case ConstantHandler.SIGNAL_CHANGE:
                    if(SvecApplication.getInstance().isStartService()){
                        changeSignal(Integer.parseInt(msg.obj.toString()));
                        if(proSvecBean != null){
                            txt = CommUtils.longToTime(proSvecBean.getRec_time())+"收到"+proSvecBean.getOrgName()+"的消息";
                            msgTxt.setText(txt);
                        }
                        stateLayout.setVisibility(View.GONE);
                        wareTxt.setVisibility(View.VISIBLE);
                        signalTxt.setVisibility(View.VISIBLE);
                        if(msg.arg2 == 13){
                            signalLayout.setBackgroundResource(R.drawable.signal_0);
                            signalTxt.setVisibility(View.GONE);
                            stateLayout.setVisibility(View.VISIBLE);
                            wareTxt.setText(R.string.mkf_str);
                            stateImg.setBackgroundResource(R.drawable.error_icon);
                            stateTxt.setText(R.string.detail_str);
                            stateTxt.setTag("detail");


                        }else{
                            stateTxt.setText(R.string.started);
                            stateTxt.setTag("stated");
                        }
                    }

                    break;
                case ConstantHandler.RESOLUTION_END:
                    SNBean svecResBean = (SNBean)msg.obj;
                    if(svecResBean != null){
                        FloatWindowManager.updateUsedPercent(2);
                        stateLayout.setVisibility(View.VISIBLE);
                        wareTxt.setVisibility(View.GONE);
                        stateImg.setBackgroundResource(R.drawable.new_msg_icon);
                        signalTxt.setVisibility(View.GONE);
                        if(!"0".equals(svecResBean.getData())){
                            proSvecBean = svecResBean;
                            txt = CommUtils.longToTime(svecResBean.getRec_time())+"收到"+svecResBean.getOrgName()+"的消息";
                            msgTxt.setText(txt);
                            SvecApplication.getInstance().setImage(txt,1);
                        }
                    }

                    break;
            }
        }
    };

    private void changeSignal(){
        new Thread(new Runnable() {
            @Override
            public void run() {
                try{
                    int i = 1;
                    while (SvecApplication.getInstance().isStartService()){
                        //每隔1分钟获取信号强度 并显示信号强度
                        Message msg = new Message();
                        msg.what = ConstantHandler.SIGNAL_CHANGE;
                        int sin = (int)(AcommsInterface.getSignal()*100);
                        Log.e("sig",sin+"");
                        msg.obj = sin;
                        if(i == 1){
                            handler.sendMessage(msg);
                        }
                        Thread.sleep(5000);
                        msg.arg2 = AcommsInterface.state();
                       if(i != 1){
                           handler.sendMessage(msg);
                       }
                        i++;
                    }
                }catch (Exception e){

                }

            }
        }).start();
    }


    public void registerSnReceiver() {
        if (snReceiver == null) {
            snReceiver = new SNReceiver() {
                @Override
                protected void doWhat(SNBean snBean) {

                    super.doWhat(snBean);
                    if (SvecApplication.getInstance().isStartService()) {
                        Message msg = new Message();
                        msg.what = ConstantHandler.RESOLUTION_END;
                        msg.obj = snBean;
                        handler.sendMessage(msg);
                    }
                }
            };
            IntentFilter snFilter = new IntentFilter(AcommsInterface.HAVE_SN_INTENT);
            registerReceiver(snReceiver, snFilter);
        }
        registerSeniorReceiver();
    }


    private SeniorReceiver seniorReceiver;

    private void registerSeniorReceiver() {
        if (seniorReceiver == null) {
            seniorReceiver = new SeniorReceiver() {
                @Override
                protected void doWhat(int type) {

                    super.doWhat(type);
                    if(type == 5){
                       initDatabase(0);
                    }else if(type == 15){
                        initDatabase(1);
                    }
                }
            };
            IntentFilter snFilter = new IntentFilter(SeniorReceiver.SENIORRECEIVER_ACTION);
            registerReceiver(seniorReceiver, snFilter);
        }
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(snReceiver != null)
            unregisterReceiver(snReceiver);
    }
}
