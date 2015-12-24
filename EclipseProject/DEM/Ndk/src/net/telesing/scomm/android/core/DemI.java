
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


	//! ��ʼ��.so
	public native  int init(int n_v, double st_v, int gap_v, double sFeq_v[], double eFeq_v[],int feq_size);
	//! ��ʼ���
	public native  void start();
	//! ֹͣ���
	public native  void stop();
    //! ��õ�ǰ���״̬
    public native  int getStatus();
    //! ��ȡ��������С
    public native  int getBufferSize();
    //! ���û�������С
    public native  void resizeBuffer(int size);
    //! ��ջ���������
    public native  void clearBuffer();
    //! �򻺳���д������
    public native  boolean wirte(short[] data,int data_size);
    //! ����������ʣ�0���գ�1����
    public native  double fillRate();
    //! ��õ�ǰ����������Ĳ���
    public native  short[] getRegSignal();

	//! �ص����� synced
    private void synced()
    {
		String str="[java] synced";
	    System.out.println(str);
        Log.d(TAG, str);  
        System.out.printf(str);
    }

    //!  �ص����� �����汾
    private void succRsv(String s)
    {
		String str="[java] succRsv "+s;
	    System.out.println(str);
        Log.d(TAG, str);  
        System.out.printf(str);
    }
    //!  �ص����� ���ܰ汾
    private void succEnc(String s)
    {
		String str="[java] succEnc "+s;
	    System.out.println(str);
        Log.d(TAG, str);  
        System.out.printf(str);
    }
    //!  �ص����� 
    private void failure(int i)
    {
		String str="[java] failure";
	    System.out.println(str);
        Log.d(TAG, str);  
        System.out.printf(str);
    }
	
    
	//����so
    static 
    {  
        System.loadLibrary("DemI");
    }
	
}
