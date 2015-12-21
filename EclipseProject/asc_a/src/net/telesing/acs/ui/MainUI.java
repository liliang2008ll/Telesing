package net.telesing.acs.ui;

import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.*;
import net.telesing.acs.R;
import net.telesing.acs.SvecApplication;
import net.telesing.acs.common.contants.ConstantHandler;
import net.telesing.acs.common.utils.YCCookie;
import net.telesing.acs.manager.FloatWindowManager;
import net.telesing.acs.manager.ServiceManager;
import net.telesing.acs.receiver.SNReceiver;
import net.telesing.acs.receiver.SeniorReceiver;
import net.telesing.scomm.android.DemInterface;
import net.telesing.scomm.android.SvecResBean;
import net.telesing.scomm.android.core.Demodulator;

import java.util.Random;


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
    private SvecResBean proSvecBean;


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
            stateTxt.setText(R.string.started);
            stateTxt.setBackgroundResource(R.drawable.edit_input_normal);
            stateTxt.setTextColor(getResources().getColor(R.color.white));
        }else {
            switchBtn.setBackgroundResource(R.drawable.sense_switch_close);
            stateLayout.setVisibility(View.GONE);
            signalTxt.setVisibility(View.GONE);
            stateTxt.setText(R.string.no_start);
            stateTxt.setBackgroundResource(R.drawable.edit_input_focused);
            stateTxt.setTextColor(getResources().getColor(R.color.gray));
            signalLayout.setBackgroundResource(R.drawable.signal_0);
        }
        if(type == 1){
            SvecApplication.getInstance().setImage(null,1);
        }
        changeSignal();

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
            }
        }
    }

    private Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what){
                case ConstantHandler.SIGNAL_CHANGE:
                    changeSignal(Integer.parseInt(msg.obj.toString()));
                    if(proSvecBean != null){
                        if(!"0".equals(proSvecBean.getBeaconSn())){
                            msgTxt.setText("收到"+proSvecBean.getOrgName()+"的消息");
                        }
                    }
                    stateLayout.setVisibility(View.GONE);
                    wareTxt.setVisibility(View.VISIBLE);
                    signalTxt.setVisibility(View.VISIBLE);
                    break;
                case ConstantHandler.RESOLUTION_END:
                    FloatWindowManager.updateUsedPercent(2);
                    SvecResBean svecResBean = (SvecResBean)msg.obj;
                    stateLayout.setVisibility(View.VISIBLE);
                    wareTxt.setVisibility(View.GONE);
                    stateImg.setBackgroundResource(R.drawable.new_msg_icon);
                    signalTxt.setVisibility(View.GONE);
                    if(!"0".equals(svecResBean.getBeaconSn())){
                        proSvecBean = svecResBean;
                        msgTxt.setText("收到"+svecResBean.getOrgName()+"的消息");
                        SvecApplication.getInstance().setImage("收到"+svecResBean.getOrgName()+"的消息",1);
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
                    while (SvecApplication.getInstance().isStartService()){
                        //每隔1分钟获取信号强度 并显示信号强度
                        Message msg = new Message();
                        msg.what = ConstantHandler.SIGNAL_CHANGE;
                        msg.obj = String.valueOf((int)(Demodulator.getSignal()*100));
                        handler.sendMessage(msg);
                        Thread.sleep(5000);
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
                protected void doWhat(SvecResBean svecResBean) {

                    super.doWhat(svecResBean);
                    if (SvecApplication.getInstance().isStartService()) {
                        Message msg = new Message();
                        msg.what = ConstantHandler.RESOLUTION_END;
                        msg.obj = svecResBean;
                        handler.sendMessage(msg);
                    }
                }
            };
            IntentFilter snFilter = new IntentFilter(DemInterface.HAVE_SN_INTENT);
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
                        operService();
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
