package net.telesing.tsdk.tlib;

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

	//!�����õĿؼ�
	Button start_button,stop_button;
	TextView log_view;
	int textcount;
	
	//�����õ���������
	String temp               = new String();	
	Acomms acomms 		      = new Acomms() ;
	CerInfor cer_temp         = new CerInfor();
	RecogCfg rec_cfg 		  = new  RecogCfg();
	RecogStatus recog_status = new RecogStatus();	
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		initButton();
		initView();
		showMessageOnTextView("�ؼ���ʼ�����.....\n");
		//!ʹ��
		rec_cfg.gap = 100;
		//!��ʼ��
	//	acomms.initRecog();
		//��ʼ����,������ 
	//	acomms.startRecog(cer_cfg);
		//�����������ò���
	//	acomms.setRecogConfig(cer_cfg);
		
		//����֤�����
		cer_temp.authority = "authority=liliang";
		cer_temp.expiration = "expiration =2016-12-09";
		cer_temp.genrTime = "genrTime =2015-12-09";
		cer_temp.genrType = "genrType =all";
		cer_temp.ID       = "ID =123456789";
		cer_temp.orgID    = "orgID =telesing";
		cer_temp.section  = "section=888888";
		cer_temp.sectSrc  = "sectSrc=nanjing";
		cer_temp.genrIp   = "genrIp =192.168.1.100";	
		cer_temp.cerLen   = 100;
		//************************************************

		//������������
		rec_cfg.bufferLen = 100;
		
	
		rec_cfg.startFeq	 = new int[]{1,2,3,4};	
		rec_cfg.endFeq       = new int[]{4,3,2,1};	
		
		//rec_cfg.startFeq[]
		rec_cfg.fftChains = 3;
		rec_cfg.fftPoints = 2048;
		rec_cfg.gap = 215;
		rec_cfg.st = 0.15;
		
		//***********************************************
		acomms.startRecog(rec_cfg);
		recog_status = acomms.getRecogStatus();		
		temp         = acomms.encryptCer(cer_temp, 100);		
		cer_temp     = acomms.decryptCer(temp, 100);		
		
		Log.e("[APP]:","ccer_temp.authority= "+cer_temp.authority);
		Log.e("[APP]:","ccer_temp.expiration= "+cer_temp.expiration);
		Log.e("[APP]:","ccer_temp.genrTime= "+cer_temp.genrTime);
		Log.e("[APP]:","ccer_temp.genrType= "+cer_temp.genrType);
		Log.e("[APP]:","ccer_temp.ID= "+cer_temp.ID);
		Log.e("[APP]:","ccer_temp.orgID= "+cer_temp.orgID);
		Log.e("[APP]:","ccer_temp.section= "+cer_temp.section);
		Log.e("[APP]:","ccer_temp.sectSrc= "+cer_temp.sectSrc);
		Log.e("[APP]:","ccer_temp.genrIp= "+cer_temp.genrIp);
		Log.e("[APP]:","ccer_temp.cerLen= "+cer_temp.cerLen);
		
		
		
		 //��ȡ��������
		 Log.e("[APP]:","recog_status.ss             = "+recog_status.ss);
		 Log.e("[APP]:","recog_status.blankBuffer    = "+recog_status.blankBuffer);
		 Log.e("[APP]:","recog_status.recogStat      = "+recog_status.recogStat);
		 Log.e("[APP]:","���ܺ���ַ���"+temp);
		 
	
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
	
	//��ť�ؼ���ʼ��
	public void initButton(){
		start_button = (Button)findViewById(R.id.start);
		stop_button  = (Button)findViewById(R.id.stop);
		start_button.setOnClickListener(new StartButtonListener());	
		stop_button.setOnClickListener(new stopButtonListener());		

		}
	//��־��ʾ��ʼ��
	public void initView() {
		//log_view = (TextView)findViewById(R.id.log);

	}
	
	//��ť�¼���Ӧ
	//�ڲ��࣬ʵ��OnClickListener�ӿ�
    class StartButtonListener implements OnClickListener
    {
        public void onClick(View v)
        {
         //  log_view.setText(acomms.debugMessageOnTextView);           
        }        
        
    }
    //�ص�������ʾ������־
    class stopButtonListener implements OnClickListener
    {
        public void onClick(View v)
        {
        	
          // log_view.append(acomms.debugMessageOnTextView);   
        	//	log_view.append(cer_cfg.toString());
        }        
        
    }
    //!��ָ���Ŀؼ�����ʾ������Ϣ
    public void showMessageOnTextView(String message ){
    	if(20 == textcount) {
    		textcount =0;
    		log_view.setText("");
    	}
    	++textcount;
     //  log_view.append(acomms.debugMessageOnTextView);  
    	
    }
}
