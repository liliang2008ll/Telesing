package net.telesing.tsdk.tlib.core;

import android.content.Context;
import android.content.Intent;
import android.database.sqlite.SQLiteDatabase;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.util.Log;
import com.google.gson.Gson;
import net.telesing.tsdk.tlib.Acomms;
import net.telesing.tsdk.tlib.CerInfor;
import net.telesing.tsdk.tlib.RecogCfg;
import net.telesing.tsdk.tlib.RecogStatus;
import net.telesing.tsdk.tlib.bean.SNBean;
import net.telesing.tsdk.tlib.bean.SectionBean;
import net.telesing.tsdk.tlib.db.Cookie;
import net.telesing.tsdk.tlib.db.DBHelper;
import net.telesing.tsdk.tlib.db.DBUtil;
import net.telesing.tsdk.tlib.utils.Utils;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Created by Administrator on 2015/12/2.
 */
public class Demodulator {
    private static final int SLEEP_MS = 100;

    private static final int AUDIO_SOURCE = MediaRecorder.AudioSource.MIC;
    private static final int SAMPLE_RATE_IN_HZ = 44100;
    private static final int CHANNEL_CONFIG = AudioFormat.CHANNEL_IN_MONO;
    private static final int AUDIO_FORMAT = AudioFormat.ENCODING_PCM_16BIT;
    private static final int MIN_BUFFER_SIZE = AudioRecord.getMinBufferSize(
            SAMPLE_RATE_IN_HZ,
            CHANNEL_CONFIG,
            AUDIO_FORMAT
    );

    private static int state = AcommsInterface.CLOSED;

    private static int audio_state = AcommsInterface.INITIALLING_FAILE;

    private static boolean isIC = true;

    private int type;

    public static int state() {
        if(state != AcommsInterface.RUNNING){
            return state;
        }else {
            return audio_state;
        }
    }

    private static Thread op = null;

    private static short[] readBuffer = null;
    private static Acomms acomms = new Acomms();
    private static AudioRecord audioRecord = null;

    private static Map<String, Long> snCache = null;
    private static DemingThread demingThread = null;
    private static ClearThread clearThread = null;
    private static Context context;
    private static Cookie cookie;
    private static SQLiteDatabase sdb;
    private static DBHelper dbHelper;
    public static void setDBHelper(DBHelper dbHelper) {
        Demodulator.dbHelper = dbHelper;
    }
    public static void setContext(Context context) {
        Demodulator.context = context;
    }

    public static void setSQLiteDatabase(SQLiteDatabase sdb) {
        Demodulator.sdb = sdb;
    }
    public static void setCookie(Cookie cookie) {
        Demodulator.cookie = cookie;
    }


    static {
        acomms = new Acomms();
    }

    static class DemingThread extends Thread {
        @Override
        public void run() {
            while (state == AcommsInterface.RUNNING) {
                int size = audioRecord.read(readBuffer, 0, readBuffer.length);
                if (AudioRecord.ERROR_INVALID_OPERATION != size) {
                    audio_state = AcommsInterface.RUNNING;
                    short[] datas = new short[size];
                    System.arraycopy(readBuffer, 0, datas, 0, size);
                    acomms.writeRecog(datas);
                }else {
                    audio_state = AcommsInterface.INITIALLING_FAILE;
                }
            }
        }
    }

