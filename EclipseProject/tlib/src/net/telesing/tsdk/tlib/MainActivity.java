package net.telesing.tsdk.tlib;

import javax.xml.transform.Templates;

import org.w3c.dom.Text;

import android.R.integer;
import android.R.string;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import net.telesing.tsdk.tlib.Acomms;
import android.view.View.OnClickListener;
import android.widget.Button;
import net.telesing.tsdk.tlib.RecogStatus;
import net.telesing.tsdk.tlib.RecogCfg;
import net.telesing.tsdk.tlib.CerInfor;

public class MainActivity extends Activity {

	// !测试用的控件
	Button start_button, stop_button;
	TextView log_view;
	int textcount;

	// 测试用的数据类型
	String temp = new String();
	String encodeCer = new String();
	Acomms acomms = new Acomms();
	CerInfor cer_temp = new CerInfor();
	CerInfor get_cer_temp = new CerInfor();
	RecogCfg rec_cfg = new RecogCfg();
	RecogCfg rec_set = new RecogCfg();
	RecogStatus recog_status = new RecogStatus();
	int  error_ount = 0;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		initButton();
		initView();
		showMessageOnTextView("控件初始化完毕.....\n");

		encodeCer ="3934353243373537454232363237363644444433414546464136383042314132393943413545314538303133313432304345333435353942373434373235424535413733383536433836353643414243423044393430313331423541374230323630413841454539343744443730393241463030393330363731393537383643384438344137303437434536383531413243363143353144453538353433353746443138413837413934463437373944393039454641323333413546454530433235314438434237304636424541313041443038413335464238363532463932464243464145353437314346393031353131323844303841333941314133343635414639393837313230443939334635373041444230433835343139394534343535393032413539413239323232354533464342423841414244323841364232393645454342303739453245433142354533393846423945373044313445413032444145384334314133373834344234443539383635334146424633333231393244334235324536363946304444363631433832353041453835384245364641454542413530413130354535463946433835373432413633383238454437324345414130464541464530343541393030383239343437373944423845394530424335443946314332383439433335393445354631444132463931344644453539";

		// !使用
		rec_cfg.gap = 100;
		// !初始化
		// acomms.initRecog();
		// 开始运行,带参数
		
		// 重新设置配置参数
		 

		// 配置样例证书参数
		cer_temp.section    = "1000";
		cer_temp.expiration = "2016-12-9";
		cer_temp.authority  = "3";

		cer_temp.genrTime = "2015-1-09 11:11:11";
		cer_temp.genrType = "1";
		cer_temp.ID       = "012";
		cer_temp.orgID    = "120";
		cer_temp.genrIp   = "192.168.1.1";
		cer_temp.sectSrc = "智声E联";
		
		cer_temp.cerLen = 100;
		// ************************************************

	
		
		//测试证书加密
		temp = acomms.encryptCer(cer_temp);

		Log.e("[APP]:", "加密证书内容为" + temp);
		
//		//测试证书绑定
		error_ount = acomms.bindCer(temp);
		
		error_ount = acomms.isBindCer(temp);
//		
//	//	error_ount = acomms.unbindCer(temp);
//		
		if(error_ount==0)
		{
			Log.e("[APP]:", "证书绑定返回值="+error_ount);	
		}
////		error_ount = acomms.isBindCer(temp);
////		if(error_ount!=0)
////		{
////			Log.e("[APP]:", "还没与证书绑定");	
////		}
//		cer_temp = acomms.decryptCer(encodeCer);
//		
////		error_ount = acomms.checkSectionsBindState("1234");
////		
////		Log.e("[APP]:","证书权限 = "+error_ount);
//		// 获取到的加密的证书

//		
//		//
//		// 获取到的证书
//		 Log.e("[APP]:","ccer_temp.section   = "+cer_temp.section);
//		 Log.e("[APP]:","ccer_temp.expiration= "+cer_temp.expiration);		 
//		 Log.e("[APP]:","ccer_temp.authority = "+cer_temp.authority);
//		 Log.e("[APP]:","ccer_temp.ID        = "+cer_temp.ID);
//		 Log.e("[APP]:","ccer_temp.orgID     = "+cer_temp.orgID);
//		 
//		 Log.e("[APP]:","ccer_temp.genrTime  = "+cer_temp.genrTime);
//		 Log.e("[APP]:","ccer_temp.genrType  = "+cer_temp.genrType);
//		 Log.e("[APP]:","ccer_temp.genrIp    = "+cer_temp.genrIp);			 
//		 Log.e("[APP]:","ccer_temp.sectSrc   = "+cer_temp.sectSrc);
//
//		 Log.e("[APP]:","ccer_temp.cerLen    = "+cer_temp.cerLen);

		// //获取到的数据
		// Log.e("[APP]:","recog_status.ss             = "+recog_status.ss);
		// Log.e("[APP]:","recog_status.blankBuffer    = "+recog_status.blankBuffer);
		// Log.e("[APP]:","recog_status.recogStat      = "+recog_status.recogStat);
		// // Log.e("[APP]:","加密后的字符串"+temp);
		
		//测试启动和停止函数
		// 启动参数配置
		rec_cfg.bufferLen = 100;
		rec_cfg.startFeq = new int[] { 1, 2, 3, 4 };
		rec_cfg.endFeq = new int[] { 4, 3, 2, 1 };
		// rec_cfg.startFeq[]
		rec_cfg.fftChains = 3;
		rec_cfg.fftPoints = 2048;
		rec_cfg.gap = 215;
		rec_cfg.st = 0.15;
		
		//设置参数配置
		rec_set.bufferLen = 5000;
		rec_set.startFeq = new int[] { 1, 2, 3, 4};
		rec_set.endFeq = new int[] { 4, 3, 2, 1 };
		// rec_cfg.startFeq[]
		rec_set.fftChains = 30;
		rec_set.fftPoints = 2048;
		rec_set.gap = 2105;
		rec_set.st = 0.105;
		
	//	rec_set=acomms.getRecogConfig();{
	//	}
		short [] waves = new  short[]{1,2,3,4,5,6,7,8,9};
		
//	    acomms.writeRecog(waves);
//	/////////////////////////////////////////////////////////	
		if(0==acomms.startRecog(rec_cfg)){
			Log.e("[APP]:","不带参数没有设置过cfg启动解调成功....");
		}
//		
		}	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	// 按钮控件初始化
	public void initButton() {
		start_button = (Button) findViewById(R.id.start);
		stop_button = (Button) findViewById(R.id.stop);
		start_button.setOnClickListener(new StartButtonListener());
		stop_button.setOnClickListener(new stopButtonListener());

	}

	// 日志显示初始化
	public void initView() {
		// log_view = (TextView)findViewById(R.id.log);

	}

	// 按钮事件响应
	// 内部类，实现OnClickListener接口
	class StartButtonListener implements OnClickListener {
		public void onClick(View v) {
			// log_view.setText(acomms.debugMessageOnTextView);
		}

	}

	// 回掉函数显示调试日志
	class stopButtonListener implements OnClickListener {
		public void onClick(View v) {

		}

	}

	// !在指定的控件上显示调试信息
	public void showMessageOnTextView(String message) {
		if (20 == textcount) {
			textcount = 0;
			log_view.setText("");
		}
		++textcount;
		// log_view.append(acomms.debugMessageOnTextView);

	}
}
