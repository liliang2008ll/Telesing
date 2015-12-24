package net.telesing.scomm.android.core;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;


public class MainActivity extends Activity {

	final DemI nc=new DemI();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        android.os.Debug.waitForDebugger();
		short adc[]={22,44,88,100,7,5,6,7,8,9};
		short def[]=new short[1024];
		
		//! DemI初始化
		int n_v=2048;
		double st_v=0.15;						//! 相似度
		int gap_v=512;
		int feq_size=3;							//!  信道数
		//! 信道
		double sFeq[]={16, 12, 8, 4};//{5.5125};
		double eFeq[]={20, 16, 12, 8};//{11.025};
		int temp_var1=nc.init(n_v,st_v,gap_v,sFeq,eFeq,feq_size);//OK
		nc.start();
		
		nc.getStatus();
		nc.getBufferSize();
		nc.resizeBuffer(1024);
		nc.wirte(adc,8);
		nc.fillRate();
		def=nc.getRegSignal();
		nc.stop();
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
}
