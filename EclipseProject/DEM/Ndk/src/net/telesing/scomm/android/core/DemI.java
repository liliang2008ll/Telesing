
package net.telesing.scomm.android.core;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class DemI 
{
	private static final String TAG = "xx";  

	public native   int     testFun1();
	public native   int     testFun2(int type);
	public native   boolean testFun3(short[] data,int data_size);


	//! 初始化.so
	public native  int init(int n_v, double st_v, int gap_v, double sFeq_v[], double eFeq_v[],int feq_size);
	//! 开始解调
	public native  void start();
	//! 停止解调
	public native  void stop();
    //! 获得当前解调状态
    public native  int getStatus();
    //! 获取缓冲区大小
    public native  int getBufferSize();
    //! 重置缓冲区大小
    public native  void resizeBuffer(int size);
    //! 清空缓冲区数据
    public native  void clearBuffer();
    //! 向缓冲区写入数据
    public native  boolean wirte(short[] data,int data_size);
    //! 缓冲区填充率，0：空；1：满
    public native  double fillRate();
    //! 获得当前解析的声码的采样
    public native  short[] getRegSignal();

	//! 回调函数 synced
    private void synced()
    {
		String str="[java] synced";
	    System.out.println(str);
        Log.d(TAG, str);  
        System.out.printf(str);
    }

    //!  回调函数 保留版本
    private void succRsv(String s)
    {
		String str="[java] succRsv "+s;
	    System.out.println(str);
        Log.d(TAG, str);  
        System.out.printf(str);
    }
    //!  回调函数 加密版本
    private void succEnc(String s)
    {
		String str="[java] succEnc "+s;
	    System.out.println(str);
        Log.d(TAG, str);  
        System.out.printf(str);
    }
    //!  回调函数 
    private void failure(int i)
    {
		String str="[java] failure";
	    System.out.println(str);
        Log.d(TAG, str);  
        System.out.printf(str);
    }
	
    
	//加载so
    static 
    {  
        System.loadLibrary("DemI");
    }
	
}
