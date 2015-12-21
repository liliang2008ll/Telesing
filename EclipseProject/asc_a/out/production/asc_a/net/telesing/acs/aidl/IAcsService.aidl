package net.telesing.acs.aidl;
interface IAcsService{
    boolean startService();//单独启动服务
    void register(String packageName,String cert_code);//注册
    int getSignal();//获得信号强度
    int start(String packageName,String cert_code);//启动
    boolean isRegister(String packageName,String cert_code);//是否注册
    boolean isRunning();//判断服务是否启动
    void stopService();//停止服务

}