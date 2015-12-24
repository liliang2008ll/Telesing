package net.telesing.tscom.ui;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import net.telesing.tscom.R;
import net.telesing.tscom.common.contants.ConstantHandler;
import net.telesing.tscom.common.utils.CommUtils;
import net.telesing.tscom.common.utils.DialogUtil;
import net.telesing.tscom.common.utils.DisplayUtil;

/**
 * Created by Administrator on 2015/11/20.
 */
public class AboutUI extends BaseUI {

    private TextView versionTxt,itemTxt_1,itemTxt_2,itemTxt_3,itemTxt_4,itemTxt_5,itemTxt_6,itemTxt_7,itemTxt_8,itemTxt_9,contentTxt;
    private String pwdRes = "";
    private LinearLayout aboutLayout;
    private boolean isOpenning = false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.about_layout);
        initWidget();
    }

    private void initWidget(){
        try {
            backBtn = (Button)findViewById(R.id.back_btn);
            saveBtn = (Button)findViewById(R.id.save_btn);
            versionTxt = (TextView)findViewById(R.id.version_txt);
            itemTxt_1 = (TextView)findViewById(R.id.xuanxiang_1);
            itemTxt_2 = (TextView)findViewById(R.id.xuanxiang_2);
            itemTxt_3 = (TextView)findViewById(R.id.xuanxiang_3);
            itemTxt_4 = (TextView)findViewById(R.id.xuanxiang_4);
            itemTxt_5 = (TextView)findViewById(R.id.xuanxiang_5);
            itemTxt_6 = (TextView)findViewById(R.id.xuanxiang_6);
            itemTxt_7 = (TextView)findViewById(R.id.xuanxiang_7);
            itemTxt_8 = (TextView)findViewById(R.id.xuanxiang_8);
            itemTxt_9 = (TextView)findViewById(R.id.xuanxiang_9);
            contentTxt = (TextView)findViewById(R.id.content_txt);
            aboutLayout = (LinearLayout)findViewById(R.id.about_layout);
            aboutLayout.setOnClickListener(new ClickListener());
            backBtn.setOnClickListener(new ClickListener());
            itemTxt_1.setOnClickListener(new ClickListener());
            itemTxt_2.setOnClickListener(new ClickListener());
            itemTxt_3.setOnClickListener(new ClickListener());
            itemTxt_4.setOnClickListener(new ClickListener());
            itemTxt_5.setOnClickListener(new ClickListener());
            itemTxt_6.setOnClickListener(new ClickListener());
            itemTxt_7.setOnClickListener(new ClickListener());
            itemTxt_8.setOnClickListener(new ClickListener());
            itemTxt_9.setOnClickListener(new ClickListener());
            saveBtn.setVisibility(View.GONE);
            versionTxt.setText(DisplayUtil.getCurrentAppVersionName(getApplicationContext()));
        }catch (Exception e){

        }

    }

    private class ClickListener implements View.OnClickListener{
        @Override
        public void onClick(View v) {
            switch (v.getId()){
                case R.id.back_btn:
                    AboutUI.this.finish();
                    break;
                case R.id.about_layout:
                    if(isOpenning){//跳转到高级设置页面
                        Intent intent = new Intent(AboutUI.this,SeniorUI.class);
                        startActivity(intent);
                    }else{
                        setItemTxt(v.getTag().toString(),null);
                    }
                    break;
                default:
                    setItemTxt(v.getTag().toString(),v);
                    break;
            }


        }
    }

    private void setItemTxt(String tag,View view){
        if(CommUtils.isNull(tag)){
            int res = R.string.about_item_1_1;
            switch (Integer.parseInt(tag)){
                case 1:
                    res = R.string.about_item_1_1;
                    break;
                case 2:
                    res = R.string.about_item_2_1;
                    break;
                case 3:
                    res = R.string.about_item_3_1;
                    break;
                case 4:
                    res = R.string.about_item_4_1;
                    break;
                case 5:
                    res = R.string.about_item_5_1;
                    break;
                case 6:
                    res = R.string.about_item_6_1;
                    break;
                case 7:
                    res = R.string.about_item_7_1;
                    break;
                case 8:
                    res = R.string.about_item_8_1;
                    break;
                case 9:
                    res = R.string.about_item_9_1;
                    break;
            }
            setTextViewBgColor();
            if(view != null){
                view.setBackgroundResource(R.color.gray);
            }
            contentTxt.setText(res);
            pwdRes += tag;
            checkPwd();
        }
    }
    private void checkPwd() {
        long data = CommUtils.getNumber(versionTxt.getText().toString());
        if(pwdRes.equals(String.valueOf(data))){//显示高级选项
            DialogUtil.showOperDialog(getApplicationContext(),handler,R.string.open_senior_str,8, ConstantHandler.RESOLUTION_SUCCESS);
        }
    }
    private Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what){
                case  ConstantHandler.RESOLUTION_SUCCESS:
                    setTextViewColor();
                    break;
            }
        }
    };

    private void setTextViewColor(){
        isOpenning = true;
        itemTxt_1.setTextColor(getResources().getColor(R.color.txt_open_gj));
        itemTxt_2.setTextColor(getResources().getColor(R.color.txt_open_gj));
        itemTxt_3.setTextColor(getResources().getColor(R.color.txt_open_gj));
        itemTxt_4.setTextColor(getResources().getColor(R.color.txt_open_gj));
        itemTxt_5.setTextColor(getResources().getColor(R.color.txt_open_gj));
        itemTxt_6.setTextColor(getResources().getColor(R.color.txt_open_gj));
        itemTxt_7.setTextColor(getResources().getColor(R.color.txt_open_gj));
        itemTxt_8.setTextColor(getResources().getColor(R.color.txt_open_gj));
        itemTxt_9.setTextColor(getResources().getColor(R.color.txt_open_gj));
    }
    private void setTextViewBgColor(){
        itemTxt_1.setBackgroundResource(R.color.main_bottom);
        itemTxt_2.setBackgroundResource(R.color.main_bottom);
        itemTxt_3.setBackgroundResource(R.color.main_bottom);
        itemTxt_4.setBackgroundResource(R.color.main_bottom);
        itemTxt_5.setBackgroundResource(R.color.main_bottom);
        itemTxt_6.setBackgroundResource(R.color.main_bottom);
        itemTxt_7.setBackgroundResource(R.color.main_bottom);
        itemTxt_8.setBackgroundResource(R.color.main_bottom);
        itemTxt_9.setBackgroundResource(R.color.main_bottom);
    }
}
