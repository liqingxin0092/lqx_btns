/**
*******************************************************************************
* @file        pid_ctrl.h
* @author      liqingxin0092
* @version     V1.0
* @date        2025-4-7
* @brief       pid控制
* 更新记录:    无
* 这pid,目前是最完善的了,对于作者来说
*****************************************************************************************
*/

/*配置模板*/
///*电机的pid结构体*/
////转速:0~100   ---->  CCR:0~100
// pid_t motor_pid_struct=
//{
//   /* kp */
//     .kp= 1.09,
//   /* ki */
//     .ki=0.0014,
//   /* kd */
//     .kd=0.05,
//   /* tar_val */
//     .tar_val= 0,
//   /* 积分误差累计值上限*/
//     .inc_max= 0,
//   /* 积分误差累计值下限*/
//     .inc_min= 0,
//   /* 积分加入的阈值*/
//     .inc_use_threshold= 40,
//   /* 输出值上限 */
//     .out_max=40,
//   /* 输出值下限 */
//     .out_min= 0,
//   /* 微分项过去权重 */
//     .dpart=0,
//   /* 输出偏移 */
//     .out_offset= 0,
//   /* 输出死区(误差容忍) */
//     .out_threshold= 0,
// };

#ifndef __PID_CTRL_H
#define __PID_CTRL_H

#include "stdint.h"

/*pid控制器结构体*/
// 一定要各自用各自的变量
typedef struct
{
    float kp;
    float ki;
    float kd;
    float tar_val;

    /*积分限幅*/
    float inc_max;
    float inc_min;

    /*积分加入的阈值*/
    float inc_use_threshold;

    /*输出限幅*/
    float out_max;
    float out_min;

    float dpart; // 过去的权重

    /*输出偏移*/
    float out_offset;

    /*输出死区*/
    float out_threshold;

    /*中间变量*/
    float err;
    float err_sum;

    /*微分先行中间变量*/
    float act;
    float act_last;

    /*微分滤波中间变量*/
    float dout;
    
    /*个性*/
    void *data;
    
} pid_t;

float pid_control(pid_t *pid_struct, float act_val);
float pid_control_gunzi(pid_t *pid_struct, float act_val, short gyro);

#endif
