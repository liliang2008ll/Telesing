package net.telesing.tscom.ui;

import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import net.telesing.tscom.R;
import net.telesing.tscom.bean.SN;
import net.telesing.tscom.common.contants.ConstantHandler;
import net.telesing.tscom.common.utils.CommUtils;
import net.telesing.tscom.common.utils.DialogUtil;
import net.telesing.tscom.db.DBUtil;

import java.util.List;

/**
 * Created by Administrator on 2015/11/23.
 */
public class EditSnUI extends BaseUI {

    private EditText snEdt;
    private Button clearBtn,reloadBtn;
    private boolean isSave = false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.edit_sn_layout);
        init();
    }

    private void init(){
        clearBtn = (Button)findViewById(R.id.clear_btn);
        snEdt = (EditText)findViewById(R.id.sn_edt);
        backBtn = (Button)findViewById(R.id.back_btn);
        saveBtn = (Button)findViewById(R.id.save_btn);
        reloadBtn = (Button)findViewById(R.id.reload_btn);
        reloadBtn.setOnClickListener(new ClickListener());
        backBtn.setOnClickListener(new ClickListener());
        clearBtn.setOnClickListener(new ClickListener());
        saveBtn.setOnClickListener(new ClickListener());
        saveBtn.setBackground(null);
        saveBtn.setText(R.string.save_str);
        initDatabase();
    }



    private void initDatabase(){
        List<SN> sns = DBUtil.findAllSN(sdb,dbHelper);
        String snsStr = "";
        if(sns != null && sns.size() > 0){
            for (int i=0;i<sns.size();i++){
                snsStr+="SN:"+sns.get(i).getSn()+"\n";
            }
        }
        snEdt.setText(snsStr);
    }

    private class ClickListener implements View.OnClickListener{
        @Override
        public void onClick(View v) {
            switch (v.getId()){
                case R.id.clear_btn:
                    snEdt.setText("");
                    break;
                case R.id.save_btn:
                    save();
                    break;
                case R.id.back_btn:
                    goBack();
                    break;
                case R.id.reload_btn:
                    DialogUtil.showOperDialog(getApplicationContext(),handler,"是否已在内存存储queyin文件夹下放置sn_list.txt文件,并会清空已加载的声码。",0,ConstantHandler.RELOAD_SNS_FILE);
                    break;

            }
        }
    }
    public void goBack(){
        if(isSave){
            EditSnUI.this.finish();
        }else {
            DialogUtil.showOperDialog(getApplicationContext(), handler, "是否保存设置后的参数!", 20, ConstantHandler.SAVE_SETTING);
        }
    }

    private Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what){
                case ConstantHandler.SAVE_SETTING:
                    save();
                    break;
                case ConstantHandler.CANCEL:
                    if(!isSave){
                        EditSnUI.this.finish();
                    }
                    break;
                case ConstantHandler.RELOAD_SNS_FILE:
                    reloadFile();
                    break;
            }
        }
    };
    private void reloadFile(){
        String filePath = Environment.getExternalStorageDirectory()+ "/queyin/sn_list.txt";
        String res = CommUtils.readText(filePath);
        if(!"文件不存在".equals(res ) && !"文件读取异常".equals(res)){
            //删选出声码信息
            saveSns(res,"读取成功");
            initDatabase();
        }else {
            DialogUtil.showToast(getApplicationContext(),res,Toast.LENGTH_SHORT);
        }
    }
    private void save(){
        if(snEdt.getText().toString().trim().length() >0){
            saveSns(snEdt.getText().toString(),"保存成功");
            isSave = true;
            EditSnUI.this.finish();
        }else{
            DialogUtil.showToast(EditSnUI.this, "声码内容为空", Toast.LENGTH_SHORT);
        }
    }

    private void saveSns(String text,String desc){
        List<SN> sns = CommUtils.findSns(text);
        if(sns != null && sns.size() > 0){
            DBUtil.deleteAllSN(dbHelper, sdb);
            for (SN snBean : sns){
                DBUtil.wavSnData(snBean,sdb,dbHelper);
            }
            DialogUtil.showToast(EditSnUI.this, desc, Toast.LENGTH_SHORT);
        }else{
            DialogUtil.showToast(EditSnUI.this, "保存内容不存在声码格式", Toast.LENGTH_SHORT);
        }
    }
}
