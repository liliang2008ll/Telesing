package net.telesing.tscom.service;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import net.telesing.tscom.SvecApplication;
import net.telesing.tscom.aidl.IAcsService;
import net.telesing.tscom.manager.ServiceManager;
import net.telesing.tsdk.tlib.core.AcommsInterface;


public class AscServiceImpl extends Service {



    @Override
    public void onCreate() {
        super.onCreate();
        Log.e("onCreate","oncreate");
    }

    @Override
    public IBinder onBind(Intent intent) {

        return new IAscServiceImpl();
    }

    private void showNotification(){
        //打开通知栏
        if(!SvecApplication.getInstance().isNotification()){
            SvecApplication.getInstance().showNotification(getApplicationContext());
        }else{
            SvecApplication.getInstance().setImage(null,1);
        }
        //打开悬浮窗
        ServiceManager.startFloatService(getApplicationContext());
    }

    public class IAscServiceImpl extends IAcsService.Stub{
        @Override
        public int startService() throws RemoteException {
            return ServiceManager.startVoice(getApplicationContext());
        }

        @Override
        public int register(String packageName, String cert_code) throws RemoteException {
            return AcommsInterface.bindCer(packageName,cert_code);
        }

        @Override
        public int getSignal() throws RemoteException {
            return (int)(AcommsInterface.getSignal()*100);
        }

        @Override
        public int start(String packageName, String cert_code) throws RemoteException {
            //启动检测开关
            startService();
            cert_code = AcommsInterface.getCert();
//            Log.e("=========>",cert_code);
            register(packageName, cert_code); //注册
            showNotification();
            return AcommsInterface.state();

        }

        @Override
        public int isRegister(String packageName, String cert_code) throws RemoteException {
            return AcommsInterface.isbindCer(cert_code);
        }

        @Override
        public int isRunning() throws RemoteException {
            if(AcommsInterface.state() == AcommsInterface.RUNNING){
                return 0;
            }
            return  1;
        }

        @Override
        public int stopService() throws RemoteException {
            ServiceManager.closeVoice(getApplicationContext());
            showNotification();
            return 0;
        }

        @Override
        public int play(String section, String datas) throws RemoteException {
            return AcommsInterface.play(section,datas);
        }

        @Override
        public String getSection(String cert) throws RemoteException {
            return AcommsInterface.getSection(cert);
        }
    }


}
