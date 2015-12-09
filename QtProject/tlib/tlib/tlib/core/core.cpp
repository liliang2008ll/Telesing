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
#include <stdio.h>
#include <string>
#include "core.h"
#include  "dem.h"
#include "./tlib/interface/i4cpp/tlib.h"
#include "./tlib/core/core.h"


//!
//! \brief CORE::cer_encrypt
//! \param input_cer
//! \return
//!
string CORE::cer_encrypt(string input_cer)
{
    return input_cer;
}
//!
//! \brief CORE::cer_decrypt
//! \param output_cer
//! \return
//!
string CORE::cer_decrypt(string input_cer)
{
    return input_cer;
}
//!
//! \brief CORE::cer_bind
//! \param input_cer
//! \param input_cer_len
//! \return Yes or No
int CORE::cer_bind(string input_cer, int input_cer_len)
{
    return SUCCEED;
}
//!
//! \brief CORE::cer_unbind
//! \param input_cer
//! \param input_cer_len
//! \return
//!
int CORE::cer_unbind(string input_cer, int input_cer_len)
{
    return SUCCEED;
}
//!
//! \brief CORE::cer_isbind
//! \param input_cer
//! \param input_cer_len
//! \return
//!
int CORE::cer_isbind(string input_cer, int input_cer_len)
{
    return SUCCEED;
}
//!
//! \brief CORE::cer_section_auth
//! \param input_section
//! \return
//!
int CORE::cer_section_auth(string input_section)
{
    return SUCCEED;
}
//!
//! \brief CORE::genr_wave
//! \param input_section
//! \param input_data
//! \param out_genr_result
//! \return
//!
short *CORE::genr_wave(string input_section, string input_data, int out_genr_result)
{

}
//!
//! \brief CORE::recog_stop
//! \return
//!
int CORE::recog_stop()
{
    stop();
}
//!
//! \brief CORE::recog_start
//! \param input_cfg
//! \return
//!
int CORE::recog_start(Recog_cfg input_cfg)
{
    //本次参数未初始化
    if(local_init == 0){

        if(input_cfg.input_fft_points == 0){
            //@默认的配置参数
            printf("use default args !\n");
            double sFeq[]={16, 12, 8, 4};//{5.5125};
            double eFeq[]={20, 16, 12, 8};//{11.025};
            local_cfg.input_buffer_len = 2048;
            local_cfg.input_end_feq    = eFeq;
            local_cfg.input_fft_chains = 3;
            local_cfg.input_gap        = 512;
            local_cfg.input_st         = 0.15;
            local_cfg.input_start_feq  = sFeq;
            dem_init(local_cfg.input_fft_points,\
                     local_cfg.input_st,         \
                     local_cfg.input_gap,        \
                     local_cfg.input_start_feq,  \
                     local_cfg.input_end_feq,    \
                     local_cfg.input_fft_chains);
        }else{
           dem_init(input_cfg.input_fft_points,\
                    input_cfg.input_st,         \
                    input_cfg.input_gap,        \
                    input_cfg.input_start_feq,  \
                    input_cfg.input_end_feq,    \
                    input_cfg.input_fft_chains);
           //保存配置信息到本地
   //        memcpy(local_cfg,input_cfg,sizeof(input_cfg));
        }
        //初始化标记
        local_init = 1;
    }
    start();
}
//!
//! \brief CORE::recog_setconfig
//! \param input_cfg
//! \return
//!
int CORE::recog_setconfig(Recog_cfg input_cfg)
{
    //先停下来,然后重新配置参数
    stop();
    recog_start(input_cfg);

}
//!
//! \brief CORE::recog_getconfig
//! \param input_cfg
//! \return
//!
int CORE::recog_getconfig(Recog_cfg input_cfg)
{
 // memcpy((void *)input_cfg,(void *)local_cfg,sizeof(local_cfg));
  //  local_cfg.input_buffer_len = input_cfg.b
}
//!
//! \brief CORE::recog_write
//! \param input_data
//! \param input_data_len
//! \return
//!
int CORE::recog_write(short *input_data, int input_data_len)
{
    wirte(input_data,input_data_len);
}
//!
//! \brief CORE::recog_status
//! \param output_status
//! \return
//!
int CORE::recog_status(Recog_status output_status)
{

      local_status.output_blank_buffer = fillRate();
      local_status.output_ss           = ST;
      local_status.output_recog_state  = thread_1_stause;
      return 0;
}
