package net.telesing.acs.view;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import net.telesing.acs.R;
import net.telesing.acs.common.contants.ConstantHandler;


public class LoginDialog extends Dialog {
    private String title;
    private int titleRes;
    private int type; //0:普通提示 1：输入密码
    private int handlerWhat;


    private TextView titleTxt;
    private Button okBtn,cancelBtn;
    private LinearLayout pwdBtnLayout;
    private Context context;
    private Handler handler;
    private View view;

    public LoginDialog(Context context, Handler handler, int title, int type, int handlerWhat){
        super(context);
        this.type = type;
        this.handlerWhat = handlerWhat;
        this.context = context;
        this.handler = handler;
        this.titleRes = title;
    }
    public LoginDialog(Context context, Handler handler, String title, int type, int handlerWhat){
        super(context);
        this.type = type;
        this.handlerWhat = handlerWhat;
        this.context = context;
        this.handler = handler;
        this.title = title;
    }

    public LoginDialog(Context context){
        super(context);

    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.alert_dialog_layout);
        this.setCancelable(false);
        initWidget();
    }
    private void initWidget() {
        titleTxt = (TextView)findViewById(R.id.title_txt);
        okBtn = (Button)findViewById(R.id.ok_btn);
        cancelBtn = (Button)findViewById(R.id.cancel_btn);
        pwdBtnLayout = (LinearLayout)findViewById(R.id.pwd_btn_layout);
        view = findViewById(R.id.view_line);
        if(title == null){
            titleTxt.setText(titleRes);
        }else {
            titleTxt.setText(title);
        }
        okBtn.setOnClickListener(new ClickListener());
        cancelBtn.setOnClickListener(new ClickListener());
        if(type == 9){
            cancelBtn.setVisibility(View.GONE);
            view.setVisibility(View.GONE);
        }
        if(type == 20){
            okBtn.setText(R.string.save_str);
        }


    }

    private class ClickListener implements View.OnClickListener{
        @Override
        public void onClick(View v) {
            switch (v.getId()){
                case R.id.ok_btn:
                    if(handler != null  && handlerWhat != 0){
                        handler.sendEmptyMessage(handlerWhat);
                    }
                    break;
                case R.id.cancel_btn:
                    handler.sendEmptyMessage(ConstantHandler.CANCEL);
                    break;
            }
            LoginDialog.this.dismiss();
        }
    }

}
