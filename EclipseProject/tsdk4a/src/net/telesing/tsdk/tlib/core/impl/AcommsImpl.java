package net.telesing.tsdk.tlib.core.impl;

import android.content.Context;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Environment;
import android.util.Log;
import net.telesing.tsdk.tlib.CerInfor;
import net.telesing.tsdk.tlib.RecogCfg;
import net.telesing.tsdk.tlib.RecogStatus;
import net.telesing.tsdk.tlib.core.Demodulator;
import net.telesing.tsdk.tlib.service.AcommsService;
import net.telesing.tsdk.tlib.utils.WAVMaker;

/**
 * Created by Administrator on 2015/12/3.
 */
public class AcommsImpl  {


    public static int state() {
        return Demodulator.state();
    }

    public static int close(Context context) {
        try {
            Intent intent = new Intent(context, AcommsService.class);
            context.stopService(intent);
        } catch (Exception e) {
            e.printStackTrace();
            return 1;
        }
        return 0;
    }

    public static int open(Context context) {
        try {
            Intent intent = new Intent(context, AcommsService.class);
            intent.putExtra(AcommsService.OP_STR, AcommsService.OP_OPEN);
            context.startService(intent);
        } catch (Exception e) {
            e.printStackTrace();
            return 1;
        }
        return 0;
    }

    public static int run(Context context) {
        try {
            Intent intent = new Intent(context, AcommsService.class);
            intent.putExtra(AcommsService.OP_STR, AcommsService.OP_RUN);
            context.startService(intent);
        } catch (Exception e) {
            e.printStackTrace();
            return 1;
        }
        return 0;
    }

    public static int bindCer(String packageName,String cer){
        int result = Demodulator.bindCer(cer);
        if(result == 0){//绑定成功 得到证书的详细信息 并根据详细信息保存到数据库中
            CerInfor cerInfor = Demodulator.decryptCer(cer);
            Demodulator.saveCer(packageName,cerInfor);
        }
        return result;
    }
    public static int unbindCer(String cer){
        return Demodulator.unbindCer(cer);
    }
    public static int isbindCer(String cer){
        return Demodulator.isbindCer(cer);
    }

    public static int checkSectionsBindState(String section){
        return Demodulator.checkSectionsBindState(section);
    }

    public static short[] genrWave(String section,String data){
        return Demodulator.genrWave(section,data);
    }

    public static int genrWave(String section,String data,String wavePath){
        short[] waves = genrWave(section,data);
        if(waves != null && waves.length > 0){
            WAVMaker.makeRaw(wavePath,waves,1);
            return 0;
        }
        return 1;
    }
    public static String getSection(String enccer){
        CerInfor infor = Demodulator.decryptCer(enccer);
        if(infor != null)
            return infor.getSection();
        return null;
    }
    public static int setRecogConfig(RecogCfg cfg){
        return Demodulator.setRecogConfig(cfg);
    }
    public static RecogCfg getRecogConfig(){
        return Demodulator.getRecogConfig();
    }

    public static RecogStatus getRecogStatus(){
        return Demodulator.getRecogStatus();
    }
    public static int play(String filePath){
        final MediaPlayer mediaPlayer = new MediaPlayer();
        try {
            mediaPlayer.reset();//重置为初始状态
            mediaPlayer.setDataSource(filePath);
            mediaPlayer.prepare();//缓冲
            mediaPlayer.start();//开始或恢复播放
        }catch (Exception e){
            return 1;
        }

        mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {//播出完毕事件
            @Override public void onCompletion(MediaPlayer arg0) {
                mediaPlayer.release();
            }
        });
        mediaPlayer.setOnErrorListener(new MediaPlayer.OnErrorListener() {// 错误处理事件
            @Override public boolean onError(MediaPlayer player, int arg1, int arg2) {
                mediaPlayer.release();
                return false;
            }
        });
        return 0;
    }
    public static int play(String section,String datas){
        String path = Environment.getExternalStorageDirectory() + "/qyttemp/temp.wav";
//        int res = genrWave(section,datas,path );
        int res = 0;
        if(res == 0){
            play(path);
        }
       return res;
    }
    public static double getSignal(){
        RecogStatus recogStatus = getRecogStatus();
        if(recogStatus != null){
            return getRecogStatus().getSs();
        }else{
            return 0;
        }


    }


    public static String getCert(){
        return Demodulator.getCert();
    }
}
