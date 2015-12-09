/*******************************************************************
 *  Copyright(c) 2015-2017 Nanjing telesing
 *  All rights reserved.
 *
 *  文件名称: core
 *  简要描述: acomms 核心算法对interface接口封装类库
 *
 *  当前版本:1.1
 *  作   者:Lee.liang
 *  日   期:
 *  说   明:
 *
 *  取代版本:1.0
 *  作   者:
 *  日   期:
 *  说   明:
 ******************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./tlib/interface/i4cpp/tlib.h"

//测试用的代码


Recog_cfg input_cfg;
Recog_cfg set_cfg;
Recog_cfg get_cfg;
Recog_status get_status;
ACOMMS acomms;
double sFeq[]={16, 12, 8, 4};//{5.5125};
double eFeq[]={20, 16, 12, 8};//{11.025};
string input_cer = "telesing|2015-12-7";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ViewLog->setText("start....");
    short testdata[1000];
    for(int i = 0; i <1000;i++){
        testdata[i] = i;
    }
    input_cfg.input_buffer_len = 2048;
    input_cfg.input_end_feq    = eFeq;
    input_cfg.input_fft_chains = 3;
    input_cfg.input_gap        = 512;
    input_cfg.input_st         = 0.15;
    input_cfg.input_start_feq  = sFeq;
 //声明通信类函数测试
    acomms.recog_start(input_cfg);
    acomms.recog_stop();
    acomms.recog_write(testdata,1000);


 //证书绑定类函数测试
    acomms.cer_bind(input_cer,10);
    acomms.cer_unbind(input_cer,10);
    acomms.cer_decrypt(input_cer);
    acomms.cer_encrypt(input_cer);
    acomms.cer_section_auth(input_cer);
    acomms.cer_isbind(input_cer,100);

 //系统配置函数测试
    acomms.recog_setconfig(set_cfg);
    acomms.recog_getconfig(get_cfg);
    acomms.recog_status(get_status);
}

MainWindow::~MainWindow()
{
    delete ui;
}
