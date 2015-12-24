package net.telesing.tscom.aidl;
interface IAcsService{
    int startService();//单独启动服务
    int register(String packageName,String cert);//注册
    int getSignal();//获得信号强度
    int start(String packageName,String cert);//启动
    int isRegister(String packageName,String cert_code);//是否注册
    int isRunning();//判断服务是否启动
    int stopService();//停止服务
    int play(String section,String datas);//播放
    String getSection(String cert);//根据证书获得码段

}