    static class ClearThread extends Thread {
        @Override
        public void run() {
            while (state == AcommsInterface.RUNNING) {
                if (snCache.size() > 0) {
                    long currentTime = System.currentTimeMillis();
                    Set<Map.Entry<String, Long>> entrySet = snCache.entrySet();
                    Iterator<Map.Entry<String, Long>> it = entrySet.iterator();
                    while (it.hasNext()) {
                        Map.Entry<String, Long> e = it.next();
                        if (e.getValue() <= currentTime) {
                            it.remove();
                        }
                    }
                } else {
                    try {
                        sleep(SLEEP_MS);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    public static int close() {

        switch (state) {
            case AcommsInterface.CLOSED:
                return 0;
            case AcommsInterface.INITIALLING:
                return suspendInitialing();
            case AcommsInterface.RELEASING:
                return 0;
            case AcommsInterface.OPENED:
                return release();
            case AcommsInterface.STARTING:
                return suspendStarting_release();
            case AcommsInterface.STOPPING:
                return awaitStopping_release();
            case AcommsInterface.RUNNING:
                return stop_release();
            default:
                return 1;
        }
    }


    public static int open() {
        switch (state) {
            case AcommsInterface.CLOSED:
                return initial();
            case AcommsInterface.INITIALLING:
                return 0;
            case AcommsInterface.RELEASING:
                return suspendReleasing();
            case AcommsInterface.OPENED:
                return 0;
            case AcommsInterface.STARTING:
                return suspendStarting();
            case AcommsInterface.STOPPING:
                return 0;
            case AcommsInterface.RUNNING:
                return stop();
            default:
                return 1;
        }
    }


    public static int run() {
        switch (state) {
            case AcommsInterface.CLOSED:
                return initial_start();
            case AcommsInterface.INITIALLING:
                return awaitInitialling_start();
            case AcommsInterface.RELEASING:
                return suspendReleasing_start();
            case AcommsInterface.OPENED:
                return start();
            case AcommsInterface.STARTING:
                return 0;
            case AcommsInterface.STOPPING:
                return suspendStopping();
            case AcommsInterface.RUNNING:
                return 0;
            default:
                return 1;
        }
    }


    private static int initial() {
        state = AcommsInterface.INITIALLING;

        op = new Thread() {
            public void run() {
                while (state == AcommsInterface.INITIALLING) {
                    try {
                        readBuffer = new short[MIN_BUFFER_SIZE];
                        snCache = new ConcurrentHashMap<String, Long>();
                        audioRecord = new AudioRecord(
                                AUDIO_SOURCE,
                                SAMPLE_RATE_IN_HZ,
                                CHANNEL_CONFIG,
                                AUDIO_FORMAT,
                                MIN_BUFFER_SIZE
                        );
                        audioRecord.startRecording();
                        state = AcommsInterface.OPENED;
                    } catch (IllegalArgumentException e) {
                        e.printStackTrace();
                        state = AcommsInterface.INITIALLING_FAILE;
                        Log.e("IllegalArgument","IllegalArgumentException");
                    } catch (IllegalStateException e) {
                        e.printStackTrace();
                        Log.e("IllegalStateException", "IllegalStateException");
                        state = AcommsInterface.INITIALLING_FAILE;
                    }
                }
            }
        };
        op.start();

        return 0;
    }

    private static int start() {
        state = AcommsInterface.STARTING;
        op = new Thread() {
            public void run() {
                while (state == AcommsInterface.STARTING) {
                    try {
                        RecogCfg recog_cfg = new RecogCfg(new int[]{cookie.getfeq(Cookie.DEM_SFEQ,16*1000)},new int[]{cookie.getfeq(Cookie.DEM_EFEQ,20*1000)},cookie.getGapInteger(Cookie.DEM_GAP), cookie.getStDouble(Cookie.DEM_ST),cookie.getBufferLeng(Cookie.BUFFER_LEN));
                        acomms.startRecog(recog_cfg);
                        audioRecord.startRecording();
                        snCache = new ConcurrentHashMap<String, Long>();
                        demingThread = new DemingThread();
                        clearThread = new ClearThread();
                        state = AcommsInterface.RUNNING;
                        demingThread.start();
                        clearThread.start();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        };
        op.start();

        return 0;
    }

    private static int stop() {
        state = AcommsInterface.STOPPING;

        op = new Thread() {
            public void run() {
                while (state == AcommsInterface.STOPPING) {
                    try {
                        audioRecord.stop();
                        acomms.stopRecog();
                        if (clearThread != null) {
                            clearThread.join();
                            clearThread = null;
                        }
                        if (demingThread != null) {
                            demingThread.join();
                            demingThread = null;
                        }

                        if (snCache != null) {
                            snCache = null;
                        }
                        state = AcommsInterface.OPENED;
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        };
        op.start();

        return 0;
    }

    private static int release() {
        state = AcommsInterface.RELEASING;

        op = new Thread() {
            public void run() {
                while (state == AcommsInterface.RELEASING) {
                    try {
                        if (readBuffer != null) {
                            readBuffer = null;
                        }
                        if (snCache != null) {
                            snCache = null;
                        }
                        if (acomms != null) {
                            acomms = null;
                        }
                        if (audioRecord != null) {
                            audioRecord.release();
                            audioRecord = null;
                        }
                        state = AcommsInterface.CLOSED;
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        };
        op.start();

        return 0;
    }

    private static void join() {
        if (op != null) {
            boolean joined = false;
            while (!joined) {
                try {
                    op.join();
                    joined = true;
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private static int suspendInitialing() {
        state = AcommsInterface.RELEASING;
        join();
        return release();
    }

    private static int suspendStarting_release() {
        state = AcommsInterface.STOPPING;
        join();
        stop();

        join();
        return release();
    }


    private static int awaitStopping_release() {
        join();

        return release();
    }

    private static int stop_release() {
        stop();

        join();
        return release();
    }

    private static int suspendReleasing() {
        state = AcommsInterface.INITIALLING;
        join();
        return initial();
    }

    private static int suspendStarting() {
        state = AcommsInterface.STOPPING;
        join();
        return stop();
    }

    private static int initial_start() {
        initial();

        join();
        return start();
    }

    private static int awaitInitialling_start() {
        join();

        return start();
    }

    private static int suspendReleasing_start() {
        state = AcommsInterface.INITIALLING;
        join();
        initial();

        join();
        return start();
    }

    private static int suspendStopping() {
        state = AcommsInterface.STARTING;
        join();
        return start();
    }


    public static int bindCer(String cer){
        if(cer != null && acomms != null){
            return acomms.bindCer(cer);
        }
        return 1;

    }

    public static int unbindCer(String cer){
        if(cer != null && acomms != null){
            return acomms.unbindCer(cer);
        }
        return 1;
    }
    public static int isbindCer(String cer){
        if(cer != null && acomms != null){
            return acomms.isBindCer(cer);
        }
        return 1;
    }

    public static int checkSectionsBindState(String section){
        if(section != null && acomms != null){
            return acomms.checkSectionsBindState(section);
        }
        return 1;
    }

    public static short[] genrWave(String section,String data){
        if(section != null && data != null && acomms != null){
            return acomms.genrWave(section,data);
        }
        return null;
    }


    public static int setRecogConfig(RecogCfg cfg){
        if(cfg != null && acomms != null){
            return acomms.setRecogConfig(cfg);
        }
        return 1;
    }

    public static RecogCfg getRecogConfig(){
        return acomms.getRecogConfig();
    }

    public static RecogStatus getRecogStatus(){
        if(acomms != null){
            return acomms.getRecogStatus();
        }
        return null;

    }
    public static int pauseRecog(){
        return  acomms.pauseRecog();
    }
    public static int restartRecog(){
        return acomms.resumeRecog();
    }
    public static CerInfor decryptCer(String encCer){
        return acomms.decryptCer(encCer);
    }

    /**
     * 将证书信息保存到数据库中
     * @return
     */
    public static void saveCer(String packageName,CerInfor cerInfor){
        DBUtil.operSvec(packageName,cerInfor.getSection(), Utils.getDiffDay(Utils.getStrSystemTime("1"),cerInfor.getExpiration()),10000,cerInfor.getSectSrc(),sdb,dbHelper);
    }

    public static void success(SNBean snBean) {
        if (state == AcommsInterface.RUNNING) {
            Long have = snCache.get(snBean.getSection()+snBean.getData());
            Intent intent = new Intent(AcommsInterface.HAVE_SN_INTENT);
            if (have == null) {
                snBean.setHave(false);
                snCache.put(snBean.getSection()+snBean.getData(), snBean.getRec_time() + cookie.getCacheTime(Cookie.CACHE_CLEAR_MS));
                intent.putExtra(AcommsInterface.HAVE_SN_STRING, snBean.getSection()+snBean.getData());
            } else {//高级测试使用
                snBean.setHave(true);
            }
            intent.putExtra("svec_bean",new Gson().toJson(snBean));
            sendBroadCast(intent,snBean.getSection(),context);

        }
    }

    public static void sendBroadCast(Intent intent,String data,Context context){
        List<SectionBean> packages = DBUtil.findPackages(data, sdb, dbHelper);
        if(packages != null && packages.size() > 0){
            for (SectionBean sectionBean : packages){
                intent.setPackage(sectionBean.getPackageName());
                context.sendBroadcast(intent);
                intent.putExtra("org_name",sectionBean.getOrgName());
                intent.setPackage("net.telesing.tscom");
                context.sendBroadcast(intent);
            }
        }else {
            intent.putExtra("org_name","未知机构");
            intent.setPackage("net.telesing.tscom");
            context.sendBroadcast(intent);
        }
    }

    public static String getCert(){

//        cer_temp.section = "1234";
//        cer_temp.expiration = "2016-12-09";
//        cer_temp.authority = "3";
//
//        cer_temp.genrTime = "2015-12-09 11:11:11";
//        cer_temp.genrType = "1";
//        cer_temp.ID = "0123456789";
//        cer_temp.orgID = "1234567890";
//        cer_temp.genrIp = "192.168.100.100";
//        cer_temp.sectSrc = "1111111122222222333333334444444455555555666666667777777788888888";
//
//        cer_temp.cerLen = 100;


        CerInfor infor = new CerInfor();
        infor.setSection("1000");
        infor.setExpiration("2015-12-21");
        infor.setAuthority("3");
        infor.setGenrTime("2015-12-21 16:30:25");
        infor.setGenrIp("192.168.1.200");
        infor.setGenrType("1");
        infor.setID("121");
        infor.setOrgID("1212");
        infor.setSectSrc("阙音演示");
        return acomms.encryptCer(infor);
    }





}
