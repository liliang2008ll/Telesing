package net.telesing.tscom.ui;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import net.telesing.tscom.R;

/**
 * Created by Administrator on 2015/11/23.
 */
public class DetailUI extends BaseUI {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.detail_layout);
        init();
    }

    private void init(){

        backBtn = (Button)findViewById(R.id.back_btn);
        saveBtn = (Button)findViewById(R.id.save_btn);
        saveBtn.setVisibility(View.GONE);
        backBtn.setOnClickListener(new ClickListener());
    }





    private class ClickListener implements View.OnClickListener{
        @Override
        public void onClick(View v) {
            switch (v.getId()){
                case R.id.back_btn:
                   DetailUI.this.finish();
                    break;


            }
        }
    }

}
