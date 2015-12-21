package net.telesing.scomm.android.core;

import android.util.Log;

/**
 * Created by FengJZ on 2015/6/8.
 */
public class DemI {

    //! 鍒濆鍖�.so
    public native int init(int n_v, double st_v, int gap_v, double sFeq_v[], double eFeq_v[], int feq_size);

    //! 寮�濮嬭В璋�
    public native void start();

    //! 鍋滄瑙ｈ皟
    public native void stop();

    //! 鑾峰緱褰撳墠瑙ｈ皟鐘舵��
    public native int getStatus();

    //! 鑾峰彇缂撳啿鍖哄ぇ灏�
    public native int getBufferSize();

    //! 閲嶇疆缂撳啿鍖哄ぇ灏�
    public native void resizeBuffer(int size);

    //! 娓呯┖缂撳啿鍖烘暟鎹�
    public native void clearBuffer();

    //! 鍚戠紦鍐插尯鍐欏叆鏁版嵁
    public native boolean wirte(short[] data, int data_size);

    //! 缂撳啿鍖哄～鍏呯巼锛�0锛氱┖锛�1锛氭弧
    public native double fillRate();

    //! 鑾峰緱褰撳墠瑙ｆ瀽鐨勫０鐮佺殑閲囨牱
    public native short[] getRegSignal();

    //! 鍥炶皟鍑芥暟 synced
    private void synced() {
        Demodulator.successSynced(System.currentTimeMillis());
        System.out.println("[java] synced");
        Log.e("================>","synced");
    }

    //! 鍥炶皟鍑芥暟 淇濈暀鐗堟湰
    private void succRsv(String s) {
        System.out.println("[java] succRsv " + s);
        Demodulator.success(s, true, System.currentTimeMillis());
    }

    //! 鍥炶皟鍑芥暟 鍔犲瘑鐗堟湰
    private void succEnc(String s) {
        System.out.println("[java] succEnc " + s);
        Demodulator.success(s, false, System.currentTimeMillis());
        Log.e("======succEnc====>", s);
    }

    //! 鍥炶皟鍑芥暟
    private void failure(int i) {
        System.out.println("[java] failure");
        Demodulator.faile(i, System.currentTimeMillis(), getRegSignal());
        Log.e("=======failure=====>", ""+i);
    }


    //鍔犺浇so
    static {
        try{
            Log.e("sdfsdfsadsafdsa","success");
           // System.loadLibrary("DemI");
        }catch (Exception e){
            Log.e("system.loadlibrary:",e.getMessage());
        }

    }

}
