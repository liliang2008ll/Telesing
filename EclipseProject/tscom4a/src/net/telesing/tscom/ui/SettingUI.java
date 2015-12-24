package net.telesing.tscom.ui;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import net.telesing.tscom.R;
import net.telesing.tscom.common.contants.ConstantHandler;
import net.telesing.tscom.common.utils.CommUtils;
import net.telesing.tscom.common.utils.DialogUtil;
import net.telesing.tscom.view.ClearEditText;
import net.telesing.tsdk.tlib.db.Cookie;


public class SettingUI extends BaseUI {
    private ClearEditText fhBumEdt, xhqzEdt, tbjgEdt, jxcsEdt, jxjgEdt, sfeqEdt, efeqEdt,fwqEdt,hcEdt,bydzEdt;
    private boolean isSave = false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.setting_layout);
        initWidget();
    }

    private void initWidget(){
        fhBumEdt = (ClearEditText)findViewById(R.id.fhcd_edt);
        xhqzEdt = (ClearEditText) findViewById(R.id.xh_qz_edt);
        tbjgEdt = (ClearEditText) findViewById(R.id.tb_jg_edt);
        jxcsEdt = (ClearEditText) findViewById(R.id.jx_cs_edt);
        jxjgEdt = (ClearEditText) findViewById(R.id.jx_jg_edt);
        sfeqEdt = (ClearEditText) findViewById(R.id.s_feq_edt);
        efeqEdt = (ClearEditText) findViewById(R.id.e_feq_edt);
        hcEdt = (ClearEditText)findViewById(R.id.hc_sj_edt);
        fwqEdt = (ClearEditText)findViewById(R.id.fwq_dz_edt);
        bydzEdt = (ClearEditText)findViewById(R.id.by_dz_edt);
        backBtn = (Button)findViewById(R.id.back_btn);
        saveBtn = (Button)findViewById(R.id.save_btn);
        backBtn.setOnClickListener(new ClickListener());
        saveBtn.setOnClickListener(new ClickListener());
        saveBtn.setBackground(null);
        saveBtn.setText(R.string.save_str);
        initDatabase();
    }


    public void initDatabase(){
        fwqEdt.setText(cookie.getZhuFString(Cookie.QUERY_MSG_BY_SN_URL));
        bydzEdt.setText(cookie.getBfString(Cookie.BY_QUERY_MSG_BY_SN_URL));
        fhBumEdt.setText(String.valueOf(cookie.getNumInteger(Cookie.DEM_N)));
        xhqzEdt.setText(String.valueOf(cookie.getStDouble(Cookie.DEM_ST)));
        tbjgEdt.setText(String.valueOf(cookie.getGapInteger(Cookie.DEM_GAP)));
        sfeqEdt.setText(String.valueOf(cookie.getfeq(Cookie.DEM_SFEQ,16*1000)));
        efeqEdt.setText(String.valueOf(cookie.getfeq(Cookie.DEM_EFEQ,20*1000)));
        hcEdt.setText(cookie.getTime(Cookie.CACHE_CLEAR_MS));

    }

    private class ClickListener implements View.OnClickListener{
        @Override
        public void onClick(View v) {
            switch (v.getId()){
                case R.id.save_btn:
                    settingParam();
                    break;
                case R.id.back_btn:
                    goBack();
                    break;
            }
        }
    }

    public void goBack(){
        if(isSave){
            SettingUI.this.finish();
        }else {
            DialogUtil.showOperDialog(getApplicationContext(), handler, "是否保存设置后的参数!", 20, ConstantHandler.SAVE_SETTING);
        }
    }

    private void settingParam(){
        String fwq = fwqEdt.getText().toString();
        String byfwq = bydzEdt.getText().toString();
        String xhcd = fhBumEdt.getText().toString();
        String xhqz = xhqzEdt.getText().toString();
        String tbjg = tbjgEdt.getText().toString();
        String hcsj = hcEdt.getText().toString();
        String sfeq = sfeqEdt.getText().toString();
        String efeq = efeqEdt.getText().toString();
        if(CommUtils.isNull(fwq) && CommUtils.isNull(byfwq) && CommUtils.isNull(xhcd) && CommUtils.isNull(xhqz) && CommUtils.isNull(tbjg) && CommUtils.isNull(hcsj)&& CommUtils.isNull(sfeq)&& CommUtils.isNull(efeq)){
            cookie.putString(Cookie.QUERY_MSG_BY_SN_URL,fwq);
            cookie.putString(Cookie.BY_QUERY_MSG_BY_SN_URL, byfwq);
            cookie.putInteger(Cookie.DEM_N, Integer.parseInt(xhcd));
            cookie.putString(Cookie.DEM_ST, xhqz);
            cookie.putInteger(Cookie.DEM_GAP, Integer.parseInt(tbjg));
            cookie.putString(Cookie.CACHE_CLEAR_MS, hcsj);
            cookie.putInteger(Cookie.DEM_SFEQ,Integer.parseInt(sfeq));
            cookie.putInteger(Cookie.DEM_EFEQ, Integer.parseInt(efeq));
            isSave = true;
            DialogUtil.showOperDialog(getApplicationContext(), handler, "参数设置成功，需检测开关重启后方可生效。是否现在重启?", 1, ConstantHandler.REST_CHECK);
        }else {
            DialogUtil.showToast(getApplicationContext(),"您有未填写项，请输入全部填写", Toast.LENGTH_SHORT);
            return;
        }

    }





    private Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what){

                case ConstantHandler.REST_CHECK:

                    //重启服务后关闭当前页面

                   SettingUI.this.finish();
                    break;
                case ConstantHandler.SAVE_SETTING:
                    settingParam();
                    break;
                case ConstantHandler.CANCEL:
                    if(!isSave){
                        SettingUI.this.finish();
                    }

                    break;
            }
        }
    };

}
