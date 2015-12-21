package net.telesing.acs.ui;

import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.Html;
import android.view.View;
import android.widget.*;
import net.telesing.acs.R;
import net.telesing.acs.adapter.MsgAdapter;
import net.telesing.acs.bean.SNBean;
import net.telesing.acs.common.contants.ConstantHandler;
import net.telesing.acs.common.utils.CommUtils;
import net.telesing.acs.common.utils.DialogUtil;
import net.telesing.acs.db.DBUtil;
import net.telesing.acs.manager.FloatWindowManager;
import net.telesing.acs.receiver.SNReceiver;
import net.telesing.scomm.android.DemInterface;
import net.telesing.scomm.android.SvecResBean;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Administrator on 2015/11/23.
 */
public class SeniorUI extends BaseUI {

    private Button startBtn, resetBtn;
    private TextView resTxt,startTxt,resetTxt;
    private ListView showList;
    private RelativeLayout settingLayout;

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
        startTxt = (TextView)findViewById(R.id.start_txt);
        resetTxt = (TextView)findViewById(R.id.reset_txt);
        showList = (ListView) findViewById(R.id.msg_show_list);
        settingLayout = (RelativeLayout) findViewById(R.id.setting_layout);
        showList.setAdapter(getMsgAdapter(null));
        backBtn.setOnClickListener(new ClickListener());
        saveBtn.setOnClickListener(new ClickListener());
        startBtn.setOnClickListener(new ClickListener());
        resetBtn.setOnClickListener(new ClickListener());
        settingLayout.setOnClickListener(new ClickListener());


    }

    private class ClickListener implements View.OnClickListener {
        @Override
        public void onClick(View v) {
            Intent intent = null;
            switch (v.getId()) {
                case R.id.back_btn:
                    SeniorUI.this.finish();
                    break;
                case R.id.save_btn:
                    intent = new Intent(SeniorUI.this, SettingUI.class);
                    startActivity(intent);
                    break;
                case R.id.start_btn:
                    if (!isStart) {
                        List<SNBean> sns = DBUtil.findAllSN(sdb, dbHelper);
                        if (sns != null && sns.size() > 0) {
                            isStart = true;
                            startTxt.setText(R.string.pause_str);
                            startBtn.setBackgroundResource(R.drawable.msg_pause);
                        } else {
                            DialogUtil.showOperDialog(getApplicationContext(), handler, "您还没有加载声码列表\n请点击空白箭头区域->重载->保存加载声码列表", 9, ConstantHandler.NO_SETTING_SNS_LIST);
                        }

                    } else {
                        startTxt.setText(R.string.start_str);
                        startBtn.setBackgroundResource(R.drawable.msg_start);
                        isStart = false;
                    }
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


    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
                case ConstantHandler.RESOLUTION_END:
                    msgAdapter.addInfo(checkSn((SvecResBean) msg.obj, 1));
                    String text = "正确率：" + ((sucList.size() / msgAdapter.getInfoList().size()) * 100) + "%（" + (msgAdapter.getInfoList().size() - sucList.size()) + "/" + sucList.size() + "/" + msgAdapter.getInfoList().size() + "）\n平均耗时" + getSnAvTime() + "秒";
                    resTxt.setText(text);
                    FloatWindowManager.updateUsedPercent(2);
                    break;
            }
        }
    };

    private SvecResBean checkSn(SvecResBean svecResBean, int type) {
        //判断当前的声码是否在数据库中存在
        if (svecResBean != null && svecResBean.getBeaconSn() != null) {
            svecResBean.setMsgRes("N");
            if (DBUtil.checkSn(svecResBean.getBeaconSn(), sdb, dbHelper)) {
                if (svecResBean.isGood()) {//成功
                    svecResBean.setBg_res(R.color.s_success);
                    svecResBean.setResult("成功");
                    if (svecResBean.isHave()) {
                        svecResBean.setMsgRes("R");
                    }
                    if (type == 1) {
                        sucList.add(svecResBean.getBeaconSn());
                    }

                } else {//弱对
                    svecResBean.setBg_res(R.color.s_fail);
                    svecResBean.setResult("弱对");
                }
            } else {
                if (svecResBean.isGood()) {//强错
                    svecResBean.setBg_res(R.color.f_success);
                    svecResBean.setResult("强错");
                } else {//弱错
                    svecResBean.setBg_res(R.color.f_fail);
                    svecResBean.setResult("弱错");
                }
            }
        }

        //
        return svecResBean;
    }

    private MsgAdapter getMsgAdapter(final List<SvecResBean> infoList) {
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
                    SvecResBean sb = msgAdapter.getInfoList().get(i);
                    if ("成功".equals(sb.getResult())) {
                        time += sb.getTime();
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
                protected void doWhat(SvecResBean svecResBean) {

                    super.doWhat(svecResBean);
                    if (isStart) {
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
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (snReceiver != null)
            unregisterReceiver(snReceiver);
    }

}
